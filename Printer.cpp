#include "Printer.h"

std::mutex Printer::_mutex_print;

Printer::~Printer() {
  std::lock_guard<std::mutex> guard(_mutex_print);
  std::cout << this->str();
}