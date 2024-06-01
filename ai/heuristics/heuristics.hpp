/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heuristics.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 13:01:18 by matteo            #+#    #+#             */
/*   Updated: 2024/05/31 20:10:11 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <map>

#include "Node.hpp"

namespace NPuzzle
{
enum class t_heuristic
{
	MANHATTAN_DISTANCE,
	LINEAR_CONFLICT,
	CORNER_TILES,
	MISPLACED_TILES,
	GASCHNIG,
	MAX,
	NONE
};

enum class t_search_strategy
{
	INFORMED,
	GREEDY,
	UNIFORM_COST,
	NONE
};

const std::map<std::string, t_heuristic>
hfromString = {
	{"Manhattan Distance", t_heuristic::MANHATTAN_DISTANCE},
	{"Linear Conflict", t_heuristic::LINEAR_CONFLICT},
	{"Corner Tiles", t_heuristic::CORNER_TILES},
	{"Misplaced Tiles", t_heuristic::MISPLACED_TILES},
	{"Gaschnig's heurisitc", t_heuristic::GASCHNIG},
	{"Coalesce All", t_heuristic::MAX}
};

const std::map<std::string, t_search_strategy>
strategyFromString = {
	{"Informed (A* standard)", t_search_strategy::INFORMED},
	{"Greedy", t_search_strategy::GREEDY},
	{"Uniform Cost - like", t_search_strategy::UNIFORM_COST}
};

class	t_Iordering_func
{
public:
	virtual uint8_t		f_val(const Node* n) const = 0;
	virtual uint8_t		h(const Node* n) const = 0;
	virtual uint8_t		g(const Node* n) const = 0;
	virtual bool		cmp(const Node* n1, const Node* n2) const = 0;
};

template <typename H>
class t_ordering_func: public t_Iordering_func
{
public:
	virtual uint8_t			f_val(const Node* n) const override {
		return n->pCost + H::getInstance()(n);
	}

	virtual uint8_t	h(const Node* n) const override {
		return H::getInstance()(n);
	}

	virtual uint8_t	g(const Node* n) const override {
		return n->pCost;
	}

	virtual bool	cmp(const Node* n1, const Node* n2) const override {
		return f_val(n1) > f_val(n2);
	}
};



class t_manhattan_score: public t_ordering_func<t_manhattan_score>
{
public:
	uint8_t	operator() (const Node* n) const;
	
	static const t_manhattan_score&	getInstance();
private:
	t_manhattan_score();
};

class t_linear_conflict_score: public t_ordering_func<t_linear_conflict_score>
{
public:
	uint8_t	operator() (const Node* n) const;
	
	static const t_linear_conflict_score&	getInstance();
private:
	t_linear_conflict_score();
};

class t_corner_tiles_score: public t_ordering_func<t_corner_tiles_score>
{
public:
	uint8_t	operator() (const Node* n) const;
	
	static const t_corner_tiles_score&	getInstance();
private:
	t_corner_tiles_score();
};

class t_misplaced_tiles_score:
	public t_ordering_func<t_misplaced_tiles_score>
{
public:
	uint8_t	operator() (const Node* n) const;
	
	static const t_misplaced_tiles_score&	getInstance();
private:
	t_misplaced_tiles_score();
};

class t_gaschnig_score: public t_ordering_func<t_gaschnig_score>
{
public:
	uint8_t	operator() (const Node* n) const;

	static const t_gaschnig_score&	getInstance();
private:
	t_gaschnig_score();
};

class t_coalesce_score: public t_ordering_func<t_coalesce_score>
{
public:
	uint8_t	operator() (const Node* n) const;

	static const t_coalesce_score& getInstance();
private:
	t_coalesce_score();
};


const std::map<t_heuristic, const t_Iordering_func*>
hToFunc = {
	{t_heuristic::MANHATTAN_DISTANCE, &t_manhattan_score::getInstance()},
	{t_heuristic::LINEAR_CONFLICT, &t_linear_conflict_score::getInstance()},
	{t_heuristic::CORNER_TILES, &t_corner_tiles_score::getInstance()},
	{t_heuristic::MISPLACED_TILES, &t_misplaced_tiles_score::getInstance()},
	{t_heuristic::GASCHNIG, &t_gaschnig_score::getInstance()},
	{t_heuristic::MAX, &t_coalesce_score::getInstance()}
};

}
