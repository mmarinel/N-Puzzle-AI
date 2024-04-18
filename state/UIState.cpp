/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UIState.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 19:45:40 by matteo            #+#    #+#             */
/*   Updated: 2024/04/18 20:39:20 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UIState.hpp"

UIState::UIState(): boardFileName{}, atRandom{false}, h{NPuzzle::t_heuristic::NONE}, currentPage{MENU} {}

UIState::~UIState() {}

UIState& UIState::getInstance() {
	static UIState	state{};
	
	return	state;
}
