#pragma once
#ifndef SAE_LIB_PUN_WRAPPER_H
#define SAE_LIB_PUN_WRAPPER_H

#include "SAELib_Concepts.h"

namespace sae
{

	/*
		Use this to add array functions to a type with no added overhead. Usage Example:
		struct Foo_Base
		{
			union
			{
				struct
				{
					int a;
					int b;
				};
				std::array<int, 2> whateverYouWantMan{};
			};
		};
		using Foo = ArrayMemberWrapper<Foo_Base, &Foo_Base::whateverYouWantMan>;
	*/
	template <typename T, auto ArrayMemberPtr> requires(std::is_member_object_pointer_v<decltype(ArrayMemberPtr)>)
	class PunWrapper : public T
	{
	private:
		using this_type = PunWrapper<T, ArrayMemberPtr>;
		using wrap_array_type = remove_scope_t<decltype(ArrayMemberPtr)>;

	protected:
		constexpr auto& get_array() noexcept
		{
			return (this->*ArrayMemberPtr);
		};
		constexpr const auto& get_array() const noexcept
		{
			return (this->*ArrayMemberPtr);
		};

	public:
		using value_type = typename wrap_array_type::value_type;
		using pointer = typename wrap_array_type::pointer;
		using reference = typename wrap_array_type::reference;
		using const_pointer = typename wrap_array_type::const_pointer;
		using const_reference = typename wrap_array_type::const_reference;

		using size_type = typename wrap_array_type::size_type;

		using iterator = typename wrap_array_type::iterator;
		using const_iterator = typename wrap_array_type::const_iterator;
		using reverse_iterator = typename wrap_array_type::reverse_iterator;
		using const_reverse_iterator = typename wrap_array_type::const_reverse_iterator;

		constexpr reference at(size_type n)
		{
			return this->get_array().at(n);
		};
		constexpr const_reference at(size_type n) const
		{
			return this->get_array().at(n);
		};

		constexpr reference operator[](size_type n)
		{
			return this->at(n);
		};
		constexpr const_reference operator[](size_type n) const
		{
			return this->at(n);
		};

		constexpr iterator begin() noexcept
		{
			return this->get_array().begin();
		};
		constexpr const_iterator begin() const noexcept
		{
			return this->get_array().cbegin();
		};
		constexpr const_iterator cbegin() const noexcept
		{
			return this->get_array().cbegin();
		};

		constexpr iterator end() noexcept
		{
			return this->get_array().end();
		};
		constexpr const_iterator end() const noexcept
		{
			return this->get_array().cend();
		};
		constexpr const_iterator cend() const noexcept
		{
			return this->get_array().cend();
		};

		constexpr reverse_iterator rbegin() noexcept
		{
			return this->get_array().rbegin();
		};
		constexpr const_reverse_iterator rbegin() const noexcept
		{
			return this->get_array().crbegin();
		};
		constexpr const_reverse_iterator crbegin() const noexcept
		{
			return this->get_array().crbegin();
		};

		constexpr reverse_iterator rend() noexcept
		{
			return this->get_array().rend();
		};
		constexpr const_reverse_iterator rend() const noexcept
		{
			return this->get_array().crend();
		};
		constexpr const_reverse_iterator crend() const noexcept
		{
			return this->get_array().crend();
		};

		constexpr size_type size() const noexcept
		{
			return this->get_array().size();
		};

		constexpr auto fill(const_reference _val)
			noexcept(noexcept(std::declval<this_type>().get_array().fill(std::declval<const_reference>())))
			requires requires (wrap_array_type _array, const_reference _fVal) { _array.fill(_fVal); }
		{
			return this->get_array().fill(_val);
		};
		constexpr auto fill(value_type&& _val)
			noexcept(noexcept(std::declval<this_type>().get_array().fill(std::move(std::declval<value_type>()))))
			requires requires (wrap_array_type _array, value_type&& _fVal) { _array.fill(std::move(_fVal)); }
		{
			return this->get_array().fill(std::move(_val));
		};

		template <typename _FwdIter>
		constexpr auto assign(_FwdIter _begin, _FwdIter _end)
		{
			std::copy(_begin, _end, this->begin());
		};

		constexpr pointer data() noexcept
		{
			return this->get_array().data();
		};
		constexpr const_pointer data() const noexcept
		{
			return this->get_array().data();
		};

		constexpr reference front() noexcept { return this->get_array().front(); };
		constexpr const_reference front() const noexcept { return this->get_array().front(); };

		constexpr reference back() noexcept { return this->get_array().back(); };
		constexpr const_reference back() const noexcept { return this->get_array().back(); };

		using T::T;
		
	};

}

#endif