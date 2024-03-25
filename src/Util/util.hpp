#pragma once

#include <QJSEngine>
#include <filesystem>
#include <qglobal.h>
#include <qqml.h>
#include <quartz/macros.hpp>
#include <quartz/util.hpp>
#include <string>

using namespace std::chrono_literals;

namespace Util {

const char *home_dir();
std::string pwd();
bool get_local_domain(std::string &result);
std::string get_username();
int get_port();
}
