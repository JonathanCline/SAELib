#pragma once
#ifndef SAELIB_OS_H
#define SAELIB_OS_H

/*
	Copyright 2021 Jonathan Cline
	Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files
	(the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge,
	publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do
	so, subject to the following conditions:
	The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
	WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
	COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
	OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
	Operating system macro will be set to true if it is detected as the current target

	Usage : 

		#if SAELIB_OS_WINDOWS

			// Windows specific code here

		#elif SAELIB_OS_LINUX

			// Linux specific code here

		#endif


	List of operating systems with detection code:
 
		SAELIB_OS_WINDOWS
		SAELIB_OS_APPLE
		SAELIB_OS_IPHONE_SIM
		SAELIB_OS_IPHONE
		SAELIB_OS_MAC
		SAELIB_OS_LINUX
		SAELIB_OS_UNIX
		SAELIB_OS_POSIX

*/

#define SAELIB_OS_WINDOWS false

#define SAELIB_OS_IPHONE_SIM false
#define SAELIB_OS_IPHONE false
#define SAELIB_OS_MAC false

#define SAELIB_OS_LINUX false
#define SAELIB_OS_UNIX false
#define SAELIB_OS_POSIX false

namespace sae
{
	enum OPERATING_SYSTEM_E
	{
		osWindows,

		osLinux,
		osUnix,
		osPosix,

		osMac,
		osIPhone,
		osIPhoneSim,
		osLinux
	};
}

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#define SAELIB_OS_WINDOWS true
namespace sae
{
	constexpr static OPERATING_SYSTEM_E target_os = OPERATING_SYSTEM_E::osWindows;
}
#elif __APPLE__

#include <TargetConditionals.h>



#if TARGET_IPHONE_SIMULATOR
#define SAELIB_OS_IPHONE_SIM true
namespace sae
{
	constexpr static OPERATING_SYSTEM_E target_os = OPERATING_SYSTEM_E::osIPhoneSim;
}
#elif TARGET_OS_IPHONE
#define SAELIB_OS_IPHONE true
namespace sae
{
	constexpr static OPERATING_SYSTEM_E target_os = OPERATING_SYSTEM_E::osIPhone;
}
#elif TARGET_OS_MAC
#define SAELIB_OS_MAC true
namespace sae
{
	constexpr static OPERATING_SYSTEM_E target_os = OPERATING_SYSTEM_E::osMac;
}
#else
#error "Unknown Apple platform"
#endif

#elif __linux__

#define SAELIB_OS_LINUX true
namespace sae
{
	constexpr static OPERATING_SYSTEM_E target_os = OPERATING_SYSTEM_E::osLinux;
}
#elif __unix__

#define SAELIB_OS_UNIX true
namespace sae
{
	constexpr static OPERATING_SYSTEM_E target_os = OPERATING_SYSTEM_E::osUnix;
}
#elif defined(_POSIX_VERSION)

#define SAELIB_OS_POSIX true
namespace sae
{
	constexpr static OPERATING_SYSTEM_E target_os = OPERATING_SYSTEM_E::osPosix;
};

#else
# error "Unknown compiler target OS"
#endif


#endif