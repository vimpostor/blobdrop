#include "stdin.hpp"

#include "backend.hpp"

Stdin::Stdin(QObject *parent) {
	stdin_nb = fileno(stdin);
	// do not buffer stdin line-wise
	disable_canonical_mode();

	this->socket = std::make_unique<QSocketNotifier>(stdin_nb, QSocketNotifier::Read);
	connect(socket.get(), &QSocketNotifier::activated, this, &Stdin::read);
}

Stdin::~Stdin() {
	reset_terminal_mode();
}

void Stdin::disable_canonical_mode() {
#ifdef Q_OS_UNIX
	struct termios term;

	if (tcgetattr(stdin_nb, &orig_term)) {
		return;
	}
	term = orig_term;

	// unset canonical mode bit
	term.c_lflag &= ~ICANON;
	// echo input characters
	term.c_lflag |= ECHO;
	// Required so that std::cin.get() always returns at minimum one character
	// This should never block the main thread falsely,
	// because we only invoke a read() if we got an explicit socket activated event,
	// so data should already be waiting for us to read
	term.c_cc[VMIN] = 1;
	// No timeout, because we only read data if it is already available
	term.c_cc[VTIME] = 0;

	// change the mode immediately
	std::ignore = tcsetattr(stdin_nb, TCSANOW, &term);
	reset_term = true;
#endif
}

void Stdin::reset_terminal_mode() {
#ifdef Q_OS_UNIX
	if (reset_term) {
		// reset the terminal mode, because it is not reset automatically
		std::ignore = tcsetattr(stdin_nb, TCSANOW, &orig_term);
	}
#endif
}

void Stdin::read() {
	char c;
	bool line_complete = false;

	// we need to do low-level parsing, because we want to react to any ESC pressed right away
	if (std::cin.good()) {
		c = std::cin.get();
		if (c == '\n' || c == '\r' || c == std::istream::traits_type::eof()) {
			line_complete = true;
		} else if (c == 0x1B) {
			// ESC pressed, abort
			Backend::get()->quit_delayed(0ms, true);
		} else {
			current_line += c;
		}
	}

	if (!std::cin.good()) {
		socket->setEnabled(false);
		setClosed(true);
	} else if (!current_line.empty() && line_complete) {
		setClosed(false);
		PathRegistry::get()->add_path(current_line);
		current_line.clear();
	}
}

void Stdin::setClosed(bool closed) {
	if (closed == m_closed) {
		return;
	}
	m_closed = closed;
	emit closedChanged();
}
