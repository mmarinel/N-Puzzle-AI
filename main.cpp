/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 19:40:48 by matteo            #+#    #+#             */
/*   Updated: 2024/04/07 17:13:52 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <QApplication>
#include <QFontDatabase>
#include <QDebug>

#include "Window.hpp"
#include "utils.hpp"

int main(int argc, char const* argv[])
{
	QApplication	app{
		argc, const_cast<char**>(argv)
	};

	QFontDatabase::addApplicationFont(":/fonts/bold");
	QFontDatabase::addApplicationFont(":/fonts/regular");
	QFontDatabase::addApplicationFont(":/fonts/medium");
	QFontDatabase::addApplicationFont(":/fonts/icons");

	// app.setFont(QFont("Roboto"));
	NPuzzle::addStyleSheet(app);
	Window	win{};

	win.show();
	auto	exit_status = app.exec();
	//TODO ...cleanup?
	return exit_status;
}
