/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Problem.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 20:31:42 by matteo            #+#    #+#             */
/*   Updated: 2024/05/13 23:06:58 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <map>
#include <stack>

#include "Tile.hpp"

class Node;

typedef enum e_action
{
	UP,
	DOWN,
	RIGHT,
	LEFT,
	NONE,
}	t_action;

std::string	actionToString(const t_action& a);

class State
{
public:
	typedef std::vector<std::vector<Tile> >	t_configuration;
public:
	int				size;
	int				i_empty;
	int				j_empty;
	long long		cmp_score;
	int				hCost;
	t_configuration	configuration;

	friend bool	operator==(const State& s1, const State& s2);
	friend bool	operator<(const State& s1, const State& s);
	
			State();
			State(const State& s);
	State&	operator=(const State& s) = delete;
			State(State&& s) = delete;
};


class Problem
{
public:
	State		initial;
	std::map<int, std::pair<int, int> >
				goal;//TODO make it const?

	/**
	 * @brief in state, empty tile is 0
	 * 
	 * @return const std::vector<t_action> 
	 */
	const std::vector<t_action>
			actions(const State& s) const;
	State*
			result(const State& s, t_action a) const;
	// int	step_cost(t_state s1, t_state s2) const;
	bool	goalTest(const State* s) const;
	std::stack<t_action>
			solution(const Node* n) const;
	
				Problem();
				Problem(const Problem& p) = delete;
				Problem(Problem&& p) = delete;
	Problem&	operator=(const Problem& p) = delete;
	
};
