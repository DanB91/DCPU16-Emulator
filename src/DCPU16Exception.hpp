/*
 * This object inherits from the excpetion class and allows you to put a
 * custom message into the expcetion
 */

#pragma once

#include <exception>
#include <string>


class DCPU16Exception : public std::exception
{

	public:
		DCPU16Exception(const std::string &str) : msg(str)
		{
		}

		virtual const char *what() const throw()
		{
			return msg.c_str();
		}

		virtual ~DCPU16Exception() throw()
		{
		}

	private:
		const std::string msg;

};

