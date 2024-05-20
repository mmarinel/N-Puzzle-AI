/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Problem.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 22:31:59 by matteo            #+#    #+#             */
/*   Updated: 2024/05/19 21:13:48 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Problem.hpp"
#include "Node.hpp"

State::State() {}

State::State(const State& s)
{
	this->size = s.size;
	this->i_empty = s.i_empty;
	this->j_empty = s.j_empty;
	this->configuration = s.configuration;
	this->cols.reserve(s.size);
	for (int j = 0; j < s.size; j++)
		this->cols[j] = s.cols[j];
	this->cols = s.cols;
	this->hCost = -1;
}

bool	operator==(const State& s1, const State& s2)
{
	if (s1.i_empty != s2.i_empty || s1.j_empty != s2.j_empty)
		return false;
	else if (s1.hCost != s2.hCost)
		return false;
	else
		return s1.cols == s2.cols;
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
		return s1.cols < s2.cols;
	// for (int i = 0; i < s1.size; i++)
	// {
	// 	for (int j = 0; j < s1.size; j++)
	// 	{
	// 		if (s1.configuration[i][j] != s2.configuration[i][j])
	// 			return s1.configuration[i][j] < s2.configuration[i][j];
	// 	}
	// }
	// return false;
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
	State*		result = new State{s};
	Tile&		empty_tile = (
			result->
			configuration
			[result->i_empty][result->j_empty]
	);
	int		empty_tile_col;
	int		new_empty_tile_col;
	
	if ( t_action::UP == a )
	{
		Tile&	tile = (
			result->
			configuration
			[result->i_empty - 1][result->j_empty]
		);

		new_empty_tile_col = result->j_empty;
		result->cols[new_empty_tile_col] = (
			result->cols[new_empty_tile_col] -
			(static_cast<uint64_t>(tile) << ((static_cast<int>(result->i_empty) - 1)*5) ) +
			(static_cast<uint64_t>(tile) << ((static_cast<int>(result->i_empty))*5) )
		);

		empty_tile = tile;
		tile = 0;
		
		result->i_empty = result->i_empty - 1;
		result->j_empty = result->j_empty;
	}
	if ( t_action::DOWN == a )
	{
		Tile&	tile = (
			result->
			configuration
			[result->i_empty + 1][result->j_empty]
		);

		new_empty_tile_col = result->j_empty;
		result->cols[new_empty_tile_col] = (
			result->cols[new_empty_tile_col] -
			(static_cast<uint64_t>(tile) << ((static_cast<int>(result->i_empty) + 1)*5) ) +
			(static_cast<uint64_t>(tile) << ((static_cast<int>(result->i_empty))*5) )
		);
		
		empty_tile = tile;
		tile = 0;
		
		result->i_empty = result->i_empty + 1;
		result->j_empty = result->j_empty;
	}
	if ( t_action::LEFT == a )
	{
		Tile&	tile = (
			result->
			configuration
			[result->i_empty][result->j_empty - 1]
		);

		new_empty_tile_col = result->j_empty - 1;
		result->cols[new_empty_tile_col] = (
			result->cols[new_empty_tile_col] -
			(static_cast<uint64_t>(tile) << (( static_cast<int>(result->i_empty))*5) )
		);
		empty_tile_col = result->j_empty;
		result->cols[empty_tile_col] = (
			result->cols[empty_tile_col] +
			(static_cast<uint64_t>(tile) << (( static_cast<int>(result->i_empty))*5) )
		);
		
		empty_tile = tile;
		tile = 0;
		
		result->i_empty = result->i_empty;
		result->j_empty = result->j_empty - 1;
	}
	if ( t_action::RIGHT == a )
	{
		// Taking moved out tile
		Tile&	tile = (
			result->
			configuration
			[result->i_empty][result->j_empty + 1]
		);

		// Updating columns representation
		new_empty_tile_col = result->j_empty + 1;
		result->cols[new_empty_tile_col] = (
			result->cols[new_empty_tile_col] -
			(static_cast<uint64_t>(tile) << ((static_cast<int>(result->i_empty))*5) )
		);
		empty_tile_col = result->j_empty;
		result->cols[empty_tile_col] = (
			result->cols[empty_tile_col] +
			(static_cast<uint64_t>(tile) << ((static_cast<int>(result->i_empty))*5) )
		);

		// swapping tiles
		empty_tile = tile;
		tile = 0;

		// Updating empty tile status
		result->i_empty = result->i_empty;
		result->j_empty = result->j_empty + 1;
	}
	return (
		result
	);
}

bool
Problem::goalTest(const State* s) const
{
	return 0 == s->hCost;
}

std::stack<t_action>
Problem::solution(const Node* n) const
{
	std::stack<t_action>	sol;
	const Node*				current;

	current = n;
	while (current->parent){
		sol.push(current->a);
		current = current->parent;
	}
	return sol;
}

std::string	actionToString(const t_action& a)
{
	switch (a)
	{
	case t_action::UP :
		return "UP";
		break;
	case t_action::DOWN :
		return "DOWN";
		break;
	case t_action::LEFT :
		return "LEFT";
		break;
	case t_action::RIGHT :
		return "RIGHT";
		break;
	
	default:
		return "NONE";
		break;
	}
}
