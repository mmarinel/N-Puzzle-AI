/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BoardState.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cy4gate_mmarinelli <cy4gate_mmarinelli@    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 20:27:38 by matteo            #+#    #+#             */
/*   Updated: 2024/06/05 23:23:08 by cy4gate_mma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>

#include "Tile.hpp"

class BoardState
{
public:
	int								size;
	int								x_empty;
	int								y_empty;
	std::vector<std::vector<Tile> >	board;

private:
	BoardState();
public:
	static BoardState&	getInstance();
						~BoardState();
						
	void				setSize(int	n);
	bool				contains(Tile nbr, size_t cap = std::string::npos);
	void				swap(int _x1, int _y1, int _x2, int _y2);
	// std::string			toString();
						
						BoardState(const BoardState&) = delete;
	BoardState&			operator=(const BoardState&) = delete;

	void				reset();
};
