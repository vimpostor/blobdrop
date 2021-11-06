#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QIcon>
#include <QQmlContext>

#include "getopts.hpp"

int main(int argc, char* argv[])
{
	QCoreApplication::setOrganizationName("blobdrop");
	QCoreApplication::setApplicationName("blobdrop");
	QQuickStyle::setStyle(QLatin1String("Material"));
	constexpr const char* materialVariantName = "QT_QUICK_CONTROLS_MATERIAL_VARIANT";
	if (!qEnvironmentVariableIsSet(materialVariantName)) {
		// dense Material style, more suited for desktop targets
		qputenv(materialVariantName, "Dense");
	}
	QGuiApplication app(argc, argv);
	Getopts::parse(app);

	QGuiApplication::setWindowIcon(QIcon::fromTheme("blobdrop", QIcon(":/blobdrop")));

	QQmlApplicationEngine engine;
	engine.addImportPath(QStringLiteral(":/"));

	engine.load(QUrl(QLatin1String("qrc:/Backend/src/qml/main.qml")));
	if (engine.rootObjects().isEmpty())
		return -1;
	return QGuiApplication::exec();
}
