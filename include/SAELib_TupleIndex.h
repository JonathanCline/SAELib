#pragma once
#ifndef SAELIB_TUPLE_INDEX_H
#define SAELIB_TUPLE_INDEX_H

/*
	Copyright 2021 Jonathan Cline
	Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files
	(the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge,
	publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do
	so, subject to the following conditions:
	The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
	WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
	COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
	OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
	Code to get the position of an element in a tuple on compile time.
	Should work back to c++17 but not tested
*/

#include <type_traits>
#include <tuple>

namespace sae
{

	namespace impl
	{
		template <typename T, typename U, size_t Idx>
#ifdef __cpp_consteval
		consteval
#else
		constexpr
#endif
			inline size_t tuple_index_finder_idx() noexcept
		{
			return ((std::is_same_v<T, U>) ? Idx : 0);
		};

		template <typename T, typename... Ts, size_t... Idxs>
#ifdef __cpp_concepts
		requires (std::is_same_v<T, Ts> || ...)
#endif
#ifdef __cpp_consteval
			consteval
#else
			constexpr
#endif
			inline auto tuple_index_finder(std::tuple<Ts...>* _tup, std::index_sequence<Idxs...> _isq) noexcept
		{
			constexpr size_t _index = (tuple_index_finder_idx<T, Ts, Idxs>() + ...);
			return _index;
		};
	};

	template <typename T, typename Tup, typename = void>
	struct tuple_element_index;

	template <typename T, typename... Ts>
#ifdef __cpp_concepts
	requires ((((std::is_same_v<T, Ts>) ? 1 : 0) + ...) == 1)
#endif
		struct tuple_element_index<T, std::tuple<Ts...>, void>
	{
		using type = size_t;
		constexpr static type value = impl::tuple_index_finder<T>((std::tuple<Ts...>*)(nullptr), std::make_index_sequence<sizeof...(Ts)>{});
	};

	template <typename T, typename Tup>
#ifdef __cpp_concepts
	requires requires () { typename tuple_element_index<T, Tup>::type; }
#endif
	constexpr inline size_t tuple_element_index_v = tuple_element_index<T, Tup>::value;

};

#endif
