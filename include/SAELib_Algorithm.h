#pragma once

#include <algorithm>
#include <utility>
#include <concepts>

namespace sae
{
	template <typename IterT, typename ValT>
	static auto find(IterT _begin, IterT _end, const ValT& _val)
	{
		return std::find(_begin, _end, _val);
	};
	template <typename IterT, typename ValT, typename OpT>
	static auto find_if(IterT _begin, IterT _end, OpT&& _op)
	{
		return std::find_if(_begin, _end, std::forward<OpT>(_op));
	};

	// template <typename OpT, typename RetT, typename Args...>
	// concept cx_invoke_result = requires

	template <typename IterT, typename OpT> requires requires(OpT _op, IterT _iter)
	{
		{ std::invoke(_op, *_iter) } -> std::same_as<bool>;
	}
	static auto rfind_if(IterT _begin, IterT _end, OpT&& _op)
	{
		auto _at = _end;
		do
		{
			_at = std::next(_at, -1);
			if (std::invoke(_op, *_at))
			{
				break;
			};
		} while (_at != _begin);
		return _at;
	};

	template <typename ValT>
	constexpr static auto lm_equals(const ValT& _val)
	{
		return [_val](const ValT& lhs)
		{
			return lhs == _val;
		};
	};

	template <typename IterT, typename ValT>
	static auto rfind(const IterT _begin, const IterT _end, const ValT& _val)
	{
		return rfind_if<IterT, ValT>(_begin, _end, lm_equals(_val));
	};
	
	template <typename IterT, typename T>
	static bool contains(const IterT _begin, const IterT _end, const T& _compare)
	{
		return std::find(_begin, _end, _compare) != _end;
	};
	template <typename IterT, typename Op>
	static bool contains_if(const IterT _begin, const IterT _end, Op&& _op)
	{
		return std::find_if(_begin, _end, std::forward<Op>(_op)) != _end;
	};

	template <typename T, typename ValT> requires requires (T a, ValT v) { sae::contains(a.begin(), a.end(), v); }
	static bool contains(const T& _container, const ValT& _compare)
	{
		return sae::contains(_container.begin(), _container.end(), _compare);
	};
	template <typename T, typename Op> requires requires (T a, Op&& v) { sae::contains_if(a.begin(), a.end(), std::forward<Op>(v)); }
	static bool contains_if(const T& _container, Op&& _op)
	{
		return sae::contains_if(_container.begin(), _container.end(), std::forward<Op>(_op));
	};

}