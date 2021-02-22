#pragma once

#include "SAELib_Singleton.h"

#include <type_traits>
#include <concepts>
#include <chrono>

namespace sae
{

	template <typename T>
	concept cx_clock = requires(T a)
	{
		typename T::duration;
		typename T::time_point;
		{ a.now() } -> std::convertible_to<typename T::time_point>;
	};

	template <typename T>
	struct is_clock : public std::false_type {};
	template <cx_clock T>
	struct is_clock<T> : public std::true_type {};

	template <cx_clock T>
	constexpr static bool is_clock_v = is_clock<T>::value;

};

#ifndef SAE_THREAD_CLOCK_TYPE 
#define SAE_THREAD_CLOCK_TYPE ::std::chrono::steady_clock
#endif

namespace sae
{
	using clock_t = SAE_THREAD_CLOCK_TYPE;
	static_assert(sae::is_clock_v<clock_t>, "SAEThread.h : Bad clock type given by SAE_THREAD_CLOCK_TYPE");
};

namespace sae
{

	namespace impl
	{
		struct SAELib_Thread_H_ClockSingletonTag {};
	};

	static clock_t& clock() noexcept
	{
		return get_singleton<clock_t, impl::SAELib_Thread_H_ClockSingletonTag>();
	};

	using time_point = typename clock_t::time_point;
	using duration = typename clock_t::duration;

	using hours = std::chrono::hours;
	using minutes = std::chrono::minutes;
	using seconds = std::chrono::seconds;
	using milliseconds = std::chrono::milliseconds;
	using microseconds = std::chrono::microseconds;
	using nanoseconds = std::chrono::nanoseconds;

	using namespace std::chrono_literals;

}