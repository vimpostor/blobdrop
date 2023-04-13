#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

#include "getopts.hpp"
#include "version.hpp"

int main(int argc, char *argv[]) {
	QCoreApplication::setOrganizationName("blobdrop");
	QCoreApplication::setApplicationName("blobdrop");
	QCoreApplication::setApplicationVersion(Version::version_string());
	QGuiApplication app(argc, argv);
	if (!Getopts::parse(app)) {
		return EXIT_FAILURE;
	}

	QGuiApplication::setWindowIcon(QIcon::fromTheme("blobdrop", QIcon(":/blobdrop")));

	QQmlApplicationEngine engine;
	engine.addImportPath(QStringLiteral(":/"));

	engine.load(QUrl(QLatin1String("qrc:/Backend/src/qml/main.qml")));
	if (engine.rootObjects().isEmpty()) {
		return EXIT_FAILURE;
	}

	return QGuiApplication::exec();
}
