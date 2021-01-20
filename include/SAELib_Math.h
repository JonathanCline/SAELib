#pragma once

#include <numbers>
#include <cmath>

namespace sae
{

	template <typename T>
	static inline auto plateu(T _x, T _rate, T _max)
	{
		return _max * (1.0 - std::pow(std::numbers::e, -_rate * _x));
	};
	template <typename T>
	static inline auto plateu(T _x, T _rate)
	{
		return (1.0 - std::pow(std::numbers::e, -_rate * _x));
	};

	template <typename T>
	static inline auto logistic(T _x, T _rate, T _max, T _midpoint)
	{
		return _max / ((T)1 + std::pow(std::numbers::e, -_rate * (_x - _midpoint)));
	};



}