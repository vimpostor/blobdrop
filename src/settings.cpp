#include "settings.hpp"

#include <QJSEngine>

Settings *Settings::create(QQmlEngine *qmlEngine, QJSEngine *jsEngine) {
	auto res = Settings::get();
	QJSEngine::setObjectOwnership(res, QJSEngine::CppOwnership);
	return res;
}

void Settings::disable_always_on_bottom() {
	supress_always_on_bottom = true;
}

void Settings::setAlwaysOnBottom(const bool v) {
	if (!supress_always_on_bottom) {
		always_on_bottom = v;
		emit alwaysOnBottomChanged(always_on_bottom);
	}
}

bool Settings::needs_gui() const {
	return frontend == Settings::Frontend::Gui || (frontend == Settings::Frontend::Auto && !hide_gui_flag);
}
