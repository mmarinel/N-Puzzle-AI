/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 19:40:48 by matteo            #+#    #+#             */
/*   Updated: 2024/04/12 16:55:30 by matteo           ###   ########.fr       */
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

	BoardState::getInstance().setSize(8);
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		BoardState::getInstance().board[i][j] = Tile{1};

	NPuzzle::addStyleSheet(app);
	Window	win{};

	win.show();
	auto	exit_status = app.exec();
	//TODO ...cleanup?
	return exit_status;
}
