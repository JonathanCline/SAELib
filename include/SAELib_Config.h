#pragma once
#ifndef SAELIB_CONFIG_H
#define SAELIB_CONFIG_H

#ifndef NDEBUG
#define SAELIB_DEBUG true
#else
#define SAELIB_DEBUG false
#endif

namespace sae
{
	namespace config
	{
#if SAELIB_DEBUG
		constexpr static bool debug_v = true;
#else
		constexpr static bool debug_v = false;
#endif

		constexpr static bool no_exceptions_v = false;

	};


};

#endif
