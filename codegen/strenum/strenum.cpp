
#include "strenum.h"

#include <fstream>
#include <string>
#include <iostream>

namespace sae::cgen
{
	void generate_strenum(std::istream& _istr, std::ostream& _ostr, const strenum_settings& _settings)
	{

		std::vector<std::string> _evals{};

		std::string _str{};
		while (!_istr.eof())
		{
			std::getline(_istr, _str, ';');
			_evals.push_back(_str);
		};

		for (auto& v : _evals)
		{
			for (auto it = v.begin(); it < v.end();)
			{
				if (std::isspace(*it))
				{
					it = v.erase(it);
				}
				else
				{
					++it;
				};
			};
		};

		for (auto& v : _evals)
		{
			std::cout << v << '\n';
		};

		const std::string _enum_name = [](const std::string& _name)
		{
			std::string _str{ _name };
			_str.append("_E");
			return _str;
		}(_settings.name);

		_ostr << "#pragma once\n";
		_ostr << "#include <SAELib_DualMap.h>\n";
		_ostr << "#include <optional>\n";
		_ostr << "#include <string>\n\n";

		if (!_settings.use_namespace.empty())
		{
			_ostr << "namespace " << _settings.use_namespace << "\n{\n\n";
		};

		_ostr << "class " << _settings.name << " \n{\n";

		_ostr << "public: \n";
		_ostr << "\tenum " << _enum_name << "\n\t{\n";

		for (auto& v : _evals)
		{
			if (!v.empty())
			{
				_ostr << "\t\t" << v << ",\n";
			};
		};

		_ostr << "\t};\n\n";

		_ostr << "private:\n";

		_ostr << "\tconst static inline ::sae::unordered_dualmap<std::string, " << _enum_name << "> STR_ENUM_DMAP\n\t{\n";

		bool _ranOnce = false;
		for (auto& v : _evals)
		{
			if (v.empty())
			{
				continue;
			};

			if (_ranOnce)
			{
				_ostr << ",\n";
			};

			_ostr << "\t\t{ " << '"' << v << '"' << " , " << _enum_name << "::" << v << " }";
			_ranOnce = true;

		};
		_ostr << "\n\t};\n";
		_ostr << '\t' << _enum_name << " val_;\n\n";

		_ostr << "public:\n";

		std::string _opArgs = "const " + _settings.name + "& _lhs, const " + _settings.name + "& _rhs";

		_ostr << "\tfriend constexpr inline bool operator==(" << _opArgs << ") noexcept = default;\n";
		_ostr << "\tfriend constexpr inline bool operator!=(" << _opArgs << ") noexcept = default;\n";

		_ostr << "\texplicit operator bool() noexcept = delete;\n\n";


		_ostr << "\tstatic inline std::optional<" << _enum_name << "> from_string(const std::string& _str)\n";
		_ostr << "\t{\n\t\tstd::optional<" << _enum_name << "> _out{ std::nullopt };\n";
		_ostr << "\t\tif(STR_ENUM_DMAP.contains_left(_str))\n\t\t{\n\t\t\t_out = STR_ENUM_DMAP.ltor(_str);\n\t\t};\n";
		_ostr << "\t\treturn _out;\n\t};\n";

		_ostr << "\tconst std::string& to_string() const\n\t{\n";
		_ostr << "\t\treturn STR_ENUM_DMAP.rtol(this->val_);\n\t};\n\n";

		_ostr << "\tconstexpr inline operator " << _enum_name << "() const noexcept { return this->val_; };\n";
		_ostr << "\tconstexpr " << _settings.name << "(" << _enum_name << " _val) noexcept : \n\t\tval_{ _val }\n\t{};\n";
		_ostr << "\t" << _settings.name << "& operator=(" << _enum_name << " _val) noexcept {\n\t\tthis->val_ = _val;\n\t\treturn *this;\n\t};\n";



		_ostr << "};\n\n\n";
		
		if (!_settings.use_namespace.empty())
		{
			_ostr << "};\n";
		};


	};



	strenum_settings strenum_cmdline(const std::vector<std::string>& _args)
	{
		strenum_settings _out{};

		for (auto it = _args.begin(); it != _args.end(); ++it)
		{
			if (it->front() == '-')
			{
				switch ((*it)[1])
				{
				case 'i':
					++it;
					_out.input_file = *it;
					break;
				case 'o':
					++it;
					_out.output_file = *it;
					break;
				case 'n':
					++it;
					_out.name = *it;
					break;
				case 's':
					++it;
					_out.use_namespace = *it;
					break;
				default:
					abort();
				};
			};
		};

		return _out;

	};

}







/*
	strenum.exe -i <input_file> -n <enum_name> -o <output_file>
*/

int main(char _nargs, char* _vargs[], char* _enva[])
{

	std::vector<std::string> _args{};
	for (int i = 1; i < _nargs; ++i)
	{
		_args.push_back(_vargs[i]);
	};

	auto _sset = sae::cgen::strenum_cmdline(_args);

	std::ofstream _ofstr{ _sset.output_file };
	std::ifstream _ifstr{ _sset.input_file };
	sae::cgen::generate_strenum(_ifstr, _ofstr, _sset);

	return 0;
};
