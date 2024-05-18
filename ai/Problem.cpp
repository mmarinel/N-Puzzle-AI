/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Problem.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 22:31:59 by matteo            #+#    #+#             */
/*   Updated: 2024/05/18 16:47:00 by matteo           ###   ########.fr       */
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
	this->affected_col = s.affected_col;
	this->cols.reserve(s.size);
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
		// return true;
		return s1.configuration == s2.configuration;
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
	// else if (s1.col_sum != s2.col_sum)
	// 	return s1.col_sum < s2.col_sum;
	else
	{
		for (uint8_t i = 0; i < s1.size; i++)
		{
			if (s1.cols[i] != s2.cols[i])
				return s1.cols[i] < s2.cols[i];
		}
		return false;
	}
	// {
	// 	for (uint8_t i = 0; i < s1.size; i++) {
	// 		for (uint8_t j = 0; j < s1.size; j++) {
	// 			uint8_t	t1 = static_cast<uint8_t>(s1.configuration[i][j]);
	// 			uint8_t	t2 = static_cast<uint8_t>(s2.configuration[i][j]);
	// 			if ( t1 != t2 )
	// 				return t1 < t2;
	// 		}
	// 	}
	// 	return false;
	// }
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
	
	if ( t_action::UP == a )
	{
		Tile&	tile = (
			result->
			configuration
			[result->i_empty - 1][result->j_empty]
		);

		result->affected_col = result->j_empty;
		result->cols[result->affected_col] = (
			result->cols[result->affected_col] -
			(tile << (result->i_empty - 1)*8) +
			(tile << (result->i_empty)*8)
		);
		// if (result->initial_empty_col == result->j_empty)
		// {
		// 	result->col_sum = (
		// 		result->col_sum -
		// 		(tile << (result->i_empty - 1)*8) +
		// 		(tile << (result->i_empty)*8)
		// 	);
		// }
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

		result->affected_col = result->j_empty;
		result->cols[result->affected_col] = (
			result->cols[result->affected_col] -
			(tile << (result->i_empty + 1)*8) +
			(tile << (result->i_empty)*8)
		);
		// if (result->initial_empty_col == result->j_empty)
		// {
		// 	result->col_sum = (
		// 		result->col_sum -
		// 		(tile << (result->i_empty + 1)*8) +
		// 		(tile << (result->i_empty)*8)
		// 	);
		// }
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

		result->affected_col = result->j_empty - 1;
		result->cols[result->affected_col] = (
			result->cols[result->affected_col] -
			(tile << (result->i_empty)*8)
		);
		// if (result->initial_empty_col == result->j_empty - 1)
		// {
		// 	result->col_sum = (
		// 		result->col_sum -
		// 		(tile << (result->i_empty)*8)
		// 	);
		// }
		empty_tile = tile;
		tile = 0;
		result->i_empty = result->i_empty;
		result->j_empty = result->j_empty - 1;
	}
	if ( t_action::RIGHT == a )
	{
		Tile&	tile = (
			result->
			configuration
			[result->i_empty][result->j_empty + 1]
		);

		result->affected_col = result->j_empty + 1;
		result->cols[result->affected_col] = (
			result->cols[result->affected_col] -
			(tile << (result->i_empty)*8)
		);
		// if (result->initial_empty_col == result->j_empty + 1)
		// {
		// 	result->col_sum = (
		// 		result->col_sum -
		// 		(tile << (result->i_empty)*8)
		// 	);
		// }
		empty_tile = tile;
		tile = 0;
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
