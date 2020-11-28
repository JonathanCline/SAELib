#pragma once

#include <istream>
#include <ostream>
#include <string>
#include <vector>

namespace sae::cgen
{
	struct strenum_settings
	{
		std::string input_file = "";
		std::string output_file = "out.h";
		std::string name = "";
		std::string use_namespace = "";
	};

	void generate_strenum(std::istream& _in, std::ostream& _ostr, const strenum_settings& _settings);
	strenum_settings strenum_cmdline(const std::vector<std::string>& _args);

}
