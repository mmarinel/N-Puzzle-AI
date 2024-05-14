/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Problem.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 22:31:59 by matteo            #+#    #+#             */
/*   Updated: 2024/05/14 21:53:08 by matteo           ###   ########.fr       */
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
		// return s1.cmp_score == s2.cmp_score;
}

bool	operator<(const State& s1, const State& s2)
{
	// return s1.cmp_score < s2.cmp_score;
	if (s1.i_empty != s2.i_empty || s1.j_empty != s2.j_empty)
		return (
			s1.i_empty < s2.i_empty ||
			(s1.i_empty == s2.i_empty && s1.j_empty < s2.j_empty)
		);
	else if (s1.hCost != s2.hCost)
		return s1.hCost < s2.hCost;
	else
	{
		for (int i = 0; i < s1.size; i++) {
			for (int j = 0; j < s1.size; j++) {
				int	t1 = static_cast<int>(s1.configuration[i][j]);
				int	t2 = static_cast<int>(s2.configuration[i][j]);
				if ( t1 != t2 )
					return t1 < t2;
			}
		}
		return false;
	}
}


Problem::Problem(): initial{}, goal{} {}

const std::vector<t_action>
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
		empty_tile = tile;
		tile = 0;
		result->i_empty = result->i_empty - 1;
		result->j_empty = result->j_empty;
		// result->cmp_score = s.cmp_score - result->size;
		result->cmp_score = s.cmp_score - tile*(s.i_empty) + tile*(result->i_empty);
	}
	if ( t_action::DOWN == a )
	{
		Tile&	tile = (
			result->
			configuration
			[result->i_empty + 1][result->j_empty]
		);
		empty_tile = tile;
		tile = 0;
		result->i_empty = result->i_empty + 1;
		result->j_empty = result->j_empty;
		// result->cmp_score = s.cmp_score + result->size;
		result->cmp_score = s.cmp_score - tile*(s.i_empty) + tile*(result->i_empty);
	}
	if ( t_action::LEFT == a )
	{
		Tile&	tile = (
			result->
			configuration
			[result->i_empty][result->j_empty - 1]
		);
		empty_tile = tile;
		tile = 0;
		result->i_empty = result->i_empty;
		result->j_empty = result->j_empty - 1;
		// result->cmp_score = s.cmp_score - 1;
		result->cmp_score = s.cmp_score - tile*(s.j_empty) + tile*(result->j_empty);
	}
	if ( t_action::RIGHT == a )
	{
		Tile&	tile = (
			result->
			configuration
			[result->i_empty][result->j_empty + 1]
		);
		empty_tile = tile;
		tile = 0;
		result->i_empty = result->i_empty;
		result->j_empty = result->j_empty + 1;
		// result->cmp_score = s.cmp_score + 1;
		result->cmp_score = s.cmp_score - tile*(s.j_empty) + tile*(result->j_empty);
	}
	return (
		result
	);
}

bool
Problem::goalTest(const State* s) const
{
	for (int i = 0; i < s->size; i++) {
		for (int j = 0; j < s->size; j++) {
			if (std::make_pair(i, j) != goal.at(s->configuration[i][j]))
				return false;
		}
	}
	return true;
}
#include <QDebug>
std::stack<t_action>
Problem::solution(const Node* n) const
{
	qDebug() << "Found Solution !";
	
	std::stack<t_action>	sol;
	const Node*				current;

	current = n;
	while (current->parent){
		sol.push(current->a);
		current = current->parent;
	}
	qDebug() << "Returning solution";
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
