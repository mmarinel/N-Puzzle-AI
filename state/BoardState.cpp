/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BoardState.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 20:59:35 by matteo            #+#    #+#             */
/*   Updated: 2024/04/11 19:38:23 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BoardState.hpp"

BoardState::BoardState(): board{}, size(0), x_empty(-1), y_empty(-1) {}

BoardState::~BoardState() {}

BoardState& BoardState::getInstance() {
	static BoardState	state{};
	
	return	state;
}
#include <iostream>
void	BoardState::setSize(int	n)
{
	size = n;
	board.clear();
	board.resize(size);
	for (std::vector<Tile>&	row: board)
	{
		row.resize(size);
	}
	std::cout << "BoardState::setSize(int n) --- END" << std::endl;
}

void			BoardState::swap(int _x1, int _y1, int _x2, int _y2)
{
	Tile	backup = board[_y1][_x1];
	board[_y1][_x1] = board[_x2][_y2];
	board[_x2][_y2] = backup;
}
