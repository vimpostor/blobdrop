#include "stdin.hpp"

Stdin::Stdin(QObject* parent) {
	socket = std::make_unique<QSocketNotifier>(fileno(stdin), QSocketNotifier::Read);
	connect(socket.get(), &QSocketNotifier::activated, this, &Stdin::read);
}

void Stdin::read() {
	std::string line;
	std::getline(std::cin, line);
	if (std::cin.eof()) {
		socket->setEnabled(false);
		setClosed(true);
	} else if (!line.empty()) {
		setClosed(false);
		PathRegistry::get()->add_path(line);
	}
}

void Stdin::setClosed(bool closed) {
	if (closed == m_closed) {
		return;
	}
	m_closed = closed;
	emit closedChanged();
}
