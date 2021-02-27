#pragma once

#include "SAELib_OS.h"

#if SAELIB_OS_WINDOWS_V

namespace sae
{
	namespace ansii
	{
		constexpr static inline auto NONE = 15;
		constexpr static inline auto BLACK = 0;
		constexpr static inline auto RED = 12;
		constexpr static inline auto GREEN = 10;
		constexpr static inline auto YELLOW = 14;
		constexpr static inline auto BLUE = 9;
		//constexpr static inline auto MAGENTA = "\033[35m";
		//constexpr static inline auto CYAN = "\033[36m";
		constexpr static inline auto WHITE = 15;
		//constexpr static inline auto BOLDBLACK = "\033[1m\033[30m";
		//constexpr static inline auto BOLDRED = "\033[1m\033[31m";
		//constexpr static inline auto BOLDGREEN = "\033[1m\033[32m";
		//constexpr static inline auto BOLDYELLOW = "\033[1m\033[33m";
		//constexpr static inline auto BOLDBLUE = "\033[1m\033[34m";
		//constexpr static inline auto BOLDMAGENTA = "\033[1m\033[35m";
		//constexpr static inline auto BOLDCYAN = "\033[1m\033[36m";
		//constexpr static inline auto BOLDWHITE = "\033[1m\033[37m";

		static inline void set_text_color(std::ostream& _ostr, int _col)
		{
			static inline HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, _col);
		};

	};
};

#elif SAELIB_OS_LINUX_V || SAELIB_OS_UNIX_V || SAELIB_OS_POSIX_V

namespace sae
{
	namespace ansii
	{

		constexpr static inline auto NONE = "\033[0m";
		constexpr static inline auto BLACK = "\033[30m";      /* Black */
		constexpr static inline auto RED = "\033[31m";      /* Red */
		constexpr static inline auto GREEN = "\033[32m";      /* Green */
		constexpr static inline auto YELLOW = "\033[33m";      /* Yellow */
		constexpr static inline auto BLUE = "\033[34m";      /* Blue */
		//constexpr static inline auto MAGENTA = "\033[35m";      /* Magenta */
		//constexpr static inline auto CYAN = "\033[36m";      /* Cyan */
		constexpr static inline auto WHITE = "\033[37m";      /* White */
		//constexpr static inline auto BOLDBLACK = "\033[1m\033[30m";      /* Bold Black */
		//constexpr static inline auto BOLDRED = "\033[1m\033[31m";      /* Bold Red */
		//constexpr static inline auto BOLDGREEN = "\033[1m\033[32m";      /* Bold Green */
		//constexpr static inline auto BOLDYELLOW = "\033[1m\033[33m";      /* Bold Yellow */
		//constexpr static inline auto BOLDBLUE = "\033[1m\033[34m";      /* Bold Blue */
		//constexpr static inline auto BOLDMAGENTA = "\033[1m\033[35m";      /* Bold Magenta */
		//constexpr static inline auto BOLDCYAN = "\033[1m\033[36m";      /* Bold Cyan */
		//constexpr static inline auto BOLDWHITE = "\033[1m\033[37m";      /* Bold White */

		void set_text_color(std::ostream& _ostr, const char* _col)
		{
			_ostr << _col;
		};

	};

}

#else

namespace sae
{
	namespace ansii
	{
		void set_text_color(std::ostream& _ostr, const char* _col)
		{};
	}
}

#endif

namespace sae
{

	struct color
	{
	public:
		enum color_e
		{
			RED,
			BLUE,
			WHITE,
			BLACK,
			YELLOW,
			GREEN,
			NONE
		};

	private:
		template <color_e Col>
		struct color_t
		{
			friend inline std::ostream& operator<<(std::ostream& _ostr, const color_t<Col>& _col)
			{
				if constexpr (Col == color_e::NONE)
				{
					ansii::set_text_color(_ostr, ansii::NONE);
				}
				else if constexpr (Col == color_e::RED)
				{
					ansii::set_text_color(_ostr, ansii::RED);
				}
				else if constexpr (Col == color_e::BLUE)
				{
					ansii::set_text_color(_ostr, ansii::BLUE);
				}
				else if constexpr (Col == color_e::GREEN)
				{
					ansii::set_text_color(_ostr, ansii::GREEN);
				}
				else if constexpr (Col == color_e::WHITE)
				{
					ansii::set_text_color(_ostr, ansii::WHITE);
				}
				else if constexpr (Col == color_e::BLACK)
				{
					ansii::set_text_color(_ostr, ansii::BLACK);
				}
				else if constexpr (Col == color_e::YELLOW)
				{
					ansii::set_text_color(_ostr, ansii::YELLOW);
				}
				else
				{
					static_assert(false);
				};

				return _ostr;
			};
		};

	public:
		static inline constexpr auto none = color_t<color_e::NONE>{};
		static inline constexpr auto red = color_t<color_e::RED>{};
		static inline constexpr auto blue = color_t<color_e::BLUE>{};
		static inline constexpr auto white = color_t<color_e::WHITE>{};
		static inline constexpr auto black = color_t<color_e::BLACK>{};
		static inline constexpr auto yellow = color_t<color_e::YELLOW>{};
		static inline constexpr auto green = color_t<color_e::GREEN>{};

	};

};

#endif