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
	


}