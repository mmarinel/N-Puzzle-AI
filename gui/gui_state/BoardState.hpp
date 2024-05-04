/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BoardState.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 20:27:38 by matteo            #+#    #+#             */
/*   Updated: 2024/04/25 12:26:26 by matteo           ###   ########.fr       */
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
						
						BoardState(const BoardState&) = delete;
	BoardState&			operator=(const BoardState&) = delete;
};
