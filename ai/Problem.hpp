/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Problem.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cy4gate_mmarinelli <cy4gate_mmarinelli@    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 20:31:42 by matteo            #+#    #+#             */
/*   Updated: 2024/06/05 10:21:19 by cy4gate_mma      ###   ########.fr       */
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

class State
{
public:
	typedef std::vector<std::vector<Tile> >	t_configuration;
	typedef std::vector<uint64_t>		t_cols;
public:
	int				size;
	int				i_empty;
	int				j_empty;
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
	typedef std::stack<t_action>	Actions;

	typedef enum e_polarity
	{
		EVEN,
		ODD
	}	t_polarity;
	
public:
	State		initial;
	int			inversions_at_initial;
	t_polarity	polarity_at_initial;
	std::map<int, std::pair<int, int> >
				goal;//TODO make it const?
	int			inversions_at_goal;
	t_polarity	polarity_at_goal;
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
	// int		step_cost(t_state s1, t_state s2) const;
	bool		goalTest(const State* s) const;
	Actions
				solution(const Node* n) const;
	static t_action
				inverseAction(const t_action& a);
	/**
	 * @brief 
	 * 
	 * @param state 
	 * @return n° inversions + polarity 
	 */
	static std::pair<int, t_polarity>
				polarity(
					const State::t_configuration& conf,
					int size
				);

				Problem();
				Problem(const Problem& p) = delete;
				Problem(Problem&& p) = delete;
	Problem&	operator=(const Problem& p) = delete;
	
};
