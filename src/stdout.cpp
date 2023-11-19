#include "stdout.hpp"

void Stdout::print_osc8_link(const std::string &url, const std::string &text) {
	std::cout << std::format("\e]8;;{}\e\\{}\e]8;;\e\\", url, text) << std::endl;
}
