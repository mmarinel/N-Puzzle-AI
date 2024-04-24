/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BoardView.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 18:41:36 by matteo            #+#    #+#             */
/*   Updated: 2024/04/24 23:43:00 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BoardView.hpp"
#include "Window.hpp"

#include <QtAlgorithms>

BoardView::BoardView(QWidget* parent): QWidget{parent},
	status(BoardState::getInstance()),
	grid(nullptr)
{
	grid = new QGridLayout();
	
	grid->setSpacing(1);
	grid->setContentsMargins(0, 0, 0, 0);
	this->setLayout(grid);
}

void	BoardView::setup()
{
	reset();
	
	for (int i = 0; i < status.size; i++)
	{
		for (int j = 0; j < status.size; j++)
		{
			TileView*	tile = new TileView{this};
			tile->setObjectName(TILE_OBJ_NAME);
			tile->setAlignment(Qt::AlignCenter);
			grid->addWidget(tile, i, j);
		}
	}

	this->resize(
		BoardState::getInstance().size * 50 + BoardState::getInstance().size - 1,
		BoardState::getInstance().size * 50 + BoardState::getInstance().size - 1
	);
}

void	BoardView::reset()
{
	QList<QWidget*>	tiles = this->findChildren<QWidget*>(
		QString(), Qt::FindDirectChildrenOnly
	);
	for (QWidget* tile: tiles)
	{
		grid->removeWidget(tile);
		delete tile;
	}
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
