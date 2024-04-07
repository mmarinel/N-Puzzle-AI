/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tile.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 18:13:12 by matteo            #+#    #+#             */
/*   Updated: 2024/04/06 20:18:07 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Tile.hpp"

Tile::Tile(): nbr(-1)
{
	empty = true;
}

Tile::Tile(int _nbr): nbr{_nbr}
{
	empty = false;
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
		nbr = -1;
	}
	else
	{
		empty = false;
		nbr = other.nbr;
	}
	return *this;
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
