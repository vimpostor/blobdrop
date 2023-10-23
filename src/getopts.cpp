#include "getopts.hpp"
#include "settings.hpp"

#include <bits/ranges_algo.h>

namespace Getopts {

bool parse(QCoreApplication &app) {
	QCommandLineParser p;
	p.setApplicationDescription("Quickly drag and drop files from the terminal to applications.");
	p.addHelpOption();
	p.addVersionOption();

	QCommandLineOption frameless_opt(QStringList() << "b"
												   << "frameless",
		"Show a frameless window.");
	QCommandLineOption frontend_opt(QStringList() << "f"
												  << "frontend",
		"Selects the frontend. Must be one of {auto (default), gui, immediate, notify, stdout}",
		"frontend");
	QCommandLineOption keep_opt(QStringList() << "k"
											  << "keep",
		"Keep dropped files around in sink mode.");
	QCommandLineOption persistent_opt(QStringList() << "p"
													<< "persistent",
		"Do not auto-hide the main window while dragging.");
	QCommandLineOption ontop_opt(QStringList() << "t"
											   << "ontop",
		"Keep the window on top of other windows.");
	QCommandLineOption auto_quit_opt(QStringList() << "x"
												   << "auto-quit",
		"The amount of drags after which the program should automatically close. Must be one of {never, first, all (default)}",
		"number");

	p.addOptions({frameless_opt, frontend_opt, keep_opt, persistent_opt, ontop_opt, auto_quit_opt});
	p.process(app);

	if (p.isSet(auto_quit_opt)) {
		const auto opt = p.value(auto_quit_opt);
		constexpr std::array str_repr = {"never", "first", "all"};
		int choice = std::ranges::find(str_repr, opt.toStdString()) - str_repr.cbegin();
		if (static_cast<Settings::AutoQuitBehavior>(choice) > Settings::AutoQuitBehavior::All) {
			std::cerr << "auto-quit needs to be one of {never,first,all}" << std::endl;
			return false;
		}
		Settings::get()->auto_quit_behavior = static_cast<Settings::AutoQuitBehavior>(choice);
	}
	if (p.isSet(ontop_opt)) {
		Settings::get()->always_on_top = true;
	}
	if (p.isSet(keep_opt)) {
		Settings::get()->keep_dropped_files = true;
	}

	if (p.isSet(frontend_opt)) {
		constexpr std::array str_repr = {"auto", "gui", "immediate", "notify", "stdout"};
		int choice = std::ranges::find(str_repr, p.value(frontend_opt).toStdString()) - str_repr.cbegin();
		if (choice > static_cast<int>(Settings::Frontend::Stdout)) {
			std::cerr << "frontend needs to be one of:" << std::ranges::fold_left(str_repr, std::string(), [](const auto &l, const auto &r) { return l + " " + r; }) << std::endl;
			return false;
		}
		const auto c = static_cast<Settings::Frontend>(choice);
		if (c == Settings::Frontend::Immediate && Util::is_wayland()) {
			std::cerr << "Wayland does not have support for this frontend, as the spec requires an implicit grab for native wl_data_device::start_drag() operations." << std::endl
					  << "Please switch to X11 to get the optimal usability experience." << std::endl;
			return false;
		}
		Settings::get()->frontend = c;
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
