#pragma once

#include "SAELib_Time.h"

#include <concepts>
#include <thread>

namespace sae
{


	template <cx_clock ClockT>
	struct basic_timer
	{
	public:
		using clock_type = ClockT;
		using duration = typename clock_type::duration;
		using time_point = typename clock_type::time_point;

		auto& clock() noexcept { return this->clock_; };
		const auto& clock() const noexcept { return this->clock_; };

		void start()
		{
			this->start_ = this->clock().now();
		};
		time_point start_time() const noexcept
		{
			return this->start_;
		};

		duration timer_duration() const noexcept
		{
			return this->tdur_;
		};
		void set_timer_duration(duration _dur) noexcept
		{
			this->tdur_ = _dur;
		};

		duration elapsed_time() const noexcept
		{
			return this->clock().now() - this->start_time();
		};
		duration remaining_time() const noexcept
		{
			return this->timer_duration() - this->elapsed_time();
		};

		bool finished() const noexcept { return this->elapsed_time() > this->timer_duration(); };

		basic_timer(duration _dur, clock_type _clock = clock_type{}) :
			tdur_{ _dur }, clock_{ std::move(_clock) }
		{};
		basic_timer(clock_type _clock = clock_type{}) :
			basic_timer{ duration{}, std::move(_clock) }
		{};

	private:
		time_point start_;
		duration tdur_{};
		mutable clock_type clock_;

	};

	using timer = basic_timer<clock_t>;
	
	template <cx_clock ClockT>
	void sleep_until(const basic_timer<ClockT>& _timer)
	{
		sleep_until(_timer.remaining_time() + _timer.start_time());
	};

	enum : bool
	{
		ivTimeout,
		ivGood
	};

	template <sae::cx_clock ClockT, typename DurationT, typename Op> requires std::convertible_to<std::invoke_result_t<Op>, bool>
	static bool invoke_with_timeout(ClockT _clock, DurationT _duration, Op _op, int _attempts = 10)
	{
		basic_timer<ClockT> _timer{ _duration, std::move(_clock) };
		bool _out = ivTimeout;
		_timer.start();
		while (!_timer.finished())
		{
			if (!std::invoke(_op))
			{
				std::this_thread::sleep_for(_timer.timer_duration() / _attempts);
			}
			else
			{
				_out = ivGood;
			};
		};
		return _out;
	};

	template <typename Op> requires std::convertible_to<std::invoke_result_t<Op>, bool>
	static bool invoke_with_timeout(const duration& _duration, Op _op, int _attempts = 10)
	{
		return invoke_with_timeout(clock(), _duration, _op, _attempts);
	};

}