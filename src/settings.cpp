#include "settings.hpp"

void Settings::setAlwaysOnBottom(const bool v) {
	if (!suppress_always_on_bottom) {
		always_on_bottom = v;
		emit alwaysOnBottomChanged(always_on_bottom);
	}
}

Settings::Frontend Settings::effective_frontend(bool outgoing) const {
	if (intercept && !outgoing) {
		return Settings::Frontend::Gui;
	} else if (frontend == Settings::Frontend::Auto) {
		if (intercept) {
			// for intercept and outgoing prefer Immediate as default frontend
			return Util::is_wayland() ? Settings::Frontend::Stdout : Settings::Frontend::Immediate;
		} else {
			// in general (not outgoing), prefer immediate and fallback to GUI
			return can_drag_immediately ? Settings::Frontend::Immediate : Settings::Frontend::Gui;
		}
	}
	return frontend;
}

bool Settings::needs_gui() const {
	// Since the corresponding QML property is marked as constant,
	// it does not matter if this changes during runtime:
	// QML won't get the updated value.
	// This means we will never have the problem, where e.g.
	// file names are passed over stdin with a delay,
	// so initially the auto frontend would show a GUI (because no filenames are ready),
	// but then filenames come trickling in on stdin while the GUI is already showing
	// so this boolean would change to false for the Auto frontend,
	// meaning the GUI could hide again.
	// Of course in that case the GUI should stay visible if it was visible once.
	return effective_frontend() == Settings::Frontend::Gui && !hide_gui;
}

void Settings::setHideGui(const bool h) {
	hide_gui = h;
	emit hideGuiChanged(h);
}
