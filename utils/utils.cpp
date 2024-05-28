/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 22:19:23 by matteo            #+#    #+#             */
/*   Updated: 2024/05/28 20:44:29 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "BoardState.hpp"

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
	auto				size = BoardState::getInstance().size;
	std::vector<int>	v(size*size);

	std::iota(v.begin(), v.end(), 0);
	std::random_shuffle(v.begin(), v.end());

	auto	it = v.begin();
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			BoardState::getInstance().board[i][j] = *it;
			if (BoardState::getInstance().board[i][j].isEmpty())
			{
				BoardState::getInstance().y_empty = i;
				BoardState::getInstance().x_empty = j;
			}
			it++;
		}
	}
}
