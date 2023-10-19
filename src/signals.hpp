#pragma once

#include <QSocketNotifier>

class Signals : public QObject {
	Q_OBJECT
public:
	Signals(const std::initializer_list<int> &sigs);

	// Unix signal handler
	static void handle_unix_signal(int);
public slots:
	// Qt signal handler
	void handle_qt_signal();
private:
	void setup_signal_handlers(const std::initializer_list<int> &sigs);

	static inline int signal_fd[2];
	QSocketNotifier *sn;
};
