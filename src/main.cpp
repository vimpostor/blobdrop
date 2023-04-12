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
	QQuickStyle::setStyle(QLatin1String("Material"));
	constexpr const char *materialVariantName = "QT_QUICK_CONTROLS_MATERIAL_VARIANT";
	if (!qEnvironmentVariableIsSet(materialVariantName)) {
		// dense Material style, more suited for desktop targets
		qputenv(materialVariantName, "Dense");
	}
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
