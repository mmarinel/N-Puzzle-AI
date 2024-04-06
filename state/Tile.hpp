/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tile.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 18:09:13 by matteo            #+#    #+#             */
/*   Updated: 2024/04/06 18:28:26 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

class Tile
{
private:
	int		nbr;
	bool	empty;
	
public:
	/**
	 * @brief Construct an empty tile
	 * 
	 */
	Tile();
	/**
	 * @brief Construct a numbered tile
	 * 
	 * @param _nbr 
	 */
	Tile(int _nbr);
	Tile(const Tile&);
	Tile&	operator=(const Tile& other);
	
	operator int() const;
	operator std::string() const;
};
