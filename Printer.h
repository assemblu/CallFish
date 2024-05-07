#pragma once

#include <iostream>
#include <sstream>
#include <mutex>

class Printer : public std::ostringstream
{
public:
	Printer() = default;
	~Printer();

private:
	static std::mutex _mutex_print;
};

#if 0
class Printer: public std::ostringstream
{
public:
	Printer() = default;
	~Printer() {
		std::lock_guard<std::mutex> guard(_mutex_print);
		std::cout << this->str();
	}

private:
	static std::mutex _mutex_print;

};

std::mutex Printer::_mutex_print{};
#endif
