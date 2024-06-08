/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BoardView.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 18:41:36 by matteo            #+#    #+#             */
/*   Updated: 2024/06/08 18:49:41 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BoardView.hpp"
#include "Window.hpp"
#include "gui_utils.hpp"

#include <QApplication>

BoardView::BoardView(QWidget* parent): QWidget{parent},
	status(BoardState::getInstance()),
	ui_status(UIState::getInstance()),
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
		nullptr, Qt::FindDirectChildrenOnly
	);
	for (QWidget* tile: tiles)
	{
		grid->removeWidget(tile);
		delete tile;
	}
	ui_status.x_empty = -1;
	ui_status.y_empty = -1;
}

void	BoardView::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);
	bool	empty;
	
	for (int i = 0; i < status.size; i++)
	{
		for (int j = 0; j < status.size; j++)
		{
			QLabel*	tile = static_cast<QLabel*>(
				grid->itemAtPosition(i, j)->widget()
			);
			tile->setText(
				static_cast<std::string>(
					status.board[i][j]
				).c_str()
			);
			empty = false;
			if (
				status.x_empty == j &&
				status.y_empty == i
			)
				empty = true;
			tile->setProperty("Empty", empty);
		}
	}
	if (
		ui_status.x_empty != status.x_empty ||
		ui_status.y_empty != status.y_empty
	)
	{//Empty tile moved
		qApp->setStyleSheet("");
		NPuzzle::addStyleSheet(*qApp);
		ui_status.x_empty = status.x_empty;
		ui_status.y_empty = status.y_empty;
	}
}

BoardView::~BoardView() {}
