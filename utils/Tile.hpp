/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tile.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cy4gate_mmarinelli <cy4gate_mmarinelli@    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 18:09:13 by matteo            #+#    #+#             */
/*   Updated: 2024/06/05 10:17:39 by cy4gate_mma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

//TODO
//TODO 1. togliere tutti uin8_t (per puzzle grandi: calcola sia male le euristiche che non riesce a descrivere tiles > 255)
//TODO 2. gestire new_window.
//TODO


class Tile
{
private:
	int			nbr;
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
	Tile(int _nbr);
	Tile(const Tile&);
	Tile&	operator=(const Tile& other);
	bool	operator==(const Tile& other);
	
	bool	isEmpty() const;
	operator int() const;
	operator std::string() const;
};
