/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BoardView.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 18:41:36 by matteo            #+#    #+#             */
/*   Updated: 2024/04/10 19:11:04 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BoardView.hpp"

BoardView::BoardView(): QWidget(),
	status(BoardState::getInstance())
{
	grid = new QGridLayout();
	
	for (int i = 0; i < status.size; i++)
	{
		for (int j = 0; j < status.size; j++)
		{
			QLabel*	tile = new QLabel{};
			tile->setObjectName(TILE_OBJ_NAME);
			grid->addWidget(tile, i, j);
		}
	}

	this->setLayout(grid);
}

void	BoardView::paintEvent(QPaintEvent* event)
{
	
	for (int i = 0; i < status.size; i++)
	{
		for (int j = 0; j < status.size; j++)
		{
			static_cast<QLabel*>(
				grid->itemAtPosition(i, j)->widget()
			)->setText(
				static_cast<std::string>(
					status.board[i][j]
				).c_str()
			);
		}
	}
}

BoardView::~BoardView() {}

