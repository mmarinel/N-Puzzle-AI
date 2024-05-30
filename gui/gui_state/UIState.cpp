/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UIState.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cy4gate_mmarinelli <cy4gate_mmarinelli@    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 19:45:40 by matteo            #+#    #+#             */
/*   Updated: 2024/05/30 18:16:33 by cy4gate_mma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UIState.hpp"

UIState::UIState():
currentPage{MENU},
boardFileName{},
atRandom{false},
atRandomSolvable{false},
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
	atRandomSolvable = false;
	size = 0;
	x_empty = -1;
	y_empty = -1;
	h = NPuzzle::t_heuristic::NONE;
}