#include "backend.hpp"

#include <QDBusInterface>

#include "settings.hpp"

void Backend::quit_delayed() {
	Settings::get()->disable_always_on_bottom();
	QTimer::singleShot(100, []() { QCoreApplication::quit(); });
}

void Backend::send_drag_notification(const QList<QString> &uris) {
	auto session = QDBusConnection::sessionBus();
	auto msg = QDBusMessage::createMethodCall("org.freedesktop.Notifications", "/org/freedesktop/Notifications", "org.freedesktop.Notifications", "Notify");
	QVariantMap hints;
	hints["x-kde-urls"] = uris;
	msg.setArguments({"blobdrop", 0U, "cursor-arrow", "Drag files", "", QStringList(), hints, 5000});
	session.call(msg);
}
