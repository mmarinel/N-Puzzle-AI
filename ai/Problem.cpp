/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Problem.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 22:31:59 by matteo            #+#    #+#             */
/*   Updated: 2024/06/08 17:43:25 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Problem.hpp"
#include "Node.hpp"

State::State()
{
	// heuristic values init
	this->hCost = -1;
	this->hManhattan = -1;
	this->hLinearConflict = -1;
	this->hCornerTiles = -1;
	this->hMisplacedTiles = -1;
	this->hGaschnig = -1;
	this->hMax = -1;
}

State::State(const State& s)
{
	// basic state info
	this->size = s.size;
	this->i_empty = s.i_empty;
	this->j_empty = s.j_empty;
	this->configuration = s.configuration;

	// columns representation
	this->cols.reserve(s.size);
	for (int j = 0; j < s.size; j++)
		this->cols[j] = s.cols[j];
	
	// heuristic values init
	this->hCost = -1;
	this->hManhattan = -1;
	this->hLinearConflict = -1;
	this->hCornerTiles = -1;
	this->hMisplacedTiles = -1;
	this->hGaschnig = -1;
	this->hMax = -1;
}

bool	operator==(const State& s1, const State& s2)
{
	if (s1.i_empty != s2.i_empty || s1.j_empty != s2.j_empty)
		return false;
	else if (s1.hCost != s2.hCost)
		return false;
	else
	{
		for (int k = 0; k < s1.size; k++)
			if (s1.cols[k] != s2.cols[k])
				return false;
		return true;
	}
}

bool	operator<(const State& s1, const State& s2)
{
	if (s1.i_empty != s2.i_empty || s1.j_empty != s2.j_empty)
		return (
			s1.i_empty < s2.i_empty ||
			(s1.i_empty == s2.i_empty && s1.j_empty < s2.j_empty)
		);
	else if (s1.hCost != s2.hCost)
		return s1.hCost < s2.hCost;
	else
	{
		for (int k = 0; k < s1.size; k++)
			if (s1.cols[k] != s2.cols[k])
				return s1.cols[k] < s2.cols[k];
		return false;
	}
}


Problem::Problem(): initial{}, goal{} {}

std::vector<t_action>
Problem::actions(const State& s) const
{
	std::vector<t_action>	_actions;
	
	if (s.j_empty > 0)
		_actions.push_back(t_action::LEFT);
	if (s.j_empty < s.size - 1)
		_actions.push_back(t_action::RIGHT);
	if (s.i_empty > 0)
		_actions.push_back(t_action::UP);
	if (s.i_empty < s.size - 1)
		_actions.push_back(t_action::DOWN);
	return (
		_actions
	);
}

State*
Problem::result(const State& s, t_action a) const
{
	State*		result = new State{s};//exact copy of parent
	Tile&		empty_tile = result->configuration[result->i_empty][result->j_empty];
	std::pair<int, int>
				moveTileOldPos = {
					// i
					t_action::UP == a
					? result->i_empty - 1
					: (t_action::DOWN == a ? result->i_empty + 1 : result->i_empty),
					// j
					t_action::LEFT == a
					? result->j_empty - 1
					: (t_action::RIGHT == a ? result->j_empty + 1 : result->j_empty)
				};
	std::pair<int, int>
				moveTileNewPos = {
					result->i_empty, result->j_empty
				};
	Tile&		tile = result->configuration[moveTileOldPos.first][moveTileOldPos.second];

	// Updating columns representation
	if (moveTileOldPos.second == moveTileNewPos.second)//Vertical Move
	{
		result->cols[moveTileOldPos.second] = (
			result->cols[moveTileOldPos.second] -// Up move means empty tile goes up, other tile goes down...
			(static_cast<uint64_t>(tile) << (static_cast<int>(moveTileOldPos.first)*5) ) +
			(static_cast<uint64_t>(tile) << (static_cast<int>(moveTileNewPos.first)*5) )
		);
	}
	else//Horizontal Move
	{
		result->cols[moveTileOldPos.second] = (
			result->cols[moveTileOldPos.second] -
			(static_cast<uint64_t>(tile) << (( static_cast<int>(moveTileOldPos.first))*5) )
		);
		result->cols[moveTileNewPos.second] = (
			result->cols[moveTileNewPos.second] +
			(static_cast<uint64_t>(tile) << (( static_cast<int>(moveTileNewPos.first))*5) )
		);
	}
	
	// Swapping tiles
	empty_tile = tile;
	tile = 0;
	
	// Updating empty tile info
	result->i_empty = moveTileOldPos.first;
	result->j_empty = moveTileOldPos.second;
	
	// heuristic values init
	result->hCost = -1;
	result->hManhattan = -1;
	result->hLinearConflict = -1;
	result->hCornerTiles = -1;
	result->hMisplacedTiles = -1;
	result->hGaschnig = -1;
	result->hMax = -1;
	
	return (
		result
	);
}

bool
Problem::goalTest(const State* s) const
{
	return 0 == s->hCost;
}

Problem::Actions
Problem::solution(const Node* n) const
{
	Actions		sol;
	const Node*	current;

	current = n;
	while (current->parent){
		sol.push(current->a);
		current = current->parent;
	}
	return sol;
}

t_action
Problem::inverseAction(const t_action& a)
{
	t_action	inverse_a = t_action::NONE;
	
	switch (a)
	{
	case t_action::UP:
		inverse_a = t_action::DOWN;
		break;

	case t_action::DOWN:
		inverse_a = t_action::UP;
		break;

	case t_action::LEFT:
		inverse_a = t_action::RIGHT;
		break;
	
	case t_action::RIGHT:
		inverse_a = t_action::LEFT;
		break;
	
	default:
		break;
	}

	return inverse_a;
}

std::pair<unsigned long, Problem::t_polarity>
Problem::polarity(const State::t_configuration& conf, int size)
{
	unsigned long	inversions = 0;
	int				firstNext_i;
	int				firstNext_j;
	int				current_i;
	int				current_j;
	int				k, i, j;

	for (k = 0; k < size * size; k++) {
		// Taking coordinate for the k-th element in the gird
		current_i = k / size;
		current_j = k % size;
		firstNext_i = (k + 1) / size;
		firstNext_j = (k + 1) % size;
		
		if (0 == conf[current_i][current_j])
			continue ;
		j = firstNext_j;
		for (i = firstNext_i; i < size; i++) {
			for (; j < size; j++) {
				if (0 == conf[i][j])
					continue ;
				inversions += (conf[current_i][current_j] > conf[i][j]);
			}
			j = 0;
		}
	}
	return std::make_pair(
		inversions,
		inversions % 2 == 0
			? t_polarity::EVEN
			: t_polarity::ODD
	);
}
