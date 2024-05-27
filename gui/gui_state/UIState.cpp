/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UIState.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 19:45:40 by matteo            #+#    #+#             */
/*   Updated: 2024/05/27 10:00:18 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UIState.hpp"

UIState::UIState():
currentPage{MENU},
boardFileName{},
atRandom{false},
size{0},
x_empty{-1},
y_empty{-1},
h{NPuzzle::t_heuristic::NONE}
{}

UIState::~UIState() {}

UIState& UIState::getInstance() {
	static UIState	state{};
	
	return	state;
}

void	UIState::reset()
{
	currentPage = MENU;
	boardFileName.clear();
	atRandom = false;
	size = 0;
	x_empty = -1;
	y_empty = -1;
	h = NPuzzle::t_heuristic::NONE;
}