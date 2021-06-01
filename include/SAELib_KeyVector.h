#pragma once

#include <vector>
#include <utility>
#include <cstdint>
#include <algorithm>

namespace sae
{

	template <typename Key, typename T>
	class kvector
	{
	public:
		using key_type = Key;
		using value_type = T;

		using entry = std::pair<key_type, value_type>;

		using size_type = size_t;
	private:
		using container_type = std::vector<entry>;

		static bool key_compare(const key_type& _key, const entry& _entry)
		{
			return _entry.first == _key;
		};
		constexpr static auto lm_entry_keytest(const key_type& _key)
		{
			return [_key](const entry& _val)
			{
				return _val.first == _key;
			};
		};

	public:
		using iterator = typename container_type::iterator;
		using const_iterator = typename container_type::const_iterator;

		iterator begin() noexcept { return this->data_.begin(); };
		const_iterator begin() const noexcept { return this->data_.cbegin(); };
		const_iterator cbegin() const noexcept { return this->data_.cbegin(); };

		iterator end() noexcept { return this->data_.end(); };
		const_iterator end() const noexcept { return this->data_.cend(); };
		const_iterator cend() const noexcept { return this->data_.cend(); };

		iterator find(const key_type& _key) noexcept
		{
			return std::find_if(this->begin(), this->end(), this->lm_entry_keytest(_key));
		};
		const_iterator find(const key_type& _key) const noexcept
		{
			return std::find_if(this->begin(), this->end(), this->lm_entry_keytest(_key));
		};

		bool contains(const key_type& _key) const noexcept
		{
			return this->find(_key) != this->end();
		};


		auto insert(const entry& _entryPair)
		{
			return this->data_.insert(this->end(), _entryPair);
		};
		auto insert(entry&& _entryPair)
		{
			return this->data_.insert(this->end(), std::move(_entryPair));
		};

		auto erase(const key_type& _key) noexcept
		{
			return std::erase_if(this->data_, this->lm_entry_keytest(_key));
		};

		void clear() noexcept
		{
			this->data_.clear();
		};

		size_type size() const noexcept
		{
			return this->data_.size();
		};

		value_type& at(const key_type& _key)
		{
			return this->find(_key)->second;
		};
		const value_type& at(const key_type& _key) const
		{
			return this->find(_key)->second;
		};

		value_type& operator[](const key_type& _key)
		{
			return this->at(_key);
		};
		const value_type& operator[](const key_type& _key) const
		{
			return this->at(_key);
		};

	private:
		container_type data_;
	};

	template <typename T>
	class id_vector : public kvector<uint32_t, T>
	{
	public:
		using id = uint32_t;
		using value_type = kvector<uint32_t, T>::value_type;

		void set_idcount(id _toNum) noexcept
		{
			this->id_count_ = _toNum;
		};
		id get_idcount() const noexcept
		{
			return this->id_count_;
		};

		id insert(value_type _val)
		{
			const auto _id = ++this->id_count_;
			kvector<id, value_type>::insert({ _id, std::move(_val) });
			return _id;
		};

	private:
		id id_count_ = 0;
	};

};
