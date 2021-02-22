#pragma once
#ifndef SAE_LIB_CONCEPTS_H
#define SAE_LIB_CONCEPTS_H

#include "SAELib_Type.h"

#include <concepts>
#include <type_traits>
#include <string>
#include <utility>

namespace sae
{
	template <class From, class To>
	concept cx_convertible_to =
		std::is_convertible_v<From, To> &&
		requires(std::add_rvalue_reference_t<From>(&f)()) {
		static_cast<To>(f());
	};

	namespace detail
	{
		template<class T, class U >
		concept SameHelper = std::is_same_v<T, U>;
	};
	template< class T, class U >
	concept cx_same_as = detail::SameHelper<T, U> && detail::SameHelper<U, T>;

	template <typename T, typename U>
	concept cx_constructable_from = std::is_constructible<T, U>::value;

	template <typename T, typename... CArgs>
	concept cx_constructible_from = requires (CArgs&&... _args)
	{
		T{ std::forward<CArgs>(_args)... };
	};

	template <typename T>
	concept cx_string_convertible = requires (T a)
	{
		{std::to_string(a)} -> sae::cx_same_as<std::string>;
	};

	template <typename T>
	concept cx_int_convertible = sae::cx_convertible_to<T, int> && requires (T a)
	{
		a << 4;
		a & 0x44;
	};
	template <typename T, class = void>
	struct is_int_convertible_t : std::false_type {};
	template <cx_int_convertible T>
	struct is_int_convertible_t <T, T> : std::true_type {};

	template <typename T>
	concept cx_float_convertible = sae::cx_convertible_to<T, float_t> && !requires (T a)
	{
		a << 4;
		a & 0x44;
	};
	template <typename T, class = void>
	struct is_float_convertible_t : std::false_type {};
	template <cx_float_convertible T>
	struct is_float_convertible_t<T, T> : std::true_type {};

	template <typename T>
	concept cx_copy_constructible = std::is_copy_constructible<T>::value;
	template <typename T>
	concept cx_copy_assignable = std::is_copy_assignable<T>::value;

	template <typename T>
	concept cx_move_constructible = std::is_move_constructible<T>::value;
	template <typename T>
	concept cx_move_assignable = std::is_move_assignable<T>::value;

	template <typename T>
	concept cx_default_constructible = std::is_default_constructible<T>::value;

	template <typename T>
	concept cx_completely_copyable = cx_copy_constructible<T> && cx_copy_assignable<T>;
	template <typename T>
	concept cx_completely_moveable = cx_move_constructible<T> && cx_move_assignable<T>;

	template <typename T>
	concept cx_has_all_constructors = cx_move_constructible<T> && cx_copy_constructible<T>;

	template <typename T>
	concept cx_completely_constructible = cx_default_constructible<T> && cx_completely_copyable<T> && cx_completely_moveable<T>;


	template <typename T>
	concept cx_is_function = std::is_function<T>::value;


	template <typename T>
	concept cx_is_free_function_pointer = std::is_function<T>::value;

	template <typename T>
	struct cx_is_member_function_pointer_t : public std::is_member_function_pointer<T> {};
	template <typename T>
	concept cx_is_member_function_pointer = cx_is_member_function_pointer_t<T>::value;



	template <typename T>
	concept cx_is_function_pointer = cx_is_free_function_pointer<T> || cx_is_member_function_pointer<T>;

	template <typename T>
	struct cx_NOT_t;
	template <>
	struct cx_NOT_t<std::true_type> : public std::false_type {};
	template <>
	struct cx_NOT_t<std::false_type> : public std::true_type {};

	template <typename T>
	concept cx_forward_iterator = requires(T a, T b)
	{
		{ ++a } -> cx_same_as<T&>;
		{ a++ } -> cx_same_as<T>;
		a.operator->();
		*a;
		a == b;
		a != b;
	};

	template <typename T>
	concept cx_iterator_enabled = requires (T a)
	{
		typename T::iterator;
		typename T::const_iterator;
		{a.begin()} -> cx_same_as<typename T::iterator>;
		{a.cbegin()} -> cx_same_as<typename T::const_iterator>;
		{a.end()} -> cx_same_as<typename T::iterator>;
		{a.cend()} -> cx_same_as<typename T::const_iterator>;
	};

	template <typename T>
	concept cx_floating_point = std::is_floating_point<T>::value;


	template <typename T>
	concept hashable = requires (T a)
	{
		std::hash<T>{}(a);
	};

	namespace impl
	{
		template <typename T>
		constexpr static bool is_hashable_impl() noexcept
		{
			return false;
		};
		template <hashable T>
		constexpr static bool is_hashable_impl() noexcept
		{
			return true;
		};
	};

	template <typename T>
	struct is_hashable
	{
		constexpr static bool value = impl::is_hashable_impl<T>();
	};

	template <typename T>
	constexpr static bool is_hashable_v = is_hashable<T>::value;




	namespace impl
	{
		template <typename ScopeT, typename MemberT>
		static constexpr MemberT remove_scope_arg_deduction(MemberT ScopeT::* _p)
		{};
	};

	template <typename T>
	struct remove_scope
	{
		using type = decltype(impl::remove_scope_arg_deduction(std::declval<T>()));
	};
	template <typename T>
	using remove_scope_t = typename remove_scope<T>::type;

	template <typename T, typename... Args>
	concept cx_constructible = requires(Args&&... _args)
	{
		T{ std::forward<Args>(_args)... };
	};

	template <typename T, typename U>
	concept cx_forward = cx_same_as<U, std::remove_cvref_t<T>>;



	template <typename T>
	concept cx_character = std::same_as<char, T> || std::same_as<char8_t, T> || std::same_as<char16_t, T> || std::same_as<char32_t, T>;


	template <typename T, typename... Ts>
	concept cx_one_of = (std::same_as<T, Ts> || ...);


}

#endif