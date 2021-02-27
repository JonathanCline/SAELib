#pragma once
#ifndef SAELIB_TOKEN_H
#define SAELIB_TOKEN_H

#include <string_view>
#include <string>
#include <vector>
#include <cstdint>
#include <algorithm>

namespace sae
{
	template <typename _Elem, typename _Traits = std::char_traits<_Elem>, typename _Alloc = std::allocator<std::basic_string_view<_Elem, _Traits>>>
	using basic_token_list = std::vector<std::basic_string_view<_Elem, _Traits>, _Alloc>;

	using token_list = basic_token_list<char>;

	namespace impl
	{
		template <typename T, typename _Elem, typename _Traits>
		static std::vector<std::string_view> split(const std::basic_string_view<_Elem, _Traits>& _str, const T& _token, size_t _tokenLen)
		{
			size_t _offset = 0;
			size_t _atpos = 0;
			std::vector<std::basic_string_view<_Elem, _Traits>> _tokens{};
			while ((_atpos = _str.find(_token, _offset)) != std::basic_string_view<_Elem, _Traits>::npos)
			{
				_tokens.push_back(_str.substr(_offset, _atpos - _offset));
				_offset = _atpos + _tokenLen;
			};
			_tokens.push_back(_str.substr(_offset));
			return _tokens;
		};
	};

	template <typename _Elem, typename _Traits>
	static basic_token_list<_Elem, _Traits> split(const std::basic_string_view<_Elem, _Traits>& _str, const _Elem _token)
	{
		return impl::split(_str, _token, 1);
	};
	template <typename _Elem, typename _Traits>
	static basic_token_list<_Elem, _Traits> split(const std::basic_string_view<_Elem, _Traits>& _str, const _Elem* _token)
	{
		return impl::split(_str, _token, std::strlen(_token));
	};
	template <typename _Elem, typename _Traits>
	static basic_token_list<_Elem, _Traits> split(const std::basic_string_view<_Elem, _Traits>& _str, const std::basic_string_view<_Elem, _Traits>& _token)
	{
		return impl::split(_str, _token, _token.size());
	};

	template <typename _Elem, typename _Traits, typename _Alloc, typename T>
	static basic_token_list<_Elem, _Traits> split(const std::basic_string<_Elem, _Traits, _Alloc>& _str, const T& _token)
	{
		return split(std::basic_string_view<_Elem, _Traits>{ _str }, _token);
	};

	template <typename _Elem, typename _Traits, typename _Alloc>
	static size_t erase_empty(basic_token_list<_Elem, _Traits, _Alloc>& _tokens)
	{
		return std::erase_if(_tokens, [](const auto& _t) { return _t.empty(); });
	};

	template <typename _Str, typename _Token> requires requires (const _Str& _cs, const _Token& _ctoken)
	{
		split(_cs, _ctoken);
	}
	static auto split_not_empty(const _Str& _s, const _Token& _token)
	{
		auto _tkns = split(_s, _token);
		erase_empty(_tkns);
		return _tkns;
	};

};


#endif