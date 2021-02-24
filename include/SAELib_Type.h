#pragma once
#ifndef SAELIB_TYPE_H
#define SAELIB_TYPE_H

#include <type_traits>
#include <concepts>
#include <cstdint>
#include <tuple>

namespace sae
{

	template <typename T>
	struct type_wrapper
	{
		using type = T;
	};

	template <typename T>
	using type_wrapper_t = typename type_wrapper<T>::type;

	struct null_t
	{
		using type = void;
	};
	
	struct empty_t {};

	namespace impl
	{
		template <typename IterT>
		constexpr static auto dereference_type_helper(IterT _iter) noexcept
		{
			return sae::type_wrapper_t< std::remove_cvref_t< decltype(*_iter) > >{};
		};
	};

	template <typename T>
	struct dereference_type
	{
		using type = typename decltype(impl::dereference_type_helper(std::declval<T>()))::type;
	};

	template <typename T>
	using dereference_type_t = typename dereference_type<T>::type;

	template <auto SelectIndex, typename... Ts> requires(SelectIndex < sizeof...(Ts) && std::convertible_to<decltype(SelectIndex), size_t>)
		struct type_switch
	{
	private:
		using tuple_type = std::tuple<Ts...>;
	public:
		constexpr static auto index = SelectIndex;
		using type = std::tuple_element_t<(size_t)index, tuple_type>;
	};

	template <auto SelectIndex, typename... Ts> requires requires { type_switch<SelectIndex, Ts...>{}; }
	using type_switch_t = typename type_switch<SelectIndex, Ts...>::type;



	namespace impl
	{
		template <template <typename...> typename T, typename... U>
		concept valid_template_parameter_list = std::is_default_constructible_v<T<U...>> && requires
		{
			T<U...>{};
		};
		template<template <typename...> typename T, typename... U> requires (!valid_template_parameter_list<T, U...>)
		consteval static bool is_valid_template_parameter_list_helper()
		{
			return false;
		};
		template<template <typename...> typename T, typename... U> requires valid_template_parameter_list<T, U...>
			consteval static bool is_valid_template_parameter_list_helper()
		{
			return true;
		};
	};

	template<template <typename...> typename T, typename... U>
	struct is_valid_template_parameter_list
	{
		constexpr static bool value = impl::is_valid_template_parameter_list_helper<T, U...>();
	};
	template <template <typename...> typename T, typename... U>
	constexpr static bool is_valid_template_parameter_list_v = is_valid_template_parameter_list<T, U...>::value;

	template <typename T, bool Condition>
	struct conditional_type;

	template <typename T>
	struct conditional_type<T, true>
	{
		using type = T;
	};
	template <typename T>
	struct conditional_type<T, false>
	{
		using type = empty_t;
	};

	template <typename T, bool Condition>
	using conditional_type_t = typename conditional_type<T, Condition>::type;

	template <template <typename...> typename T, typename... TArgs>
	using conditional_type_template_t = conditional_type_t<T<TArgs...>, is_valid_template_parameter_list_v<T, TArgs...>>;




	template <typename ScopeT>
	struct class_member_variable_breakdown;

	template <typename ScopeT, typename MemberT>
	struct class_member_variable_breakdown<MemberT ScopeT::*>
	{
		using class_type = ScopeT;
		using member_type = MemberT;
		using type = MemberT ScopeT::*;
	};

	template <typename T> struct class_member_variable_class_type;
	template <typename ScopeT, typename MemberT> struct class_member_variable_class_type<MemberT ScopeT::*>
	{
		using type = ScopeT;
	};
	template <typename T> requires requires { class_member_variable_class_type<T>{}; }
	using class_member_variable_class_type_t = typename class_member_variable_class_type<T>::type;

	template <typename T> struct class_member_variable_member_type;
	template <typename ScopeT, typename MemberT> struct class_member_variable_member_type<MemberT ScopeT::*>
	{
		using type = MemberT;
	};
	template <typename T> requires requires { class_member_variable_member_type<T>{}; }
	using class_member_variable_member_type_t = typename class_member_variable_member_type<T>::type;





};

#endif