#pragma once

#include <future>
#include <ranges>
#include <algorithm>
#include <cstdint>
#include <atomic>
#include <concepts>
#include <compare>
#include <exception>

namespace sae
{
	// Request ID to use when associating requests / responses
	using request_id = uint32_t;

	template <typename T>
	concept message = true;

	// Request ID handling
	constexpr static request_id nullreq = 0;
	namespace impl
	{
		extern inline std::atomic<request_id> request_id_counter{ 0u };
	};
	static request_id new_request_id() { return ++impl::request_id_counter; };

	// associates an ID with a type
	template <typename T>
	struct identifiable
	{
	public:
		T* operator->() { return &this->data; };
		const T* operator->() const { return &this->data; };

		template <typename _T>
		constexpr bool operator==(const identifiable<_T>& _other) const noexcept { return this->id == _other.id; };
		constexpr bool operator==(request_id _other) const noexcept { return this->id == _other; };

		request_id id;
		T data;
	};

	// Unparsed response type (now just an alias as response is templated)
	template <message MessageT>
	using unparsed_response = identifiable<MessageT>;

	template <typename T>
	using response = identifiable<T>;

	// Response reciever base, allows for later type erasure
	template <message MessageT>
	struct abstract_receiver
	{
		virtual void handle(unparsed_response<MessageT>&& _response) = 0;
		virtual ~abstract_receiver() = default;
	};

	// Given to the hub telling it which reciever to give recieved data to
	template <message MessageT>
	using request_token = identifiable<abstract_receiver<MessageT>*>;

	// CRTP type for adding recievers with async facilities easily
	template <message MessageT, typename T, typename DataT>
	struct basic_receiver : public abstract_receiver<MessageT>
	{
	public:
		using message_type = MessageT;

		using value_type = DataT;
		using promise_type = identifiable<std::promise<value_type>>;
		using future_type = std::future<value_type>;
	private:
		using container_type = std::vector<promise_type>;
		T* crtp() { return static_cast<T*>(this); };
		const T* crtp() const { return static_cast<const T*>(this); };
	public:
		future_type request(const request_id _useID)
		{
			promise_type _promise{ _useID };
			auto _out = _promise->get_future();
			this->pms.push_back(std::move(_promise));
			return _out;
		};
		void handle(unparsed_response<message_type>&& _response) final
		{
			// Find associated promise, if it exists
			auto& _promises = this->pms;
			const auto _begin = std::ranges::begin(_promises);
			const auto _end = std::ranges::end(_promises);
			const auto _it = std::find(_begin, _end, _response);

			// If the response is promised, then ask the CRTP'd type to parse the data and fill promise
			if (_it != _end)
			{
				auto& _promise = _it->data;
				auto _crtp = this->crtp();
				try
				{
					_promise.set_value(_crtp->parse(_response.data));
				}
				catch (...)
				{
					_promise.set_exception(std::current_exception());
				};

				// Remove associated promise
				const auto _eraseIt = std::next(_end, -1);
				std::iter_swap(_it, _eraseIt);
				_promises.erase(_eraseIt, _end);
			};
		};

	private:
		container_type pms{};
	};

	template <typename T, typename MessageT>
	concept receiver = message<MessageT> && std::derived_from<T, abstract_receiver<MessageT>> &&
		requires(T a, request_id _id)
	{
		a.request(_id);
	};

	// Stores request data and pushes recieved data to the correct reciever
	template <message MessageT>
	class request_hub
	{
	public:
		using message_type = MessageT;
		auto insert(request_token<message_type> _token)
		{
			const auto _id = _token.id;
			this->rtokens.push_back(_token);
			return _id;
		};

		// Returns true if response was handled
		bool handle(unparsed_response<message_type> _rawResponse)
		{
			// Find the request token with matching ID if it exists
			auto& _tokens = this->rtokens;
			const auto _tkBegin = std::ranges::begin(_tokens);
			const auto _tkEnd = std::ranges::end(_tokens);
			const auto _tkIter = std::find(_tkBegin, _tkEnd, _rawResponse);
			auto _out = (_tkIter != _tkEnd);

			// Pass response to the token's reciever if a request token was found
			if (_out)
			{
				const auto& _receiver = _tkIter->data;
				_receiver->handle(std::move(_rawResponse));

				// Remove token from the list
				const auto _eraseAfter = std::next(_tkEnd, -1);
				std::iter_swap(_tkIter, _eraseAfter);
				_tokens.erase(_eraseAfter, _tkEnd);
			};
			return _out;
		};

	private:
		std::vector<request_token<message_type>> rtokens;
	};

};
