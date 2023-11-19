#pragma once

#include <QSocketNotifier>
#include <iostream>
#include <qqml.h>

#include "path_registry.hpp"

#ifdef Q_OS_UNIX
#include <termios.h>
#endif

class Stdin : public QObject {
	Q_OBJECT
	QML_ELEMENT
	QML_SINGLETON
	Q_PROPERTY(bool closed MEMBER m_closed NOTIFY closedChanged)
public:
	explicit Stdin(bool);
	~Stdin();

	static Stdin *get();
	static Stdin *create(QQmlEngine *qmlEngine, QJSEngine *jsEngine);

	bool is_tty = true;
signals:
	void closedChanged();
private:
	void disable_canonical_mode();
	void reset_terminal_mode();
	void read();
	bool m_closed = true;
	void setClosed(bool closed);
	std::unique_ptr<QSocketNotifier> socket;
	int stdin_nb = -1;
	std::string current_line;
#ifdef Q_OS_UNIX
	bool reset_term = false;
	struct termios orig_term;
#endif
};
