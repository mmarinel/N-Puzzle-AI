/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 19:40:48 by matteo            #+#    #+#             */
/*   Updated: 2024/05/28 20:43:27 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <QApplication>
#include <QFontDatabase>

#include "Window.hpp"
#include "gui_utils.hpp"

int main(int argc, char const* argv[])
{
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
