/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heuristics.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 13:01:18 by matteo            #+#    #+#             */
/*   Updated: 2024/05/14 22:18:42 by matteo           ###   ########.fr       */
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
	MISPLACED_TILES,
	GASCHNIG,
	MAX,
	NONE
};

const std::map<std::string, t_heuristic>
hfromString = {
	{"Manhattan Distance", t_heuristic::MANHATTAN_DISTANCE},
	{"Misplaced Tiles", t_heuristic::MISPLACED_TILES},
	{"Gaschnig's heurisitc", t_heuristic::GASCHNIG},
	{"Coalesce All", t_heuristic::MAX}
};

class	t_Iordering_func
{
public:
	virtual int		f_val(const Node* n) const = 0;
	virtual bool	cmp(const Node* n1, const Node* n2) const = 0;
};

template <typename H>
class t_ordering_func: public t_Iordering_func
{
public:
	virtual int			f_val(const Node* n) const override {
		return n->pCost + H::getInstance()(n);
		// return H::getInstance()(n);
	}

	virtual bool	cmp(const Node* n1, const Node* n2) const override {
		return f_val(n1) > f_val(n2);
		// auto	f1 = f_val(n1);
		// auto	f2 = f_val(n2);

		// if (f1 != f2)
		// 	return f1 > f2;
		// else
		// 	return H::getInstance()(n1) > H::getInstance()(n2);
		// 	// return (f1 - n1->pCost) > (f2 - n2->pCost);
	}
};



class t_manhattan_score: public t_ordering_func<t_manhattan_score>
{
public:
	int	operator() (const Node* n) const;
	
	static const t_manhattan_score&	getInstance();
private:
	t_manhattan_score();
};

class t_misplaced_tiles_score:
	public t_ordering_func<t_misplaced_tiles_score>
{
public:
	int	operator() (const Node* n) const;
	
	static const t_misplaced_tiles_score&	getInstance();
private:
	t_misplaced_tiles_score();
};

class t_gaschnig_score: public t_ordering_func<t_gaschnig_score>
{
public:
	int	operator() (const Node* n) const;

	static const t_gaschnig_score&	getInstance();
private:
	t_gaschnig_score();
};

class t_coalesce_score: public t_ordering_func<t_coalesce_score>
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
	{t_heuristic::MISPLACED_TILES, &t_misplaced_tiles_score::getInstance()},
	{t_heuristic::GASCHNIG, &t_gaschnig_score::getInstance()},
	{t_heuristic::MAX, &t_coalesce_score::getInstance()}
};

}
