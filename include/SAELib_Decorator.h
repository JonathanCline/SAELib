#pragma once

#include "SAELib_Type.h"

namespace sae
{

	template <typename T>
	struct Decorate_ForwardIterator
	{
	private:
		auto& get_derived_container() noexcept
		{
			return static_cast<T*>(this)->get_container();
		};
		const auto& get_derived_container() const noexcept
		{
			return static_cast<const T*>(this)->get_container();
		};

	public:
		auto begin() noexcept { return this->get_derived_container().begin(); };
		auto begin() const noexcept { return this->get_derived_container().cbegin(); };
		auto cbegin() const noexcept { return this->get_derived_container().cbegin(); };

		auto end() noexcept { return this->get_derived_container().end(); };
		auto end() const noexcept { return this->get_derived_container().cend(); };
		auto cend() const noexcept { return this->get_derived_container().cend(); };

	};

	template <typename T>
	struct Decorate_ReverseIterator
	{
	private:
		auto& get_derived_container() noexcept
		{
			return static_cast<T*>(this)->get_container();
		};
		const auto& get_derived_container() const noexcept
		{
			return static_cast<T*>(this)->get_container();
		};

	public:
		auto rbegin() noexcept { return this->get_derived_container().rbegin(); };
		auto rbegin() const noexcept { return this->get_derived_container().crbegin(); };
		auto crbegin() const noexcept { return this->get_derived_container().crbegin(); };

		auto rend() noexcept { return this->get_derived_container().rend(); };
		auto rend() const noexcept { return this->get_derived_container().crend(); };
		auto crend() const noexcept { return this->get_derived_container().crend(); };

	};
	
	template <typename T>
	struct Decorate_Iterator : public Decorate_ForwardIterator<Decorate_Iterator<T>>, public Decorate_ReverseIterator<Decorate_Iterator<T>>
	{	
	private:
	 	friend Decorate_ForwardIterator<Decorate_Iterator<T>>;
		friend Decorate_ReverseIterator<Decorate_Iterator<T>>;

		auto& get_derived_container() noexcept
		{
			return static_cast<T*>(this)->get_container();
		};
		const auto& get_derived_container() const noexcept
		{
			return static_cast<const T*>(this)->get_container();
		};

	protected:
		auto& get_container() noexcept { return this->get_derived_container(); };
		const auto& get_container() const noexcept { return this->get_derived_container(); };

	};

	namespace impl
	{

		template <typename T, typename = void> struct Decorate_ForwardIterator_TypeAliases;
		template <typename T> requires requires 
		{ 
			typename T::iterator; typename T::const_iterator; 
		}
		struct Decorate_ForwardIterator_TypeAliases<T, void>
		{
			using iterator = typename T::iterator;
			using const_iterator = typename T::const_iterator;
		};

		template <typename T, typename = void> struct Decorate_ReverseIterator_TypeAliases;
		template <typename T> requires requires { typename T::reverse_iterator; typename T::const_reverse_iterator; }
		struct Decorate_ReverseIterator_TypeAliases<T, void>
		{
			using reverse_iterator = typename T::reverse_iterator;
			using const_reverse_iterator = typename T::const_reverse_iterator;
		};

	};

	template <typename ContainerT>
	struct Decorate_IteratorTypeAliases : 
		public conditional_type_template_t<impl::Decorate_ForwardIterator_TypeAliases, ContainerT>,
		public conditional_type_template_t<impl::Decorate_ReverseIterator_TypeAliases, ContainerT>
	{};
	
	template <typename T>
	constexpr static bool has_forward_iterator_v = is_valid_template_parameter_list_v<impl::Decorate_ForwardIterator_TypeAliases, T>;
	template <typename T>
	constexpr static bool has_reverse_iterator_v = is_valid_template_parameter_list_v<impl::Decorate_ReverseIterator_TypeAliases, T>;

	template <typename T, typename ContainerT>
	struct Decorate_ContainerIterator :
		public conditional_type_t<impl::Decorate_ForwardIterator_TypeAliases<ContainerT>, has_forward_iterator_v<ContainerT>>,
		public conditional_type_t<Decorate_ForwardIterator<Decorate_ContainerIterator<T, ContainerT>>, has_forward_iterator_v<ContainerT>>,
		public conditional_type_t<impl::Decorate_ReverseIterator_TypeAliases<ContainerT>, has_reverse_iterator_v<ContainerT>>,
		public conditional_type_t<Decorate_ReverseIterator<Decorate_ContainerIterator<T, ContainerT>>, has_reverse_iterator_v<ContainerT>>
	{
	private:
		friend Decorate_ForwardIterator<Decorate_ContainerIterator<T, ContainerT>>;
		friend Decorate_ReverseIterator<Decorate_ContainerIterator<T, ContainerT>>;
		auto& get_container() noexcept { return static_cast<T*>(this)->get_container(); };
		const auto& get_container() const noexcept { return static_cast<const T*>(this)->get_container(); };

	public:

	};

	template <typename T, typename ValueT>
	struct Decorate_Vector : public Decorate_Iterator<Decorate_Vector<T, ValueT>>
	{
	private:
		friend Decorate_Iterator<Decorate_Vector<T, ValueT>>;

		auto& get_derived_container() noexcept
		{
			return static_cast<T*>(this)->get_container();
		};
		const auto& get_derived_container() const noexcept
		{
			return static_cast<const T*>(this)->get_container();
		};

	protected:
		auto& get_container() noexcept { return this->get_derived_container(); };
		const auto& get_container() const noexcept { return this->get_derived_container(); };

	public:
		using size_type = size_t;

		using value_type = ValueT;
		using pointer = ValueT*;
		using reference = ValueT&;
		using const_pointer = const ValueT*;
		using const_reference = const ValueT&;

		auto size() const noexcept { return this->get_container().size(); };
		void resize(size_type _size) noexcept { this->get_container().resize(_size); };
		
		auto capacity() const noexcept { return this->get_container().capacity(); };
		void reserve(size_type _size) noexcept { this->get_container().reserve(_size); };

		auto& front() noexcept { return this->get_container().front(); };
		const auto& front() const noexcept { return this->get_container().front(); };

		auto& back() noexcept { return this->get_container().back(); };
		const auto& back() const noexcept { return this->get_container().back(); };

		auto data() const noexcept { return this->get_container().data(); };

		auto& at(size_type _index) { return this->get_container().at(_index); };
		const auto& at(size_type _index) const { return this->get_container().at(_index); };
		
		auto& operator[](size_type _index) { return this->get_container()[_index]; };
		const auto& operator[](size_type _index) const { return this->get_container()[_index]; };

		void push_back(const_reference _value) { this->get_container().push_back(_value); };
		void push_back(value_type&& _value) { this->get_container().push_back(std::move(_value)); };

		void pop_back() noexcept { this->get_container().pop_back(); };

		void insert(const_reference _value) { this->get_container().push_back(_value); };
		void insert(value_type&& _value) { this->get_container().push_back(std::move(_value)); };

	};

}