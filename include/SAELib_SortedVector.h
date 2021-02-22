#pragma once

#include <SAELib_Concepts.h>
#include <SAELib_Decorator.h>

#include <memory>
#include <cstdint>
#include <algorithm>
#include <numeric>
#include <vector>


#if false

namespace sae
{


	template <typename T, typename AllocT = std::allocator<T>>
	class sorted_vector : public Decorate_Iterator<sorted_vector<T, AllocT>>
	{
	public:
		using value_type = T;
		using pointer = value_type*;
		using reference = value_type&;
		using const_pointer = const value_type*;
		using const_reference = const value_type&;

		using allocator_type = AllocT;
		using size_type = size_t;

	private:
		friend Decorate_Iterator<sorted_vector<T, AllocT>>;

		using container_type = typename std::vector<value_type, allocator_type>;

		container_type& get_container() noexcept { return this->data_; };
		const container_type& get_container() const noexcept { return this->data_; };

	public:
		using iterator = typename container_type::iterator;
		using const_iterator = typename container_type::const_iterator;
		using reverse_iterator = typename container_type::reverse_iterator;
		using const_reverse_iterator = typename container_type::const_reverse_iterator;

	private:

		struct Range
		{
			using value_type = typename pointer;
			value_type first;
			value_type second;
			Range(value_type _f, value_type _s) noexcept :
				first{ _f }, second{ _s }
			{};
		};

		auto check_range(Range _in, const T& _key) noexcept
		{
			auto _middle = std::midpoint(_in.first, _in.second);
			if (_key < *_middle)
			{
				return Range(_in.first, _middle);
			}
			else
			{
				return Range(_middle, _in.second);
			};
		};

		auto find_lower_bound(const T& _k)
		{
			Range _r{ this->get_container().data(), this->get_container().data() + this->get_container().size() };
			while (std::distance(_r.first, _r.second) > 1)
			{
				_r = check_range(_r, _k);
			};
			return _r.first;
		};


	public:



		template <cx_forward<value_type> _T>
		void insert(_T&& _val)
		{
			auto _it = this->find_lower_bound(_val);
			if ((this->data_.size() == 1) || (_it != this->data_.data() && _it != this->data_.data() + this->data_.size()))
			{
				++_it;
			};
			this->get_container().insert(this->begin() + std::distance(this->data_.data(), _it), std::forward<_T>(_val));
		};





	private:
		container_type data_;

	};





};

#endif
