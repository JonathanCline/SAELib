#pragma once
#ifndef SAELIB_MEMORY_H
#define SAELIB_MEMORY_H

#include <memory>
#include <concepts>
#include <type_traits>

namespace sae
{
	template <typename T, typename... TArgs>
	concept can_make_unique = requires(TArgs... _ts)
	{
		std::unique_ptr<T>{ new T(std::forward<TArgs>(_ts)...) };
		std::make_unique<T>(std::forward<TArgs>(_ts)...);
	};

	template <typename T, typename... Ts> requires can_make_unique<T, Ts...>
	static std::unique_ptr<T> make_unique(Ts&&... _cargs)
	{
		return std::make_unique<T>( std::forward<Ts>(_cargs)... );
	};


	template <typename T, typename... Ts>
	concept can_make_shared = requires(Ts... _ts)
	{
		std::shared_ptr<T>{ new T(std::forward<Ts>(_ts)...) };
		std::make_shared<T>(std::forward<Ts>(_ts)...);
	};
	
	template <typename T, typename... Ts> requires can_make_shared<T, Ts...>
	static std::shared_ptr<T> make_shared(Ts&&... _cargs)
	{
		return std::make_shared<T>( std::forward<Ts>(_cargs)... );
	};
};

#endif