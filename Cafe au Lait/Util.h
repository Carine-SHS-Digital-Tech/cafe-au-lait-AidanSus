#pragma once

enum con_colour : WORD
{
	fg_red = FOREGROUND_RED,
	fg_green = FOREGROUND_GREEN,
	fg_blue = FOREGROUND_BLUE,
	fg_white = 7,
};

namespace util
{

	std::string to_lower(std::string & input)
	{

		std::string lower_string{ "" };
		
		for (int i{ 0 }; i < input.size(); ++i)
			lower_string += tolower(input[i]);
		
		return lower_string;

	}

	std::string get_input()
	{
		std::string input_string{ "" };
		std::getline(std::cin, input_string);

		return input_string;
	}

	std::string get_lower_input()
	{
		std::string input = get_input();
		
		return to_lower(input);
	}

	const void change_colour(con_colour colour)
	{

		HANDLE output_handle = GetStdHandle(STD_OUTPUT_HANDLE);

		if (output_handle != INVALID_HANDLE_VALUE && output_handle != nullptr)
			SetConsoleTextAttribute(output_handle, colour);

	}

}