#include "getopts.hpp"
#include "settings.hpp"

#include <bits/ranges_algo.h>

namespace Getopts {

bool parse(QCoreApplication &app) {
	QCommandLineParser p;
	p.setApplicationDescription("Quickly drag and drop files from the terminal to applications.");
	p.addHelpOption();
	p.addVersionOption();

	constexpr std::array frontend_opts = {"auto", "gui", "immediate", "notify", "stdout"};
	// TODO: Uncomment this, once we move to C++23
	// const std::string frontends_descr = std::ranges::fold_left(frontend_opts, std::string(), [](const auto &l, const auto &r) { return l + " " + r; });
	const std::string frontends_descr = std::accumulate(frontend_opts.cbegin(), frontend_opts.cend(), std::string(), [](const auto &l, const auto &r) { return l + "" + r; });

	QCommandLineOption frameless_opt(QStringList() << "b"
												   << "frameless",
		"Show a frameless window.");
	QCommandLineOption frontend_opt(QStringList() << "f"
												  << "frontend",
		"Selects the frontend. Must be one of:" + QString::fromStdString(frontends_descr) + " (auto is default)",
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
		"behaviour");

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
		int choice = std::ranges::find(frontend_opts, p.value(frontend_opt).toStdString()) - frontend_opts.cbegin();
		if (choice > static_cast<int>(Settings::Frontend::Stdout)) {
			std::cerr << "frontend needs to be one of the following:" << frontends_descr << std::endl;
			return false;
		}
		const auto c = static_cast<Settings::Frontend>(choice);
		if (c == Settings::Frontend::Immediate && Util::is_wayland()) {
			std::cerr << "Wayland does not have support for this frontend, as the spec requires an implicit grab for native wl_data_device::start_drag() operations, thus making it impossible to implement this workflow on Wayland." << std::endl
					  << "This frontend might work over XWayland (force it with QT_QPA_PLATFORM=xcb) but will likely be very buggy. Please switch to X11 to get the optimal usability experience or use another frontend." << std::endl;
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
