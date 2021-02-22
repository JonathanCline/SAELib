#pragma once


namespace sae
{

	template <typename T>
	class cursor
	{
	public:
		using position_type = uint32_t;
		using offset_type = int32_t;

		using iterator = T;

	private:
		static iterator next(iterator _at, offset_type _n) noexcept
		{
			return std::next(_at, _n);
		};
		static offset_type distance(iterator _begin, iterator _end) noexcept
		{
			return std::distance(_begin, _end);
		};

	public:
		T head() const noexcept { return this->at_; };
		T begin() const noexcept { return this->begin_; };
		T end() const noexcept { return this->end_; };

		cursor& set(iterator i)
		{
			this->at_ = i;
			return *this;
		};

		cursor& seek(offset_type _n)
		{
			return this->set(next(this->head(), _n));
		};

		cursor& next(offset_type _n)
		{
			return this->seek(_n);
		};
		cursor& next()
		{
			return this->next(1);
		};

		cursor& back(offset_type _n)
		{
			return this->seek(-_n);
		};
		cursor& back()
		{
			return this->back(1);
		};

		cursor& operator+=(offset_type _n) noexcept
		{
			return this->seek(_n);
		};
		cursor& operator-=(offset_type _n) noexcept
		{
			return this->seek(_n);
		};

		cursor& operator++() noexcept
		{
			return this->next();
		};
		cursor& operator--() noexcept
		{
			return this->back();
		};

		cursor& seek_pos(position_type _n)
		{
			return this->set(next(this->begin(), _n));
		};

		cursor& seek_begin()
		{
			return this->set(this->begin());
		};
		cursor& seek_end()
		{
			return this->set(this->end());
		};

		position_type pos() const noexcept
		{
			return distance(this->begin(), this->at_);
		};

		constexpr cursor(T _begin, T _end, T _at) noexcept :
			begin_{ _begin }, end_{ _end }, at_{ _at }
		{};

		template <std::ranges::range RangeT>
		constexpr cursor(RangeT& _range, T _at) noexcept :
			cursor{ std::ranges::begin(_range), std::ranges::end(_range), _at }
		{};

	private:
		T begin_;
		T end_;
		T at_;

	};

};
