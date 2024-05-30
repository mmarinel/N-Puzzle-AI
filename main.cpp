/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cy4gate_mmarinelli <cy4gate_mmarinelli@    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 19:40:48 by matteo            #+#    #+#             */
/*   Updated: 2024/05/30 18:30:25 by cy4gate_mma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <QApplication>
#include <QFontDatabase>

#include <ctime>

#include "Window.hpp"
#include "gui_utils.hpp"

int main(int argc, char const* argv[])
{
	// seeding rng
	std::srand(std::time(NULL));
	
	QApplication	app{
		argc, const_cast<char**>(argv)
	};

	// Set style
	app.setWindowIcon(QIcon(":/images/icon"));

	QFontDatabase::addApplicationFont(":/fonts/bold");
	QFontDatabase::addApplicationFont(":/fonts/regular");
	QFontDatabase::addApplicationFont(":/fonts/medium");
	QFontDatabase::addApplicationFont(":/fonts/icons");

	NPuzzle::addStyleSheet(app);

	// Create widget tree
	Window	*win = new Window{};

	// Fire first event and start event loop
	win->show();
	auto	exit_status = app.exec();

	// Cleanup
	delete win;

	return exit_status;
}
