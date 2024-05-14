/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tile.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 18:13:12 by matteo            #+#    #+#             */
/*   Updated: 2024/05/11 11:35:32 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Tile.hpp"

Tile::Tile(): nbr(0)
{
	empty = true;
}

Tile::Tile(int _nbr): nbr{_nbr}
{
	empty = (0 == nbr);
}

Tile::Tile(const Tile& other)
{
	*this = other;
}

Tile&	Tile::operator=(const Tile& other)
{
	if (other.empty)
	{
		empty = true;
		nbr = 0;
	}
	else
	{
		empty = false;
		nbr = other.nbr;
	}
	return *this;
}

bool	Tile::operator==(const Tile& other)
{
	return this->nbr == other.nbr;
}

bool	Tile::isEmpty() const
{
	return empty;
}

Tile::operator int() const
{
	return nbr;
}

Tile::operator std::string() const
{
	if (empty)
		return "";
	else
		return std::to_string(nbr);
}
