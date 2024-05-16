/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AStar.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 21:13:01 by matteo            #+#    #+#             */
/*   Updated: 2024/05/16 22:40:03 by matteo           ###   ########.fr       */
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
: p{}, solution{}, criteria(nullptr), moves{0}
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

void	NPuzzle::Agent::aStar()
{
	OpenSetNodeQueue	frontier{worse, t_frontierNodesEquals{}};
	ClosedSetStateQueue	explored;
	Node*				node = new Node{p};
	node->s 				 = new State{p.initial};
	node->s->hCost 			 = this->criteria->h(node);
	
	if (false == solvable(node->s))
		return ;
	frontier.push(node);
	while (false == frontier.empty())
	{
		node = frontier.top();
		frontier.pop();
		if (std::end(explored) != explored.find(node->s))
			continue ;
		if (p.goalTest(node->s))
		{
			solution = std::move(p.solution(node));
			moves = solution.size();
			qDebug() << "returning solution";
			return;
		}
		explored.insert(node->s);

		for (const t_action& a: usefulActions(node,  p.actions(*(node->s))))
		{
			Node*	child = child_node(node, a);

			if (std::end(explored) != explored.find(child->s))
				continue ;
			frontier.push(child);
		}
	}
}

bool	NPuzzle::Agent::solvable(State* initial)
{
	if (0 != initial->size % 2)
		return 0 == polarity(initial) % 2;
	else
		return (
			(
				0 == polarity(initial) % 2 &&
				0 != (-(initial->i_empty - initial->size)) % 2
			) ||
			(
				0 != polarity(initial) % 2 &&
				0 == (-(initial->i_empty - initial->size)) % 2
			)
		);
}

int		NPuzzle::Agent::polarity(State* initial)
{
	const auto&		conf = initial->configuration;
	int				inversions = 0;
	int				firstNext_i;
	int				firstNext_j;
	int				current_i;
	int				current_j;
	int				k, i, j;

	for (k = 0; k < initial->size * initial->size; k++) {
		current_i = k / initial->size;
		current_j = k % initial->size;
		firstNext_i = (k + 1) / initial->size;
		firstNext_j = (k + 1) % initial->size;
		
		if (0 == conf[current_i][current_j])
			continue ;
		j = firstNext_j;
		for (i = firstNext_i; i < initial->size; i++) {
			for (; j < initial->size; j++) {
				if (0 == conf[i][j])
					continue ;
				qDebug() << "Checking "
					<< current_i << ", " << current_j
					<< " against "
					<< i << ", " << j;
				qDebug() << conf[current_i][current_j] << " > " << conf[i][j];
				inversions += conf[current_i][current_j] > conf[i][j];
			}
			j = 0;
		}
	}
	qDebug() << "n° inversions: " << inversions;
	return inversions;
}

const std::vector<t_action>
NPuzzle::Agent::usefulActions(
	const Node* node,
	std::vector<t_action>&& actions
)
{
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
	return actions;
}
