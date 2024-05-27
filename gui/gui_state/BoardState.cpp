/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BoardState.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 20:59:35 by matteo            #+#    #+#             */
/*   Updated: 2024/05/27 10:09:17 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BoardState.hpp"

BoardState::BoardState(): size(0), x_empty(-1), y_empty(-1), board{}
{
	setSize(3);
}

BoardState::~BoardState() {}

BoardState& BoardState::getInstance() {
	static BoardState	state{};
	
	return	state;
}

void	BoardState::setSize(int	n)
{
	size = n;
	board.clear();
	board.resize(size);
	for (std::vector<Tile>&	row: board)
	{
		row.resize(size);
	}
}

bool	BoardState::contains(Tile nbr, size_t cap)
{
	size_t	checked_elements = 0;

	for(int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (cap == checked_elements)
				return false;
			if (nbr == board[i][j])
				return true;
			checked_elements += 1;
		}
	}
	return false;
}

void			BoardState::swap(int _x1, int _y1, int _x2, int _y2)
{
	Tile	backup = board[_y1][_x1];
	board[_y1][_x1] = board[_x2][_y2];
	board[_x2][_y2] = backup;
}

void		BoardState::reset()
{
	size = 0;
	x_empty = -1;
	y_empty = -1;
	board.clear();
}
