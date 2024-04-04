/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BoardState.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 20:59:35 by matteo            #+#    #+#             */
/*   Updated: 2024/04/04 21:02:41 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BoardState.hpp"

BoardState* BoardState::state = nullptr;

BoardState::BoardState(): board(nullptr), size(0), x_empty(-1), y_empty(-1) {}

BoardState::~BoardState() {
	if (nullptr == board)
		return ;
	for (int i = 0; i < size; i++)
		delete[] board[i];
	delete[] board;
}

BoardState& BoardState::getInstance() {
	if (nullptr == state)
		state = new BoardState{};
	
	return	*state;
}
