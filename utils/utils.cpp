/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 22:19:23 by matteo            #+#    #+#             */
/*   Updated: 2024/04/23 15:48:07 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "BoardState.hpp"

#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QDebug>
#include <string>

bool	NPuzzle::parse_file(QString filepath)
{
	qDebug() << "NPuzzle::parse_file(QString filepath) --- START";
	QFile		file(filepath);
	QString		line;
	QStringList	parts;
	QStringList	next_row;
	int			i = 0, j = 0;
	bool		ok;
	size_t		size = std::string::npos;
	size_t		parsed_elements = 0;

	if (false == file.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;
	QTextStream		in(&file);
	while (false == in.atEnd())
	{
		line = in.readLine().trimmed();
		// empty line
		if (line.isEmpty())
			continue ;
		// terminated line
		if (
			'$' != line[line.length() - 1] ||
			1 != line.count('$')
		)
			return false;
		parts = line.split(QRegularExpression("[#|$]"));
		// skip comment lines in file
		if (parts[0].trimmed().isEmpty())
			continue ;
		next_row = parts[0].trimmed().split(QRegularExpression("[,\\s]+"));
		// reading size
		if (std::string::npos == size)
		{
			// there are multiple number for the size line; ergo not a size line
			if(1 != next_row.length())
				return false;
			size = next_row[0].toUInt(&ok);
			BoardState::getInstance().size = size;
			// size was not a positive integer
			if (false == ok)
				return false;
			// continue reading file...
			continue ;
		}
		for (QString nbr: next_row)
		{
			BoardState::getInstance().board[i][j] = nbr.toUInt(&ok);
			if (false == ok)
				return false;
			parsed_elements++;
			j++;
		}
		j = 0;
		i++;
	}
	qDebug() << "parsed_elements = " << parsed_elements;
	qDebug() << "NPuzzle::parse_file(QString filepath) --- END";
	return size*size == parsed_elements;
}
