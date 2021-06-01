#pragma once

#include "SAELib_Singleton.h"

#include <type_traits>
#include <concepts>
#include <chrono>
#include <time.h>
#include <string>
#include <iomanip>
#include <string_view>
#include <array>
#include <cstdint>

#include <ostream>

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
	




	template <typename T>
	concept cx_real_clock = cx_clock<T> && requires(const T& a, std::time_t _t)
	{
		{ a.to_time_t(typename T::time_point{}) } -> std::convertible_to<std::time_t>;
		{ a.from_time_t(_t) } -> std::convertible_to<typename T::time_point>;
	};




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





	enum class Month
	{
		January = 0,
		February,
		March,
		April,
		May,
		June,
		July,
		August,
		September,
		October,
		November,
		December
	};

	namespace impl
	{
		constexpr static std::array<std::string_view, 12> month_names_v
		{
			"January",
			"February",
			"March",
			"April",
			"May",
			"June",
			"July",
			"August",
			"September",
			"October",
			"November",
			"December"
		};
	};





	constexpr static std::string_view to_string(Month _m) noexcept
	{
		return impl::month_names_v[(size_t)_m];
	};

	

	enum class Day
	{
		Sunday = 0,
		Monday, 
		Tuesday, 
		Wednesday,
		Thursday,
		Friday,
		Saturday
	};

	namespace impl
	{
		constexpr static std::array<std::string_view, 7> day_names_v
		{
			"Sunday",
			"Monday",
			"Tuesday",
			"Wednesday",
			"Thursday",
			"Friday",
			"Saturday"
		};
	};

	constexpr static std::string_view to_string(Day _m) noexcept
	{
		return impl::day_names_v[(size_t)_m];
	};

	struct timestamp
	{
		constexpr static timestamp from_tm(const std::tm* _time) noexcept
		{
			timestamp _out{};
			_out.year = _time->tm_year + 1900;
			_out.month = Month{ _time->tm_mon + 1 };
			_out.day = _time->tm_mday;
			_out.hour = hours{ _time->tm_hour };
			_out.min = minutes{ _time->tm_min };
			_out.sec = seconds{ _time->tm_sec };
			_out.dst = _time->tm_isdst;
			_out.yearday = _time->tm_yday;
			_out.weekday = Day{ _time->tm_wday };
			return _out;
		};

		// True if daylight savings time
		bool dst = false;

		// Years
		int year = 1970;

		// Month of the year
		Month month = Month::January;

		// Days since beginning of the month
		int day = 0;

		// Hours since start of day
		hours hour{ 0 };

		// Minutes since start of hour
		minutes min{ 0 };

		// Second since start of minute
		seconds sec{ 0 };

		// Day of the week
		Day weekday = Day::Thursday;
		
		// Days since staart of year
		int yearday = 0;

		constexpr timestamp() noexcept = default;
	
		constexpr timestamp(const std::tm* _time) noexcept :
			timestamp{ from_tm(_time) } 
		{};
		timestamp& operator=(const std::tm* _time) noexcept
		{
			return ((*this) = this->from_tm(_time));
		};

	};
	
	constexpr static timestamp unix_epoch_timestamp()
	{
		timestamp _out{};

		_out.dst = false;
		_out.year = 1970;
		_out.month = Month::January;
		_out.day = 0;
		_out.hour = hours{ 0 };
		_out.min = minutes{ 0 };
		_out.sec = seconds{ 0 };

		_out.weekday = Day::Thursday;
		_out.yearday = 0;

		return _out;
	};



	template <cx_real_clock ClockT>
	static timestamp local_timestamp(const ClockT& _clock = ClockT{})
	{
		const auto _ctime = _clock.to_time_t(_clock.now());
#if defined(WIN32)
		tm _buffer[1]{};
		const auto _error = (int)localtime_s(&_buffer[0], &_ctime);
		const auto _tlocal = &_buffer[0];
#else
		const auto _tlocal = std::localtime(&_ctime);
		const auto _error = errno();
#endif
	#ifndef NDEBUG
		if (_error != 0)
		{
			std::exit(_error);
		};
	#endif
		return timestamp{ _tlocal };
	};
	
	static timestamp local_timestamp()
	{
		return local_timestamp<std::chrono::system_clock>();
	};



	

};
