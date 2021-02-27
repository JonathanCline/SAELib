#pragma once
#ifndef SAELIB_COMMNANDLINE_H
#define SAELIB_COMMNANDLINE_H

#include "SAELib_CCommand.h"

namespace sae
{

	static void parse_command_line(const int _nargs, char* _args[], command_set& _cset)
	{
		auto _tokens = parse_command_line(_nargs, _args);
		if (!_tokens.empty())
		{
			std::string _cmd = _tokens.front();
			std::iter_swap(_tokens.begin(), _tokens.end() - 1);
			_tokens.erase(_tokens.end() - 1);
			if (_cset.contains(_cmd))
			{
				_cset.invoke(_cmd, _tokens);
			};
		};
	};


};

#endif