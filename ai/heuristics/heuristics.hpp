/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heuristics.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cy4gate_mmarinelli <cy4gate_mmarinelli@    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 13:01:18 by matteo            #+#    #+#             */
/*   Updated: 2024/06/05 10:18:51 by cy4gate_mma      ###   ########.fr       */
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

const std::map<std::string, t_heuristic>
hfromString = {
	{"Manhattan Distance", t_heuristic::MANHATTAN_DISTANCE},
	{"Linear Conflict", t_heuristic::LINEAR_CONFLICT},
	{"Corner Tiles", t_heuristic::CORNER_TILES},
	{"Misplaced Tiles", t_heuristic::MISPLACED_TILES},
	{"Gaschnig's heurisitc", t_heuristic::GASCHNIG},
	{"Coalesce All", t_heuristic::MAX}
};

class	t_Iordering_func
{
public:
	virtual int		f_val(const Node* n) const = 0;
	virtual int		h(const Node* n) const = 0;
	virtual int		g(const Node* n) const = 0;
	virtual bool		cmp(const Node* n1, const Node* n2) const = 0;
};

template <typename H>
class t_RbsOrdering_func: public t_Iordering_func
{
public:
	virtual int			f_val(const Node* n) const override {
		if (-1 == n->f)
			const_cast<Node*>(n)->f = g(n) + H::getInstance()(n);
		return n->f;
	}

	virtual int	h(const Node* n) const override {
		return H::getInstance()(n);
	}

	virtual int	g(const Node* n) const override {
		return n->pCost;
	}
	
	virtual bool	cmp(const Node* n1, const Node* n2) const override {
		return f_val(n1) > f_val(n2);
	}
};



class t_manhattan_score: public t_RbsOrdering_func<t_manhattan_score>
{
public:
	int	operator() (const Node* n) const;
	
	static const t_manhattan_score&	getInstance();
private:
	t_manhattan_score();
};

class t_linear_conflict_score: public t_RbsOrdering_func<t_linear_conflict_score>
{
public:
	int	operator() (const Node* n) const;
	
	static const t_linear_conflict_score&	getInstance();
private:
	t_linear_conflict_score();
};

class t_corner_tiles_score: public t_RbsOrdering_func<t_corner_tiles_score>
{
public:
	int	operator() (const Node* n) const;
	
	static const t_corner_tiles_score&	getInstance();
private:
	t_corner_tiles_score();
};

class t_misplaced_tiles_score:
	public t_RbsOrdering_func<t_misplaced_tiles_score>
{
public:
	int	operator() (const Node* n) const;
	
	static const t_misplaced_tiles_score&	getInstance();
private:
	t_misplaced_tiles_score();
};

class t_gaschnig_score: public t_RbsOrdering_func<t_gaschnig_score>
{
public:
	int	operator() (const Node* n) const;

	static const t_gaschnig_score&	getInstance();
private:
	t_gaschnig_score();
};

class t_coalesce_score: public t_RbsOrdering_func<t_coalesce_score>
{
public:
	int	operator() (const Node* n) const;

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
