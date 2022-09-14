#pragma once

#include <QCommandLineParser>
#include <iostream>

#include "path_registry.hpp"
#include "settings.hpp"

namespace Getopts {
bool parse(QCoreApplication &app);
}
