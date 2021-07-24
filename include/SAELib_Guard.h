#pragma once

#include <utility>

namespace sae
{
	template <auto DestroyFunc>
	class scope_watch
	{
	public:

		void release()
		{
			if (this->state_)
			{
				this->state_ = false;
				std::invoke(DestroyFunc);
			};
		};

		constexpr scope_watch() noexcept :
			state_{ true }
		{};

		scope_watch(const scope_watch& other) = delete;
		scope_watch& operator=(const scope_watch& other) = delete;

		constexpr scope_watch(scope_watch&& other) noexcept :
			state_{ std::exchange(other.state_, false) }
		{};
		scope_watch& operator=(scope_watch&& other)
		{
			this->release();
			this->state_ = std::exchange(other.state_, false);
			return *this;
		};

		~scope_watch()
		{
			this->release();
		};

	private:
		bool state_ = false;
	};

};