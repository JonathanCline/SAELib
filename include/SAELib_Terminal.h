#pragma once
#ifndef SAELIB_TERMINAL_H
#define SAELIB_TERMINAL_H

#include "SAELib_Token.h"
#include "SAELib_CCommand.h"
#include "SAELib_Thread.h"
#include "SAELib_Stream.h"
#include "SAELib_ANSII.h"

#include <istream>
#include <ostream>
#include <sstream>

namespace sae
{
	
	struct terminal
	{
	public:
		using command = void(*)(terminal* _terminal, const token_list& _args);
		using command_set = basic_command_set<command>;

	private:
		std::atomic_bool keep_open_ = true;

	public:
		std::atomic_bool echo = false;
		
		void set_close_flag()
		{
			this->keep_open_ = false;
		};
		bool keep_open() const
		{
			return this->keep_open_;
		};

		void terminal_main()
		{
			redirect_standard_io(this->rin, this->rout);

			std::string _uinput{};
			while (this->thread_.is_running() && this->keep_open())
			{
				cout << color::green << "$ " << color::none;
				std::getline(cin, _uinput, '\n');

				auto _tkns = split_not_empty(_uinput, ' ');

				if (this->echo)
				{
					for (auto& t : _tkns) { cout << "  " << t << '\n'; };
				};

				auto _cmds = this->commands_.acquire();
				if (!_tkns.empty())
				{
					auto _result = _cmds->invoke(std::string{ _tkns.front() }, this, _tkns);
					if (_result)
					{
						cout << '\n';
						continue;
					};
				};
				
				cout << color::red << "Unrecognized command \"" << _uinput << "\"\n\n";



			};

			cout << "Exiting terminal...\n";

			// Reset standard io
			std::cout.rdbuf(cout.rdbuf());
			std::cin.rdbuf(cin.rdbuf());

		};

		void insert(const std::string& _name, const command& _cmd)
		{
			this->commands_.lock();
			this->commands_->insert(_name, _cmd);
			this->commands_.unlock();
		};

		terminal() :
			thread_{ &terminal::terminal_main, this }
		{};
		
	private:
		resource_guard<command_set> commands_{};
		ithread thread_{};

		std::istringstream rin{};
		std::ostringstream rout{};

	};





};

#endif