#include "stdout.hpp"

void Stdout::print_urls(const QList<QUrl>& urls) {
	for (const auto& u : urls) {
		auto url = u.toString().toStdString();
		if (url.starts_with("file://")) {
			url = url.substr(7);
		}
		std::cout << url << std::endl;
	}
}
