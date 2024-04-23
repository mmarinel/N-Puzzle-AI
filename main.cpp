/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 19:40:48 by matteo            #+#    #+#             */
/*   Updated: 2024/04/23 14:54:23 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <QApplication>
#include <QFontDatabase>
#include <QDebug>

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
	
	/// TESTs
	int	count = 1;
	BoardState::getInstance().setSize(9);
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			if (9 - 1 != i || 9 -1 != j)
				BoardState::getInstance().board[i][j] = Tile{count++};
	/// TESTs

	// Create widget tree
	Window	*win = new Window{};

	// Fire first event and start event loop
	win->show();
	auto	exit_status = app.exec();

	// Cleanup
	delete win;

	return exit_status;
}
