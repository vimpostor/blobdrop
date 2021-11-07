#include "getopts.hpp"

#include <bits/ranges_algo.h>

namespace Getopts {

bool parse(QCoreApplication& app) {
	QCommandLineParser p;
	p.setApplicationDescription("Quickly drag and drop files from the terminal to applications.");
	p.addHelpOption();
	p.addVersionOption();

	QCommandLineOption auto_quit_opt(QStringList() << "x" << "auto-quit", "Whether to quit after dragging and dropping. 0 = disable, 1 = after first drag, 2 (default) = after all paths have been used", "number");

	p.addOption({auto_quit_opt});
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
	// add all trailing arguments to the path list
	std::ranges::for_each(p.positionalArguments(), [](auto i){ PathRegistry::get()->add_path(i.toStdString()); });
	return true;
}

}
