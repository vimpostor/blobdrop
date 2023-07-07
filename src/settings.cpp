#include "settings.hpp"

#include <QJSEngine>

Settings *Settings::create(QQmlEngine *qmlEngine, QJSEngine *jsEngine) {
	auto res = Settings::get();
	QJSEngine::setObjectOwnership(res, QJSEngine::CppOwnership);
	return res;
}

void Settings::prepare_quit() {
	is_about_to_quit = true;
}

void Settings::setAlwaysOnBottom(const bool v) {
	if (!is_about_to_quit) {
		always_on_bottom = v;
		emit alwaysOnBottomChanged(always_on_bottom);
	}
}
