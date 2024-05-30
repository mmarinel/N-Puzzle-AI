/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cy4gate_mmarinelli <cy4gate_mmarinelli@    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 22:19:23 by matteo            #+#    #+#             */
/*   Updated: 2024/05/30 18:30:10 by cy4gate_mma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "BoardState.hpp"
#include "UIState.hpp"

#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QDebug>

#include <algorithm>
#include <numeric>
#include <vector>
#include <string>

bool	NPuzzle::parse_file(QString filepath)
{
	QFile		file(filepath);
	QString		line;
	QStringList	parts;
	QStringList	next_row;
	int			i = 0, j = 0;
	bool		ok;
	size_t		size = std::string::npos;
	Tile		tile;
	size_t		parsed_elements_count = 0;

	if (false == file.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;
	QTextStream		in(&file);
	while (false == in.atEnd())
	{
		line = in.readLine().trimmed();
		// skip empty line
		if (line.isEmpty())
			continue ;
		// non-terminated line
		if (
			'$' != line[line.length() - 1] ||
			1 != line.count('$')
		)
			return false;
		parts = line.split(QRegularExpression("[#|$]"));
		// skip comment lines
		if (parts[0].trimmed().isEmpty())
			continue ;
		next_row = parts[0].trimmed().split(QRegularExpression("[,\\s]+"));
		// size not parsed yet
		if (std::string::npos == size)
		{
			// there are multiple numbers for the size line; ergo not a size line
			if(1 != next_row.length())
				return false;
			size = next_row[0].toUInt(&ok);
			// size was not a positive integer
			if (false == ok)
				return false;
			// continue reading file...
			BoardState::getInstance().size = size;
			BoardState::getInstance().board.clear();
			BoardState::getInstance().setSize(size);
			continue ;
		}
		// parsing next row
		// invalid row count
		if (size != static_cast<size_t>(next_row.length()))
			return false;
		for (QString nbr: next_row)
		{
			tile = nbr.toUInt(&ok);
			// not a positive integer
			if (false == ok)
				return false;
			// already parsed or not within boundaries
			if (
				false == (tile < size*size) ||
				BoardState::getInstance().contains(
					tile, parsed_elements_count
				)
			)
				return false;
			BoardState::getInstance().board[i][j] = tile;
			if (BoardState::getInstance().board[i][j].isEmpty())
			{
				BoardState::getInstance().y_empty = i;
				BoardState::getInstance().x_empty = j;
			}
			parsed_elements_count++;
			j++;
		}
		j = 0;
		i++;
	}
	return size*size == parsed_elements_count;
}

void	NPuzzle::generate_board()
{
	auto					size = BoardState::getInstance().size;
	std::vector<t_action>	v( NPuzzle::shuffle_moves);
	bool					solvable = UIState::getInstance().atRandomSolvable;
	State::t_configuration	goal_conf;
	std::pair<uint8_t, uint8_t>
							blank_pos;

	goal_conf.clear();
	goal_conf.resize(size);
	for (int i = 0; i < size; i++) {
		goal_conf[i].resize(size);
	}
	// Taking goal
	blank_pos = fillGridAsGoal(goal_conf, size, 0, 1, size*size);
	// Checking user-chosen solvability
	if (false == solvable)
	{
		Tile	last = goal_conf[size - 1][size - 1];
		goal_conf[size - 1][size - 1] = goal_conf[size - 1][size - 2];
		goal_conf[size - 1][size - 2] = last;
	}
	// Generating random moves
	for (t_action& a: v)
	{
		a = static_cast<t_action>(
			std::rand() / (1.0f + RAND_MAX) * 4
		);
	}
	// performing moves
	State::t_configuration&		board = BoardState::getInstance().board;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			board[i][j] = goal_conf[i][j];
		}
	}
	// board = goal_conf;
	for (t_action& a: v)
	{
		std::pair<int, int>
		movedTilePos = blank_pos;

		switch (a)
		{
		case t_action::UP:
			movedTilePos.first -= 1;
			break;
		case t_action::DOWN:
			movedTilePos.first += 1;
			break;
		case t_action::LEFT:
			movedTilePos.second -= 1;
			break;
		case t_action::RIGHT:
			movedTilePos.second += 1;
			break;
		
		default:
			break;
		}

		if (
			(movedTilePos.first < 0 || movedTilePos.first >= size) ||
			(movedTilePos.second < 0 || movedTilePos.second >= size)
		)
			continue ;
		board[blank_pos.first][blank_pos.second] = board[movedTilePos.first][movedTilePos.second];
		board[movedTilePos.first][movedTilePos.second] = 0;
		blank_pos = movedTilePos;
	}
	BoardState::getInstance().y_empty = blank_pos.first;
	BoardState::getInstance().x_empty = blank_pos.second;
}

std::pair<uint8_t, uint8_t>
NPuzzle::fillGridAsGoal(
	State::t_configuration& grid,
	int size,
	int offset,
	int nbr,
	int last_nbr
)
{
	if (1 == size)// Checking last element of the puzzle...(case N odd)
	{
		grid[0 + offset][0 + offset] = 0;
		
		return std::make_pair(0 + offset, 0 + offset);
	}

	int	i, j;

	//top row
	for (j = 0 + offset;			j < size + offset;	j++)
	{
		grid[0 + offset][j] = nbr++;
	}
	// right column
	for (i = (0 + offset)			+ 1;			i < size + offset;	i++)
	{
		grid[i][size + offset - 1] = nbr++;
	}
	//bottom row
	for (j = (size + offset - 1)	- 1;		j >= (0 + offset);	j--)
	{
		grid[size + offset - 1][j] = nbr++;
	}
		// Checking last element of the puzzle...(case N even)-->
		// -->in this case, after k iterations, the last element on the bottom row is the last one
	if (last_nbr == grid[size + offset - 1][j + 1])
	{
		grid[size + offset - 1][j + 1] = 0;

		return std::make_pair(size + offset - 1, j + 1);
	}
	//left column
	for (i = (size + offset - 1)	- 1;		i >= (0 + offset)	+ 1;	i--)
	{
		grid[i][0 + offset] = nbr++;
	}
	return fillGridAsGoal(grid, size - 2, offset + 1, nbr, last_nbr);
}
