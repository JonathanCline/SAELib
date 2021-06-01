#pragma once

#include <algorithm>
#include <ranges>
#include <utility>

namespace sae
{
	template <typename T, typename ValT> requires requires (const T& a, const ValT& b) { std::ranges::find(a, b); }
	constexpr bool contains(const T& _range, const ValT& _cmp)
	{
		return std::ranges::find(_range, _cmp) != std::ranges::end(_range);
	};
	template <typename T, typename Pred> requires requires (const T& a, const Pred& p) { std::ranges::find_if(a, p); }
	constexpr bool contains_if(const T& _range, Pred&& _pred)
	{
		return std::ranges::find_if(_range, std::forward<Pred>(_pred)) != std::ranges::end(_range);
	};
}