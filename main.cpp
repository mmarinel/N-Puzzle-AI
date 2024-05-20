/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 19:40:48 by matteo            #+#    #+#             */
/*   Updated: 2024/05/20 22:00:31 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <QApplication>
#include <QFontDatabase>
#include <QDebug>

#include "Window.hpp"
#include "gui_utils.hpp"

#if defined(CONFIG_ARCH_SUPPORTS_INT128) && defined(__SIZEOF_INT128__)
#define IS_DEF 1
#else
#define IS_DEF 0
#endif

int main(int argc, char const* argv[])
{
	QApplication	app{
		argc, const_cast<char**>(argv)
	};

	qDebug() << "128-bit integers supported: " << IS_DEF;
	qDebug() << "sizeof(uint64_t): " << sizeof(uint64_t);
	qDebug() << "sizeof(__uint128_t): " << sizeof(__uint128_t);
	qDebug() << "sizeof(int): " << sizeof(int);
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
