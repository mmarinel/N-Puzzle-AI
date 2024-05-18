/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tile.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 18:09:13 by matteo            #+#    #+#             */
/*   Updated: 2024/05/16 22:44:01 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

class Tile
{
private:
	uint8_t		nbr;
	bool		empty;
	
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
	Tile(uint8_t _nbr);
	Tile(const Tile&);
	Tile&	operator=(const Tile& other);
	bool	operator==(const Tile& other);
	
	bool	isEmpty() const;
	operator uint8_t() const;
	operator std::string() const;
};
