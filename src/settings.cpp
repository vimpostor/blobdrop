#include "settings.hpp"

#include <QJSEngine>

Settings *Settings::create(QQmlEngine *qmlEngine, QJSEngine *jsEngine) {
	auto res = Settings::get();
	QJSEngine::setObjectOwnership(res, QJSEngine::CppOwnership);
	return res;
}
