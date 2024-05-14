/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AStar.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 21:13:01 by matteo            #+#    #+#             */
/*   Updated: 2024/05/14 22:19:54 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AStar.hpp"

#include <queue>
#include <vector>
#include <set>
#include <functional>

NPuzzle::Agent::Agent(
		const std::vector<std::vector<Tile> >& config,
		const size_t size,
		int	x_empty,
		int	y_empty,
		t_heuristic h
)
: p{}, solution{}, criteria(nullptr)
{
	// setting up Problem(s)
		// initial state
	p.initial.configuration = config;
	p.initial.cmp_score = 0;
	p.initial.size = size;
	p.initial.i_empty = y_empty;
	p.initial.j_empty = x_empty;
		// goal state
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			p.goal[(i*size) + (j+1)] = std::make_pair(i, j);
		}
	}
	p.goal.erase(size*size);
	p.goal[0] = std::make_pair(size - 1, size - 1);
	qDebug() << "p.goal";
	for(const auto& pair: p.goal)
	{
		qDebug()
			<< "nbr: " << pair.first
			<< "; (i, j): " << pair.second.first << ", " << pair.second.second;
		
	}


	// for (int i = 0; i < size; i++) {
	// 	for (int j = 0; j < size; j++) {
	// 		p.initial.cmp_score += p.initial.configuration[i][j]*
	// 	}
	// }

	// setting A* specific politics
		// heuristic
	this->criteria = NPuzzle::hToFunc.at(h);
	worse = std::bind(
		&NPuzzle::t_Iordering_func::cmp,
		this->criteria,
		std::placeholders::_1, std::placeholders::_2
	);
}

void	NPuzzle::Agent::run()
{
	this->aStar();
}
#include <iostream>
void	NPuzzle::Agent::aStar()
{
	OpenSetNodeQueue	frontier{worse, t_frontierNodesEquals{}};
	ClosedSetStateQueue	explored;
	Node*				node = new Node{p};
	node->s 				 = new State{p.initial};
	
	frontier.push(node);
	while (false == frontier.empty())
	{
		node = frontier.top();
		frontier.pop();
		qDebug() << "Popped node with f_cost: "
			<< this->criteria->f_val(node)
			<< " and h: " << this->criteria->f_val(node) - node->pCost;
		for (int i = 0; i < node->s->size; i++) {
			for (int j = 0; j < node->s->size; j++) {
				std::cout << node->s->configuration[i][j] << " ";
			}
			std::cout << std::endl;
		}
		if (p.goalTest(node->s))
		{
			solution = std::move(p.solution(node));
			qDebug() << "returning solution";
			return;
		}
		explored.insert(node->s);

		auto		actions =  p.actions(*(node->s));
		t_action	inverse_a = t_action::NONE;

		switch (node->a)
		{
		case t_action::UP:
			inverse_a = t_action::DOWN;
			break;

		case t_action::DOWN:
			inverse_a = t_action::UP;
			break;

		case t_action::LEFT:
			inverse_a = t_action::RIGHT;
			break;
		
		case t_action::RIGHT:
			inverse_a = t_action::LEFT;
			break;
		
		default:
			break;
		}

		for (int i = 0; i < actions.size(); i++)
		{
			if (inverse_a == actions[i])
			{
				actions.erase(actions.begin() + i);
				break ;
			}
		}
		// qDebug() << "available actions ";
		// for (int i = 0; i < actions.size(); i++) {
		// 	std::cout << actionToString(actions[i]) << " ";
		// }
		// std::cout << std::endl;
		for (const t_action& a: actions)
		{
			Node*	child = child_node(node, a);

			// qDebug() << "used action a " << actionToString(a).c_str() << " to produce child: ";
			// for (int i = 0; i < child->s->size; i++) {
			// 	for (int j = 0; j < child->s->size; j++) {
			// 		std::cout << child->s->configuration[i][j] << " ";
			// 	}
			// 	std::cout << std::endl;
			// }
			// qDebug() << "child has f_cost: " << this->criteria->f_val(child);
			
			std::pair<OpenSetNodeQueue::iterator, const Node*>
			existing = frontier.find(child);
			if (
				frontier.end() == existing.first &&
				std::end(explored) == explored.find(child->s)
			)
			{
				frontier.push(child);
			}
			else if (frontier.end() != existing.first)
			{
				// qDebug() << "already in frontier";
				const Node*	existing_node = existing.second;

				if (worse(existing_node, child))
				{
					// *(existing.first) = child;
					frontier.replace(existing.first, child);
				}
			}
			else
			{
				// qDebug() << "already in EXPLORED";
			}
		}
		qDebug() << "frontier: " << frontier.size() << " nodes";
		qDebug() << "explored: " << explored.size() << " states";
	}
	qDebug() << "NPuzzle::Agent::aStar -- END";
}
