/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Problem.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 20:31:42 by matteo            #+#    #+#             */
/*   Updated: 2024/05/26 14:37:24 by matteo           ###   ########.fr       */
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
	typedef std::vector<uint64_t>		t_cols;
public:
	uint8_t			size;
	uint8_t			i_empty;
	uint8_t			j_empty;
	int				hCost;
	int				hManhattan;
	int				hLinearConflict;
	int				hCornerTiles;
	int				hMisplacedTiles;
	int				hGaschnig;
	int				hMax;
	t_configuration	configuration;
	t_cols			cols;//integer representation of columns

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
	std::map<uint8_t, std::pair<uint8_t, uint8_t> >
				goal;//TODO make it const?
	int			inversions_at_goal;
	int			x_empty_at_goal;
	int			y_empty_at_goal;

	/**
	 * @brief in state, empty tile is 0
	 * 
	 * @return const std::vector<t_action> 
	 */
	std::vector<t_action>
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
