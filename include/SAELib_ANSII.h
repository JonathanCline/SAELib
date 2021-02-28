#pragma once
#ifndef SAELIB_ANSII_H
#define SAELIB_ANSII_H

#include <ostream>



namespace sae
{
	namespace ansii::win
	{
		constexpr static auto NONE = 15;
		constexpr static auto BLACK = 0;
		constexpr static auto RED = 12;
		constexpr static auto GREEN = 2;
		constexpr static auto YELLOW = 14;
		constexpr static auto BLUE = 9;
		constexpr static auto MAGENTA = 5;
		constexpr static auto CYAN = 3;
		constexpr static auto WHITE = 15;

		constexpr static std::array<int, 9> ANSII_COLORS_V
		{
			NONE, BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE
		};

#ifdef _WINDOWS_
		static inline void set_text_color(std::ostream& _ostr, int _col)
		{
			static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, _col);
		};
#endif

	};

	namespace ansii::unx
	{

		constexpr static auto NONE = "\033[0m";
		constexpr static auto BLACK = "\033[30m";      /* Black */
		constexpr static auto RED = "\033[31m";      /* Red */
		constexpr static auto GREEN = "\033[32m";      /* Green */
		constexpr static auto YELLOW = "\033[33m";      /* Yellow */
		constexpr static auto BLUE = "\033[34m";      /* Blue */
		constexpr static auto MAGENTA = "\033[35m";      /* Magenta */
		constexpr static auto CYAN = "\033[36m";      /* Cyan */
		constexpr static auto WHITE = "\033[37m";      /* White */

		constexpr static std::array<const char*, 9> ANSII_COLORS_V
		{
			NONE, BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE
		};

		static inline void set_text_color(std::ostream& _ostr, const char* _col)
		{
			_ostr << _col;
		};

	};

#ifdef _WINDOWS_
	namespace ansii
	{
		using namespace win;
	};
#else 
	namespace ansii
	{
		using namespace unx;
	};
#endif

}

namespace sae
{

	struct color
	{
	public:
		enum class color_e
		{
			NONE = 0,
			BLACK,
			RED,
			GREEN,
			YELLOW,
			BLUE,
			MAGENTA,
			CYAN,
			WHITE
		};

		template <color_e Col>
		struct color_t
		{
			friend inline std::ostream& operator<<(std::ostream& _ostr, const color_t<Col>& _col)
			{
				ansii::set_text_color(_ostr, ansii::ANSII_COLORS_V.at((size_t)Col));
				return _ostr;
			};
		};

		static constexpr auto none = color_t<color_e::NONE>{};
		static constexpr auto black = color_t<color_e::BLACK>{};
		static constexpr auto red = color_t<color_e::RED>{};
		static constexpr auto green = color_t<color_e::GREEN>{};
		static constexpr auto yellow = color_t<color_e::YELLOW>{};
		static constexpr auto blue = color_t<color_e::BLUE>{};
		static constexpr auto magenta = color_t<color_e::MAGENTA>{};
		static constexpr auto cyan = color_t<color_e::CYAN>{};
		static constexpr auto white = color_t<color_e::WHITE>{};

	};

	static inline std::ostream& operator<<(std::ostream& _ostr, color::color_e _col)
	{
		ansii::set_text_color(_ostr, ansii::ANSII_COLORS_V.at((size_t)_col));
		return _ostr;
	};


	/*
		DO NOT CHANGE THE BELOW CODE UNLESS YOU ARE ADDING MORE CHECKS
	*/

	static_assert((int)color::color_e::NONE == 0);
	static_assert((int)color::color_e::BLACK == 1);
	static_assert((int)color::color_e::RED == 2);
	static_assert((int)color::color_e::GREEN == 3);
	static_assert((int)color::color_e::YELLOW == 4);
	static_assert((int)color::color_e::BLUE == 5);
	static_assert((int)color::color_e::MAGENTA == 6);
	static_assert((int)color::color_e::CYAN == 7);
	static_assert((int)color::color_e::WHITE == 8);

};

#endif