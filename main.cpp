/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 19:40:48 by matteo            #+#    #+#             */
/*   Updated: 2024/04/17 22:04:04 by matteo           ###   ########.fr       */
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

	app.setWindowIcon(QIcon(":/images/icon"));

	QFontDatabase::addApplicationFont(":/fonts/bold");
	QFontDatabase::addApplicationFont(":/fonts/regular");
	QFontDatabase::addApplicationFont(":/fonts/medium");
	QFontDatabase::addApplicationFont(":/fonts/icons");

	// app.setFont(QFont("Roboto"));

	int	count = 1;
	BoardState::getInstance().setSize(8);
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			if (8 - 1 != i || 8 -1 != j)
				BoardState::getInstance().board[i][j] = Tile{count++};

	NPuzzle::addStyleSheet(app);
	Window	win{};

	win.show();
	auto	exit_status = app.exec();
	//TODO ...cleanup?
	return exit_status;
}
