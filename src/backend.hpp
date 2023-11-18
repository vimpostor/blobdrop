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
	STRICT_SINGLETON(Backend)

	static Backend *create(QQmlEngine *qmlEngine, QJSEngine *jsEngine);
	void quit_delayed(const std::chrono::milliseconds delay = 100ms);
	void drag_paths(const std::vector<Path> &paths);
	void print_hyperlinks(const std::vector<Path> &paths);
	void send_drag_notification(const QList<QString> &uris);

	void hide_terminal();
	void restore_terminal();

	Q_INVOKABLE QPoint get_mouse_pos() const;
signals:
	void drag_finished();
private:
	QDrag *drag = nullptr;
#if !defined(Q_OS_WIN) && !defined(Q_OS_DARWIN)
	Xcb xcb;
	xcb_window_t last_window = 0;
#endif
};
