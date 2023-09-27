#include "backend.hpp"

#include <QDBusInterface>

#include "settings.hpp"

void Backend::quit_delayed(const int delay) {
	Settings::get()->disable_always_on_bottom();
	QTimer::singleShot(delay, []() { QCoreApplication::quit(); });
}

void Backend::send_drag_notification(const QList<QString> &uris) {
	auto session = QDBusConnection::sessionBus();
	auto msg = QDBusMessage::createMethodCall("org.freedesktop.Notifications", "/org/freedesktop/Notifications", "org.freedesktop.Notifications", "Notify");
	QVariantMap hints;
	hints["x-kde-urls"] = uris;
	QString title = "Drag file";
	if (uris.size() > 1) {
		title += "s";
	}
	msg.setArguments({"blobdrop", 0U, "cursor-arrow", title, "", QStringList(), hints, 5000});
	session.call(msg);
}
