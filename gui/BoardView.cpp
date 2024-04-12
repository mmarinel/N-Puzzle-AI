/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BoardView.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 18:41:36 by matteo            #+#    #+#             */
/*   Updated: 2024/04/11 20:49:30 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BoardView.hpp"
#include "Window.hpp"

BoardView::BoardView(): QWidget(),
	status(BoardState::getInstance())
{
	grid = new QGridLayout();
	
	for (int i = 0; i < status.size; i++)
	{
		for (int j = 0; j < status.size; j++)
		{
			TileView*	tile = new TileView{};
			tile->setObjectName(TILE_OBJ_NAME);
			tile->setAlignment(Qt::AlignCenter);
			grid->addWidget(tile, i, j);
		}
	}

	grid->setSpacing(1);
	grid->setContentsMargins(0, 0, 0, 0);

	// this->setFixedSize(
	// 	WIDTH / 2,
	// 	WIDTH / 2
	// );
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


void	BoardView::resizeEvent(QResizeEvent* event)
{
	double	aspect_ratio = (
		static_cast<double>(this->height())
		/
		static_cast<double>(this->width())
	);
	int		target_width;

	if (aspect_ratio < 1) // Too wide
	{
		this->setMaximumWidth(
			static_cast<int>(this->height())
		);
	}
	else if (aspect_ratio > 1) // Too tall
	{
		this->setMaximumHeight(
			static_cast<int>(this->width())
		);
	}
}
