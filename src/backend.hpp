#pragma once

#include <QCoreApplication>
#include <QDrag>
#include <QPoint>
#include <QTimer>
#include <chrono>

#include "Util/util.hpp"
#include "path.hpp"

#if !defined(Q_OS_WIN) && !defined(Q_OS_DARWIN)
#include "xcb.hpp"
#endif

class Backend : public QObject {
	Q_OBJECT
	QML_ELEMENT
	QML_SINGLETON
public:
	QML_CPP_SINGLETON(Backend)

	void quit_delayed(const std::chrono::milliseconds delay = 100ms);
	void drag_paths(const std::vector<Path> &paths);
	void print_hyperlinks(const std::vector<Path> &paths);
	void send_drag_notification(const std::vector<Path> &uris);

	void hide_terminal();
	void restore_terminal();

	void exec_frontend(const std::vector<Path> &paths);
	Q_INVOKABLE QPoint get_mouse_pos() const;
	Q_INVOKABLE void handle_dropped_urls(const QList<QUrl> &urls);
signals:
	void drag_finished();
private:
	QDrag *drag = nullptr;
#if !defined(Q_OS_WIN) && !defined(Q_OS_DARWIN)
	Xcb xcb;
	xcb_window_t last_window = 0;
#endif
};
