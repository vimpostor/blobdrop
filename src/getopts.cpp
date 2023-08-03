#include "getopts.hpp"
#include "settings.hpp"

#include <bits/ranges_algo.h>

namespace Getopts {

bool parse(QCoreApplication &app) {
	QCommandLineParser p;
	p.setApplicationDescription("Quickly drag and drop files from the terminal to applications.");
	p.addHelpOption();
	p.addVersionOption();

	QCommandLineOption frameless_opt(QStringList() << "f"
												   << "frameless",
		"Show a frameless window.");
	QCommandLineOption keep_opt(QStringList() << "k"
											  << "keep",
		"Keep dropped files around in sink mode.");
	QCommandLineOption link_opt(QStringList() << "l"
											  << "link",
		"Print paths to stdout via OSC8 hyperlinks.");
	QCommandLineOption notify_opt(QStringList() << "n"
												<< "notification",
		"Send a notification for dragging.");
	QCommandLineOption persistent_opt(QStringList() << "p"
													<< "persistent",
		"Do not auto-hide the main window while dragging.");
	QCommandLineOption ontop_opt(QStringList() << "t"
											   << "ontop",
		"Keep the window on top of other windows.");
	QCommandLineOption auto_quit_opt(QStringList() << "x"
												   << "auto-quit",
		"Whether to autoquit after a drag is finished. 0 = disable, 1 = after first drag, 2 (default) = after all paths have been used",
		"number");

	p.addOptions({frameless_opt, keep_opt, link_opt, notify_opt, persistent_opt, ontop_opt, auto_quit_opt});
	p.process(app);

	if (p.isSet(auto_quit_opt)) {
		const auto opt = p.value(auto_quit_opt);
		if (std::ranges::all_of(opt.toStdString(), ::isdigit)) {
			if (opt.toInt() > 2) {
				std::cerr << "auto-quit needs to be one of {0,1,2}" << std::endl;
				return false;
			}
			Settings::get()->auto_quit_behavior = static_cast<Settings::AutoQuitBehavior>(opt.toInt());
		} else {
			std::cerr << "auto-quit needs to be a number" << std::endl;
			return false;
		}
	}
	if (p.isSet(ontop_opt)) {
		Settings::get()->always_on_top = true;
	}
	if (p.isSet(keep_opt)) {
		Settings::get()->keep_dropped_files = true;
	}
	if (p.isSet(link_opt)) {
		Settings::get()->print_hyperlinks = true;
	}
	if (p.isSet(notify_opt)) {
		Settings::get()->send_notification = true;
	}
	if (p.isSet(persistent_opt)) {
		Settings::get()->disable_always_on_bottom();
	}
	if (p.isSet(frameless_opt)) {
		Settings::get()->frameless = true;
	}

	// add all trailing arguments to the path list
	std::ranges::for_each(p.positionalArguments(), [](auto i) { PathRegistry::get()->add_path(i.toStdString()); });
	return true;
}

}
