/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Agent.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cy4gate_mmarinelli <cy4gate_mmarinelli@    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 21:07:49 by matteo            #+#    #+#             */
/*   Updated: 2024/05/30 16:36:00 by cy4gate_mma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Problem.hpp"
#include "heuristics.hpp"

#include <QThread>
#include <QDebug>

#include <set>
#include <stack>
#include <queue>
#include <vector>
#include <functional>
#include <limits>

namespace NPuzzle
{

class t_frontierNodesEquals;

template <
	typename T,
	class Container = std::vector<T*>,
	class Compare = std::less<typename Container::value_type>,
	class EqualCmp = t_frontierNodesEquals
>
class	OpenSetQueue: public std::priority_queue<T*, Container, Compare>
{
private:
	const EqualCmp&		equals;
	const Compare&		comp;
public:

	typedef typename 
		std::priority_queue<T*, Container, Compare>
			::container_type::iterator
		iterator;
	typedef typename 
		std::priority_queue<T*, Container, Compare>
			::value_type
		value_type;
	
	static unsigned long long		nbr_selected;

	OpenSetQueue(const Compare& cmp, const EqualCmp& equals):
		std::priority_queue<T*, Container, Compare>{cmp},
		equals{equals},
		comp{cmp}
	{}

	~OpenSetQueue()
	{
		for (auto el: this->c)
		{
			delete el;
		}
	}

	std::pair<iterator, const T*>	find(const T* val)
	{
		auto first = this->c.begin();
		auto last = this->c.end();
		
		while (first != last)
		{
			if (equals(val, *first))
				return std::pair<iterator, const T*>{first, *first};
			first++;
		}
		return std::pair<iterator, const T*>{last, nullptr};
	}

	void	replace(iterator oldEl, T* newEl)
	{
		this->c.erase(oldEl);
		std::make_heap(this->c.begin(), this->c.end(), comp);
		this->push(newEl);
	}

	iterator	end() noexcept{
		return this->c.end();
	}
	iterator	begin() noexcept{
		return this->c.begin();
	}

	void	pop()
	{
		nbr_selected += 1;
		std::priority_queue<T*, Container, Compare>::pop();
	}
};




class	Agent: public QThread
{
	Q_OBJECT
public:

	typedef std::function<bool(const Node*, const Node*)>	ordering_criteria;
	
	typedef struct s_exploredSet_cmp
	{
		bool	operator()(const State* s1, const State* s2) const
		{
			return *s1 < *s2;
		}
	}	t_exploredSet_cmp;
	typedef struct s_exploredSetNode_cmp
	{
		bool	operator()(const Node* const& n1, const Node* const& n2) const
		{
			return *(n1->s) < *(n2->s);
		}
	}	t_exploredSetNode_cmp;
	
	typedef struct s_frontierNodesEquals
	{
		bool	operator()(const Node* n1, const Node* n2) const
		{
			return *(n1->s) == *(n2->s);
		}
	}	t_frontierNodesEquals;
	
	typedef OpenSetQueue<
		Node, std::vector<Node*>, ordering_criteria, t_frontierNodesEquals
		>
	OpenSetNodeQueue;

	typedef std::set<State*, t_exploredSet_cmp>
	ClosedSetStateQueue;

	typedef std::set<Node*, t_exploredSetNode_cmp>
	ClosedSetNodeQueue;

	typedef struct	s_rbfsIterResult
	{
		int					cutoff;
		bool				solutionFound;
		Problem::Actions	actions;
		bool				failure;
	}	t_rbfsIterResult;
	
private:
	Problem					p;
	/**
	 * @brief to initialize before calling run
	 * 
	 */
	const t_Iordering_func*	criteria;
	ordering_criteria		worse;

private slots:
	void	aStar();
	void	rbfs();

private:
	t_rbfsIterResult
			rbfsRec(
				Node* node,
				ClosedSetStateQueue& explored,
				int bound
			);
	bool	solvable(State* initial);
	
	
	/**
	 * @brief this function sets the given map as the SNAIL goal state for the n-puzzle
	 * 
	 * @param state 
	 */
	void	setAsForwardGoal(
		Problem& p,
		std::map<uint8_t, std::pair<uint8_t, uint8_t>>& state,
		size_t size
	);
	/**
	 * @brief this function recursively fills a grid with the goal configuration (SNAIL solution).
	 * It does its job by filling the external frame (up-bottom edge + left-right edge)
	 * and then considering a smaller grid on the next iteration 
	 * @param grid 
	 * @param size size of current (sub)grid
	 * @param offset offset from the up-bottom and side edges of current iteration (current sub-grid)
	 * @param nbr the next tile number to put
	 * @return (i,j) coordinates of empty tile
	 */
	std::pair<uint8_t, uint8_t>
	fillGridAsGoal(
		State::t_configuration& grid,
		int size,
		int offset,
		int nbr
	);
	const std::vector<t_action>
			usefulActions(
				const Node* node,
				std::vector<t_action>&& actions
			);

public:
	Agent(
		const std::vector<std::vector<Tile> >& config,
		const size_t size,
		uint8_t	x_empty,
		uint8_t	y_empty,
		t_heuristic h
	);
	~Agent();
	
	void			run() override;
	const Problem&	problem();

	std::stack<t_action>	solution;
	int						moves;

signals:
	void	workDone();
};
}
