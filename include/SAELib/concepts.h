#pragma once
#ifndef SAELIB_CONCEPTS_H
#define SAELIB_CONCEPTS_H

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
	This is a semi-standin concepts library that was created before MSVC had a standard concepts library. 
	Also contains various concepts that are frequently useful
*/

#include "type.h"

#include <concepts>
#include <type_traits>
#include <limits>
#include <utility>

namespace sae
{
	/*
		Type comparison
	*/

	using std::is_same;
	using std::is_same_v;

	template< class T, class U >
	concept cx_same_as = is_same_v<T, U>;



	using std::is_convertible;
	using std::is_convertible_v;

	template <class From, class To>
	concept cx_convertible_to = is_convertible_v<From, To>;

	

	template <typename T, typename... Ts>
	struct is_any_of : std::bool_constant<(std::is_same_v<T, Ts> || ...)> {};

	template <typename T, typename... Ts>
	constexpr static bool is_any_of_v = is_any_of<T, Ts...>::value;

	template <typename T, typename... Ts>
	concept cx_any_of = is_any_of_v<T, Ts...>;





	/*
		Character type traits and concept
	*/


	template <typename T>
	struct is_character : public is_any_of<T,
		unsigned char,
		char,
		char8_t,
		char16_t,
		char32_t,
		wchar_t
	> {};

	template <typename T>
	constexpr static bool is_character_v = is_character<T>::value;

	template <typename T>
	concept cx_character = is_character_v<T>;



	/*
		Basic type catagory concepts
	*/


	using std::is_fundamental;
	using std::is_fundamental_v;

	template <typename T>
	concept cx_fundamental = is_fundamental_v<T>;



	using std::is_integral;
	using std::is_integral_v;

	template <typename T>
	concept cx_integral = is_integral_v<T>;
	


	using std::is_arithmetic;
	using std::is_arithmetic_v;

	template <typename T>
	concept cx_arithmetic = is_arithmetic_v<T>;
	


	using std::is_floating_point;
	using std::is_floating_point_v;

	template <typename T>
	concept cx_floating_point = is_floating_point_v<T>;



	using std::is_signed;
	using std::is_signed_v;

	template <typename T>
	concept cx_signed = is_signed_v<T>;



	using std::is_unsigned;
	using std::is_unsigned_v;

	template <typename T>
	concept cx_unsigned = is_unsigned_v<T>;



	template <typename T>
	constexpr static bool is_integer_v = is_arithmetic_v<T> && std::numeric_limits<T>::is_integer;

	template <typename T>
	struct is_integer : public std::bool_constant<is_integer_v<T>> {};

	template <typename T>
	concept cx_integer = is_integer_v<T>;










	/*
		Constructor related concepts
	*/

	using std::is_constructible;
	using std::is_constructible_v;

	template <typename T, typename... Args>
	concept cx_constructible = is_constructible_v<T, Args...>;




	using std::is_copy_constructible;
	using std::is_copy_constructible_v;

	template <typename T>
	concept cx_copy_constructible = is_copy_constructible_v<T>;



	using std::is_copy_assignable;
	using std::is_copy_assignable_v;

	template <typename T>
	concept cx_copy_assignable = is_copy_assignable_v<T>;



	using std::is_move_constructible;
	using std::is_move_constructible_v;

	template <typename T>
	concept cx_move_constructible = is_move_constructible_v<T>;



	using std::is_move_assignable;
	using std::is_move_assignable_v;

	template <typename T>
	concept cx_move_assignable = is_move_assignable_v<T>;



	using std::is_default_constructible;
	using std::is_default_constructible_v;

	template <typename T>
	concept cx_default_constructible = is_default_constructible_v<T>;



	/*
		Trivial type concepts
	*/

	using std::is_trivially_default_constructible;
	using std::is_trivially_default_constructible_v;

	template <typename T>
	concept cx_trivially_default_constructible = is_trivially_default_constructible_v<T>;



	using std::is_trivially_copy_constructible;
	using std::is_trivially_copy_constructible_v;

	template <typename T>
	concept cx_trivially_copy_constructible = is_trivially_copy_constructible_v<T>;



	using std::is_trivially_copy_assignable;
	using std::is_trivially_copy_assignable_v;

	template <typename T>
	concept cx_trivially_copy_assignable = is_trivially_copy_assignable_v<T>;



	using std::is_trivially_copyable;
	using std::is_trivially_copyable_v;

	template <typename T>
	concept cx_trivially_copyable = is_trivially_copyable_v<T>;




	using std::is_trivially_move_constructible;
	using std::is_trivially_move_constructible_v;

	template <typename T>
	concept cx_trivially_move_constructible = is_trivially_move_constructible_v<T>;



	using std::is_trivially_move_assignable;
	using std::is_trivially_move_assignable_v;

	template <typename T>
	concept cx_trivially_move_assignable = is_trivially_move_assignable_v<T>;



	template <typename T>
	constexpr static bool is_trivially_moveable_v = is_trivially_move_assignable_v<T> && is_trivially_move_constructible_v<T>;

	template <typename T>
	struct is_trivially_moveable : public std::bool_constant<is_trivially_moveable_v<T>> {};

	template <typename T>
	concept cx_trivially_moveable = is_trivially_moveable_v<T>;



	using std::is_trivially_assignable;
	using std::is_trivially_assignable_v;

	template <typename From, typename To>
	concept cx_trivially_assignable = is_trivially_assignable_v<From, To>;



	using std::is_trivially_constructible;
	using std::is_trivially_constructible_v;

	template <typename T>
	concept cx_trivially_constructible = is_trivially_constructible_v<T>;



	using std::is_trivially_default_constructible;
	using std::is_trivially_default_constructible_v;

	template <typename T>
	concept cx_trivially_destructible = is_trivially_default_constructible_v<T>;

	using std::is_trivial;
	using std::is_trivial_v;

	template <typename T>
	concept cx_trivial = is_trivial_v<T>;




	template <typename From, typename To>
	constexpr static bool is_forwardable_to_v = is_constructible_v<To, std::remove_reference_t<From>>;

	template <typename From, typename To>
	struct is_forwardable_to : public std::bool_constant<is_forwardable_to_v<From, To>> {};

	template <typename From, typename To>
	concept cx_forwardable_to = is_forwardable_to_v<From, To>;



	/*
		Function related concepts
	*/

	using std::is_function;
	using std::is_function_v;

	template <typename T>
	concept cx_function = is_function_v<T>;



	template <typename T, typename = void>
	struct is_function_pointer : public std::false_type {};
	
	template <typename RetT, typename... Args>
	struct is_function_pointer<RetT(*)(Args...), void> : public std::true_type {};
	
	template <typename RetT, typename ScopeT, typename... Args>
	struct is_function_pointer<RetT(ScopeT::*)(Args...), void> : public std::true_type {};

	template <typename T>
	constexpr static bool is_function_pointer_v = is_function_pointer<T>::value;

	template <typename T>
	concept cx_function_pointer = is_function_pointer_v<T>;






	/*
		Member function pointer concepts
	*/
	
	using std::is_member_function_pointer;
	using std::is_member_function_pointer_v;

	template <typename T>
	concept cx_member_function_pointer = is_member_function_pointer_v<T>;



	/*
		Free function pointer concepts
	*/

	template <typename T>
	constexpr static bool is_free_function_pointer_v = is_function_pointer_v<T> && (!is_member_function_pointer_v<T>);

	template <typename T>
	struct is_free_function_pointer : std::bool_constant<is_free_function_pointer_v<T>> {};

	template <typename T>
	concept cx_free_function_pointer = is_free_function_pointer_v<T>;





};

#endif