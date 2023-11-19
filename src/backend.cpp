#include "backend.hpp"

#include <QCursor>
#include <QDBusInterface>
#include <QGuiApplication>
#include <QIcon>
#include <QMimeData>
#include <QPixmap>

#include "path_registry.hpp"
#include "settings.hpp"
#include "stdout.hpp"

void Backend::quit_delayed(const std::chrono::milliseconds delay) {
	// cancel any pending drags
	if (drag) {
		drag->cancel();
	}
	// remove the possibly set keep-below hint, as we want to quit now
	Backend::get()->restore_terminal();

	Settings::get()->suppress_always_on_bottom = true;
	QTimer::singleShot(delay, qGuiApp, QGuiApplication::quit);
}

void Backend::drag_paths(const std::vector<Path> &paths) {
	hide_terminal();

	/**
	 * Qt takes ownership both over the QDrag as well as the QMimeData
	 * So no, this is not a memory leak.
	 */
	drag = new QDrag(this);
	auto mimedata = new QMimeData();

	QList<QUrl> urls;
	for (auto &i : paths) {
		urls.push_back(i.get_url());
	}

	mimedata->setUrls(urls);
	drag->setMimeData(mimedata);

	constexpr const int cursor_size = 24;
	QPixmap pixmap;
	if (paths.size() == 1) {
		const auto p = paths.front();
		if (!p.thumbnail.isEmpty()) {
			// try using the thumbnail first
			constexpr const int max_size = 128;
			pixmap = QPixmap(p.thumbnail.toLocalFile());
			if (std::max(pixmap.width(), pixmap.height()) > max_size) {
				pixmap = pixmap.scaled(QSize(max_size, max_size), Qt::KeepAspectRatio, Qt::SmoothTransformation);
			}
		}
		if (pixmap.isNull()) {
			// fallback to mime type icon
			pixmap = QIcon::fromTheme(QString::fromStdString(p.iconName)).pixmap(cursor_size);
		}
	} else {
		// show a collective pseudo thumbnail of all files
		pixmap = QIcon::fromTheme("emblem-documents").pixmap(cursor_size);
	}
	if (!pixmap.isNull()) {
		drag->setPixmap(pixmap);
	}

	// The object is destroyed by Qt as soon as the drag is finished
	connect(drag, &QObject::destroyed, this, [this]() {
		restore_terminal();
		emit drag_finished();
		drag = nullptr;
	});

	std::ignore = drag->exec();
}

void Backend::print_hyperlinks(const std::vector<Path> &paths) {
	for (auto &i : paths) {
		Stdout::print_osc8_link(i.get_uri(), i.pretty_print());
	}
}

void Backend::send_drag_notification(const std::vector<Path> &uris) {
#if !defined(Q_OS_WIN) && !defined(Q_OS_DARWIN)
	if (uris.empty()) {
		return;
	}

	auto session = QDBusConnection::sessionBus();
	auto msg = QDBusMessage::createMethodCall("org.freedesktop.Notifications", "/org/freedesktop/Notifications", "org.freedesktop.Notifications", "Notify");

	QList<QString> urls;
	for (const auto &u : uris) {
		urls.push_back(QString::fromStdString(u.get_uri()));
	}

	QVariantMap hints;
	hints["x-kde-urls"] = urls;
	QString title = "Drag file";
	if (uris.size() > 1) {
		title += "s";
	}
	msg.setArguments({"blobdrop", 0U, "cursor-arrow", title, "", QStringList(), hints, 5000});
	session.call(msg);
#endif
}

void Backend::hide_terminal() {
#if !defined(Q_OS_WIN) && !defined(Q_OS_DARWIN)
	if (Settings::get()->suppress_always_on_bottom || !xcb.init()) {
		return;
	}

	last_window = xcb.active_window();
	if (last_window) {
		xcb.set_keep_window_below(last_window);
	}
#endif
}

void Backend::restore_terminal() {
#if !defined(Q_OS_WIN) && !defined(Q_OS_DARWIN)
	if (!last_window) {
		return;
	}

	xcb.set_keep_window_below(last_window, false);
	last_window = 0;
#endif
}

void Backend::exec_frontend(const std::vector<Path> &paths) {
	const auto f = Settings::get()->effective_frontend(true);
	if (f == Settings::Frontend::Immediate) {
		Backend::get()->drag_paths(paths);
	} else if (f == Settings::Frontend::Stdout) {
		Backend::get()->print_hyperlinks(paths);
	} else if (f == Settings::Frontend::Notification) {
		Backend::get()->send_drag_notification(paths);
	}
}

QPoint Backend::get_mouse_pos() const {
	return QCursor::pos();
}

void Backend::handle_dropped_urls(const QList<QUrl> &urls) {
	std::vector<Path> paths;
	for (auto &u : urls) {
		auto url = u.toString().toStdString();
		if (url.starts_with("file://")) {
			url = url.substr(7);
		}
		paths.push_back(url);
	}

	if (Settings::get()->keep_dropped_files) {
		for (auto &i : paths) {
			PathRegistry::get()->add_path(i);
		}
	}

	if (Settings::get()->intercept) {
		QTimer::singleShot(0, qGuiApp, [this, paths]() { exec_frontend(paths); });
	} else {
		Backend::get()->print_hyperlinks(paths);
	}

	if (Settings::get()->auto_quit_behavior == Settings::AutoQuitBehavior::First) {
		Backend::get()->quit_delayed();
	}
}
