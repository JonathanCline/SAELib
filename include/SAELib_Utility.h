#pragma once

#include <array>
#include <utility>

namespace sae
{
	using std::nothrow_t;
	using std::nothrow;

	namespace literals
	{
		constexpr static char8_t operator""_c8(char _c) noexcept
		{
			return (char8_t)_c;
		};
		constexpr static char16_t operator""_c16(char _c) noexcept
		{
			return (char16_t)_c;
		};
		constexpr static char32_t operator""_c32(char _c) noexcept
		{
			return (char32_t)_c;
		};
	};
	
	using namespace literals;


	constexpr static size_t strlen(const char* _cString) noexcept
	{
		size_t _out = 0;
		for (auto c = _cString; true; ++c)
		{
			++_out;
			if (*c == '\0')
			{
				break;
			};
		};
		return _out;
	};
	constexpr static size_t strlen(const char8_t* _cString8) noexcept
	{
		size_t _out = 0;
		for (auto c = _cString8; true; ++c)
		{
			++_out;
			if (*c == '\0'_c8)
			{
				break;
			};
		};
		return _out;
	};
	constexpr static size_t strlen(const char16_t* _cString16) noexcept
	{
		size_t _out = 0;
		for (auto c = _cString16; true; ++c)
		{
			++_out;
			if (*c == '\0'_c16)
			{
				break;
			};
		};
		return _out;
	}; 
	constexpr static size_t strlen(const char32_t* _cString32) noexcept
	{
		size_t _out = 0;
		for (auto c = _cString32; true; ++c)
		{
			++_out;
			if (*c == '\0'_c32)
			{
				break;
			};
		};
		return _out;
	};



};
