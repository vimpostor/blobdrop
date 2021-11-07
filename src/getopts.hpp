#pragma once

#include <iostream>
#include <QCommandLineParser>

#include "path_registry.hpp"
#include "settings.hpp"

namespace Getopts {
	bool parse(QCoreApplication& app);
}
