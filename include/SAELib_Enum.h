#pragma once
#ifndef SAELIB_ENUM_H
#define SAELIB_ENUM_H

#include <type_traits>
#include <concepts>
#include <compare>

namespace sae
{
	template <typename T>
	concept cx_enum = std::is_enum_v<T>;

	template <typename BaseT, cx_enum EnumT, std::integral ValueT, typename TagT>
	class enum_decorator : public BaseT
	{
	protected:
		using base_type = BaseT;
		using tag_type = TagT;

	public:
		using value_type = ValueT;
		using enum_type = EnumT;

		constexpr value_type value() const noexcept { return this->val_; };
		constexpr enum_type value_e() const noexcept { return (enum_type)this->value(); };

		constexpr explicit operator value_type() const noexcept { return this->val_; };
		constexpr operator enum_type() const noexcept { return (enum_type)this->val_; };

		friend constexpr inline auto operator|(const enum_decorator& _lhs, const value_type& _rhs) noexcept
		{
			return enum_decorator{ _lhs.value() | _rhs };
		}; 
		friend constexpr inline auto& operator|=(enum_decorator& _lhs, const value_type& _rhs) noexcept
		{
			_lhs.val_ |= _rhs;
			return _lhs;
		};
		
		friend constexpr inline auto operator|(const enum_decorator& _lhs, const enum_type& _rhs) noexcept
		{
			return enum_decorator{ _lhs.value() | (value_type)_rhs };
		};
		friend constexpr inline auto& operator|=(enum_decorator& _lhs, const enum_type& _rhs) noexcept
		{
			_lhs.val_ |= (value_type)_rhs;
			return _lhs;
		};

		friend constexpr inline auto operator|(const enum_decorator& _lhs, const enum_decorator& _rhs) noexcept
		{
			return enum_decorator{ _lhs.value() | _rhs.value() };
		};
		friend constexpr inline auto& operator|=(enum_decorator& _lhs, const enum_decorator& _rhs) noexcept
		{
			_lhs.val_ |= _rhs.value();
			return _lhs;
		};

		friend constexpr inline auto operator&(const enum_decorator& _lhs, const value_type& _rhs) noexcept
		{
			return enum_decorator{ _lhs.value() & _rhs };
		};
		friend constexpr inline auto& operator&=(enum_decorator& _lhs, const value_type& _rhs) noexcept
		{
			_lhs.val_ &= _rhs;
			return _lhs;
		};

		friend constexpr inline auto operator&(const enum_decorator& _lhs, const enum_type& _rhs) noexcept
		{
			return enum_decorator{ _lhs.value() & (value_type)_rhs };
		};
		friend constexpr inline auto& operator&=(enum_decorator& _lhs, const enum_type& _rhs) noexcept
		{
			_lhs.val_ &= (value_type)_rhs;
			return _lhs;
		};

		friend constexpr inline auto operator&(const enum_decorator& _lhs, const enum_decorator& _rhs) noexcept
		{
			return enum_decorator{ _lhs.value() & _rhs.value() };
		};
		friend constexpr inline auto& operator&=(enum_decorator& _lhs, const enum_decorator& _rhs) noexcept
		{
			_lhs.val_ &= _rhs.value();
			return _lhs;
		};

		friend constexpr inline auto operator^(const enum_decorator& _lhs, const value_type& _rhs) noexcept
		{
			return enum_decorator{ _lhs.value() ^ _rhs };
		};
		friend constexpr inline auto& operator^=(enum_decorator& _lhs, const value_type& _rhs) noexcept
		{
			_lhs.val_ ^= _rhs;
			return _lhs;
		};

		friend constexpr inline auto operator^(const enum_decorator& _lhs, const enum_type& _rhs) noexcept
		{
			return enum_decorator{ _lhs.value() ^ (value_type)_rhs };
		};
		friend constexpr inline auto& operator^=(enum_decorator& _lhs, const enum_type& _rhs) noexcept
		{
			_lhs.val_ ^= (value_type)_rhs;
			return _lhs;
		};

		friend constexpr inline auto operator^(const enum_decorator& _lhs, const enum_decorator& _rhs) noexcept
		{
			return enum_decorator{ _lhs.value() ^ _rhs.value() };
		};
		friend constexpr inline auto& operator^=(enum_decorator& _lhs, const enum_decorator& _rhs) noexcept
		{
			_lhs.val_ ^= _rhs.value();
			return _lhs;
		};

		constexpr enum_decorator operator~() const noexcept
		{
			return enum_decorator{ ~this->value() };
		};

		friend inline constexpr auto operator<=>(const enum_decorator& lhs, const enum_decorator& rhs) noexcept
		{
			return lhs.value() <=> rhs.value();
		};

		friend inline constexpr auto operator<=>(const enum_decorator& lhs, const value_type& rhs) noexcept
		{
			return lhs.value() <=> rhs;
		};
		friend inline constexpr auto operator<=>(const value_type& lhs, const enum_decorator& rhs) noexcept
		{
			return lhs <=> rhs.value();
		};

		friend inline constexpr auto operator<=>(const enum_decorator& lhs, const EnumT& rhs) noexcept
		{
			return lhs.value() <=> (value_type)rhs;
		};
		friend inline constexpr auto operator<=>(const EnumT& lhs, const enum_decorator& rhs) noexcept
		{
			return (value_type)lhs <=> rhs.value();
		};

		using BaseT::BaseT;

		explicit constexpr enum_decorator(value_type _val) noexcept :
			val_{ _val }
		{};
		enum_decorator& operator=(value_type _val) noexcept
		{
			this->val_ = _val;
			return *this;
		};

		constexpr enum_decorator(enum_type _val) noexcept :
			val_{ (value_type)_val }
		{};
		enum_decorator& operator=(enum_type _val) noexcept
		{
			this->val_ = (value_type)_val;
		};

		constexpr enum_decorator(const enum_decorator& _val) noexcept = default;
		enum_decorator& operator=(const enum_decorator& _val) noexcept = default;
		constexpr enum_decorator(enum_decorator&& _val) noexcept = default;
		enum_decorator& operator=(enum_decorator&& _val) noexcept = default;

	protected:
		value_type val_;
	};

};

#endif