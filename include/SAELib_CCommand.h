#pragma once
#ifndef SAELIB_CCOMMAND_H
#define SAELIB_CCOMMAND_H

#include "SAELib_Functor.h"
#include "SAELib_Decorator.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <concepts>
#include <string_view>

namespace sae
{

	template <typename CallbackT, typename KeyT = std::string>
	class basic_command_set : public Decorate_ContainerIterator<basic_command_set<CallbackT, KeyT>, std::unordered_map<KeyT, CallbackT>>
	{
	public:
		using key_type = KeyT;
		using callback_type = CallbackT;
		using pair_type = std::pair<key_type, callback_type>;

	private:
		friend Decorate_ContainerIterator<basic_command_set<callback_type, key_type>, std::unordered_map<key_type, callback_type>>;

		auto& get_container() noexcept { return this->cset_; };
		const auto& get_container() const noexcept { return this->cset_; };

	public:
		auto find(const key_type& _key) { return this->get_container().find(_key); };
		auto find(const key_type& _key) const { return this->get_container().find(_key); };

		void insert(const key_type& _key, callback_type _cb)
		{
			this->get_container().insert({ _key, std::move(_cb) });
		};
		bool contains(const key_type& _key) const
		{
			return this->get_container().contains(_key);
		};
		void erase(const key_type& _key)
		{
			this->get_container().erase(_key);
		};
		void clear() noexcept
		{
			this->get_container().clear();
		};

		auto& at(const key_type& _key) { return this->get_container().at(_key); };
		const auto& at(const key_type& _key) const { return this->get_container().at(_key); };

		template <typename... Ts> requires requires { std::invoke(std::declval<callback_type>(), std::forward<Ts>(std::declval<Ts&&>())...); }
		bool invoke(const key_type& _key, Ts&&... _args)
		{
			auto _it = this->find(_key);
			auto _found = _it != this->end();
			if (_found)
			{
				std::invoke(_it->second, std::forward<Ts>(_args)...);
			};
			return _found;
		};

		basic_command_set() = default;

		basic_command_set(std::initializer_list<pair_type> _vals) :
			cset_{ _vals.begin(), _vals.end() }
		{};
		basic_command_set& operator=(std::initializer_list<pair_type> _vals)
		{
			this->get_container() = _vals;
			return *this;
		};

	private:
		std::unordered_map<key_type, callback_type> cset_{};

	};

	using command_callback = functor<void(const std::vector<std::string>& _args)>;
	using command_set = basic_command_set<command_callback>;
	
	static auto parse_command_line(const int _nargs, char* _args[])
	{
		std::vector<std::string> _out{};
		_out.resize(_nargs);
		for (int i = 0; i < _nargs; ++i)
		{
			_out[i] = _args[i];
		};
		return _out;
	};

};

#endif