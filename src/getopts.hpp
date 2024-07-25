#pragma once

#include <QCommandLineParser>
#include <iostream>
#include <quartz/getopts.hpp>

#include "path_registry.hpp"
#include "settings.hpp"

namespace Getopts {

QStringList setup_args(int argc, char *argv[]);
bool parse(const QStringList &args);
}
