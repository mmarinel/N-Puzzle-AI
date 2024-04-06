/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BoardState.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 20:59:35 by matteo            #+#    #+#             */
/*   Updated: 2024/04/06 18:29:37 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BoardState.hpp"

BoardState::BoardState(): board{}, size(0), x_empty(-1), y_empty(-1) {}

BoardState::~BoardState() {}

BoardState& BoardState::getInstance() {
	static BoardState	state{};
	
	return	state;
}

void	BoardState::setSize(int	_size)
{
	size = _size;
	board.clear();
	board.resize(size);
	for (std::vector<Tile>	row: board)
	{
		row.resize(size);
	}
}

void			BoardState::swap(int _x1, int _y1, int _x2, int _y2)
{
	Tile	backup = board[_y1][_x1];
	board[_y1][_x1] = board[_x2][_y2];
	board[_x2][_y2] = backup;
}
