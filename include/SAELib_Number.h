#pragma once

#include <SAELib_Concepts.h>
#include <SAELib_Singleton.h>

#include <random>

namespace sae
{
	template <typename T> requires requires 
	{
		std::numeric_limits<T>::max();
		std::numeric_limits<T>::min();
	}
	constexpr static auto limits()
	{
		using lmtype = std::numeric_limits<T>;
		return std::pair{ lmtype::min(), lmtype::max() };
	};

	template <typename T>
	requires requires
	{
		limits<T>();
	}
	constexpr static auto max()
	{
		auto [_min, _max] = limits<T>();
		return _max;
	};

	template <typename T>
	requires requires
	{
		limits<T>();
	}
	constexpr static auto min()
	{
		auto [_min, _max] = limits<T>();
		return _min;
	};

	struct random_source
	{
		std::random_device device{};
		std::mt19937 engine{ this->device() };
	};
	static auto& default_random_engine()
	{
		static random_source _src{};
		return _src.engine;
	};

	template <typename T>
	concept cx_arithmetic = std::is_arithmetic_v<T>;

	template <typename T>
	concept cx_number = cx_arithmetic<T> && !cx_same_as<T, bool>;

	template <typename T>
	concept cx_random_engine = requires(T a, std::uniform_int_distribution<int> b, std::uniform_real_distribution<float> c)
	{
		b(a);
		c(a);
	};

	template <cx_random_engine EngineT, cx_number T>
	static T rand(EngineT& _engine, T _min, T _max)
	{
		if constexpr (std::is_floating_point_v<T>)
		{
			return std::uniform_real_distribution<T>{_min, _max}(_engine);
		}
		else if constexpr (std::is_integral_v<T>)
		{
			return std::uniform_int_distribution<T>{_min, _max}(_engine);
		};
	};
	template <cx_number T>
	static T rand(T _min, T _max)
	{
		return rand(default_random_engine(), _min, _max);
	};
	
	template <cx_number T = int>
	static T rand()
	{
		auto [_min, _max] = limits<T>();
		return rand<T>(_min, _max);
	};

};