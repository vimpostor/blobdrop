#pragma once

#include "Util/util.hpp"

class Settings {
public:
	enum class AutoQuitBehavior {
		Never,
		First,
		All,
	};

	SINGLETON(Settings)
	AutoQuitBehavior auto_quit_behavior = Settings::AutoQuitBehavior::All;
};
