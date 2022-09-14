#pragma once

#include <iostream>
#include <qqml.h>

#include "path_registry.hpp"
#include "settings.hpp"

class Stdout : public QObject {
	Q_OBJECT
	QML_ELEMENT
	QML_SINGLETON
public:
	Q_INVOKABLE void print_urls(const QList<QUrl> &urls);
};
