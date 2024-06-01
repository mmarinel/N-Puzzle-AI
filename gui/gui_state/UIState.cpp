/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UIState.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 19:45:40 by matteo            #+#    #+#             */
/*   Updated: 2024/05/31 19:53:53 by matteo           ###   ########.fr       */
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
search_strategy{NPuzzle::t_search_strategy::NONE},
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
	search_strategy = NPuzzle::t_search_strategy::NONE;
	h = NPuzzle::t_heuristic::NONE;
}