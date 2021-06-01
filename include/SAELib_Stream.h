#pragma once
#ifndef SAELIB_STREAM_H
#define SAELIB_STREAM_H

#include "SAELib_Type.h"

#include <istream>
#include <iterator>
#include <type_traits>
#include <algorithm>
#include <vector>
#include <iostream>

namespace sae
{
	constexpr static size_t DRAIN_READ_CHUNK_SIZE_V = 1024;

	template <typename IterT, typename T = dereference_type_t<IterT>> requires std::is_trivially_copy_assignable_v<T>
	static IterT drain_into(std::istream& _istr, const IterT _destBegin, const IterT _destEnd)
	{
		auto _outIter = _destBegin;

		const size_t _destSize = std::distance(_destBegin, _destEnd);
		
		if constexpr (std::is_trivially_copy_assignable_v<T>)
		{
			constexpr size_t _bufferLen = DRAIN_READ_CHUNK_SIZE_V;
			const auto _maxReadCount = std::min<size_t>(_destSize, _bufferLen);

			T _readBuff[_bufferLen]{ T{} };
			std::fill_n(_readBuff, _bufferLen, T{});

			auto _remainingCount = _destSize;

			while (!_istr.eof() && _remainingCount != 0)
			{
				_istr.read((char*)(&_readBuff[0]), std::min(_remainingCount, _bufferLen));
				const auto _gotCount = _istr.gcount();
				using size_type = decltype(_gotCount);
				#ifndef NDEBUG
				if (!(_gotCount <= static_cast<size_type>(_bufferLen))) { std::terminate(); };
				if (!(_gotCount <= static_cast<size_type>(_remainingCount))) { std::terminate(); };
				#endif
				_outIter = std::copy_n(_readBuff, _gotCount, _outIter);
				_remainingCount -= _gotCount;
			};
		};

		return _outIter;
	};

	template <typename T = std::byte> requires std::is_trivially_copy_assignable_v<T>
	static std::vector<T> drain(std::istream& _istr, T* _rbuff, size_t _rbuffLen)
	{
		using return_type = std::vector<T>;
		using iterator = typename return_type::iterator;

		return_type _out{};
		_out.resize(_rbuffLen);

		const auto _buffEnd = _rbuff + _rbuffLen;

		while (!_istr.eof())
		{
			auto _buffPtr = drain_into<iterator, T>(_istr, _rbuff, _rbuff + _rbuffLen);
			if (_buffPtr == _buffEnd && !_istr.eof())
			{
				_out.resize(_out.size() + _rbuffLen);
			}
			else
			{
				auto _endIt = _out.end();
				auto _eraseAfterIt = _endIt - std::distance(_buffPtr, _buffEnd);
				_out.erase(_eraseAfterIt, _endIt);
				break;
			};
		};

		return _out;
	};

	template <typename T = std::byte> requires std::is_trivially_copy_assignable_v<T>
	static std::vector<T> drain(std::istream& _istr)
	{
		using return_type = std::vector<T>;
		using iterator = typename return_type::iterator;

		return_type _out{};
		_out.resize(DRAIN_READ_CHUNK_SIZE_V);
		
		auto _beginIt = _out.begin();

		while (!_istr.eof())
		{
			auto _dIt = drain_into<iterator, T>(_istr, _beginIt, _out.end());
			if (_dIt == _out.end() && !_istr.eof())
			{
				_out.resize(_out.size() + DRAIN_READ_CHUNK_SIZE_V);
				_beginIt = _out.end() - DRAIN_READ_CHUNK_SIZE_V;
			}
			else
			{
				_out.erase(_dIt, _out.end());
				break;
			};
		};

		return _out;
	};



	/**
	 * @brief Sets std::cout and std::cin to be redirected into a different set of streams
	 * @param _stdin std::cout will now output into this istream
	 * @param _stdout std::cin will now input into this ostream
	*/
	static void redirect_standard_io(std::istream& _stdin, std::ostream& _stdout)
	{
		std::cout.rdbuf(_stdin.rdbuf());
		std::cin.rdbuf(_stdout.rdbuf());
	};



}


#endif