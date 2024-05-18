/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Agent.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 21:13:01 by matteo            #+#    #+#             */
/*   Updated: 2024/05/18 16:47:14 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Agent.hpp"

#include <queue>
#include <vector>
#include <set>
#include <functional>

NPuzzle::Agent::Agent(
		const std::vector<std::vector<Tile> >& config,
		const size_t size,
		uint8_t	x_empty,
		uint8_t	y_empty,
		t_heuristic h
)
: p{}, solution{}, criteria(nullptr), moves{0}
{
	// setting up Problem(s)
		// initial state
	p.initial.configuration = config;
	p.initial.size = size;
	p.initial.i_empty = y_empty;
	p.initial.j_empty = x_empty;
	p.initial.affected_col = -1;
	p.initial.cols.reserve(size);
	qDebug() << "p.initial.col_sum";
	for (int j = 0; j < size; j++)
	{
		p.initial.cols[j] = 0;
		for (int i = 0; i < size; i++)
		{
			p.initial.cols[j] += (
				p.initial.configuration[i][j] << (i*8)
			);
			// qDebug() << "summing " << (p.initial.configuration[i][p.initial.initial_empty_col] << (i*8));
			// p.initial.col_sum += (
			// 	p.initial.configuration[i][p.initial.initial_empty_col] << (i*8)
			// );
		}
	}
	
	
		// goal state
	for (uint8_t i = 0; i < size; i++) {
		for (uint8_t j = 0; j < size; j++) {
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
	// this->aStar();
	this->idaStar();
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

void	NPuzzle::Agent::idaStar()
{
	// OpenSetNodeQueue		path{
	// 	[](const Node* n1, const Node* n2){return n1->pCost < n2->pCost;},
	// 	t_frontierNodesEquals{}
	// };
	std::stack<Node*>		path;
	ClosedSetNodeQueue		explored;
	Node*					initial = new Node{p};
	initial->s 				 	 	= new State{p.initial};
	initial->s->hCost 			 	= this->criteria->h(initial);
	int						bound 	= initial->s->hCost;
	t_idaStarIterResult		result;
	
	if (false == solvable(initial->s))
		return ;
	// qDebug() << "NPuzzle::Agent::idaStar() --- Solving...";
	path.push(initial);
	explored.insert(initial);
	// path.insert(initial);
	while (true)
	{
		// qDebug() << "before aStarDepthLimited(path, bound)";
		result = aStarDepthLimited(path, explored, bound);
		// qDebug() << "After aStarDepthLimited(path, bound)";
		if (result.solutionFound)
		{
			solution = std::move(p.solution(path.top()));
			qDebug() << "returning solution";
			// Node*	last = *(path.end()--);
			// solution = std::move(p.solution(path.top()));
			// solution = std::move(p.solution(last));
			moves = solution.size();
			qDebug() << "with " << moves << " moves";
			return;
		}
		bound = result.cutoff;
	}
}

NPuzzle::Agent::t_idaStarIterResult
NPuzzle::Agent::aStarDepthLimited(
				std::stack<Node*>& path,
				ClosedSetNodeQueue& explored,
				int bound
)
{
	Node*				node = path.top();
	int					f_val;
	int					min;
	t_idaStarIterResult	result;
	
	f_val = this->criteria->f_val(node);
	if (f_val > bound)
		return (t_idaStarIterResult){f_val, false};
	if (p.goalTest(node->s))
		return (t_idaStarIterResult){f_val, true};
	
	min = std::numeric_limits<int>::max();
	OpenSetNodeQueue	fringe{worse, t_frontierNodesEquals{}};
	// qDebug() << "Before filling fringe";
	for (const t_action& a: usefulActions(node,  p.actions(*(node->s))))
	{
		Node*	child = child_node(node, a);
		
		// auto	existing = path.find(child);
		child->s->hCost = this->criteria->h(child);
		if (explored.end() != explored.find(child)) //existing.first)
			continue ;
		fringe.push(child);
	}
	// qDebug() << "After filling fringe";
	for (Node* child: fringe)
	{
		path.push(child);
		explored.insert(child);
		// path.insert(child);
		result = aStarDepthLimited(path, explored, bound);
		if (result.solutionFound)
			return result;
		if (result.cutoff < min)
			min = result.cutoff;
		// qDebug() << "before deleting";
		path.pop();
		explored.erase(child);
		delete child;
	}
	return (t_idaStarIterResult){min, false};
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

	for (uint8_t i = 0; i < actions.size(); i++)
	{
		if (inverse_a == actions[i])
		{
			actions.erase(actions.begin() + i);
			break ;
		}
	}
	return actions;
}
