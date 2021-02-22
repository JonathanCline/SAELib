#pragma once
#ifndef SAELIB_TYPE_H
#define SAELIB_TYPE_H

#include <type_traits>

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

}

#endif