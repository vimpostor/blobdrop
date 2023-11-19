#pragma once

#include <QJSEngine>
#include <filesystem>
#include <qglobal.h>
#include <qqml.h>
#include <string>

using namespace std::chrono_literals;

#define SINGLETON(TYPE) \
	static TYPE *get() { \
		static TYPE s; \
		return &s; \
	}

#define QML_CPP_SINGLETON(TYPE) \
	TYPE() = delete; \
	explicit TYPE(bool singleton_constructor) {}; \
	static TYPE *get() { \
		static TYPE s {true}; \
		return &s; \
	} \
	static TYPE *create(QQmlEngine *qmlEngine, QJSEngine *jsEngine) { \
		auto res = get(); \
		QJSEngine::setObjectOwnership(res, QJSEngine::CppOwnership); \
		return res; \
	}

namespace Util {

const char *home_dir();
std::string pwd();
bool is_wayland();
}
