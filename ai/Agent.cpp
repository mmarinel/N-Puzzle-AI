/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Agent.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 21:13:01 by matteo            #+#    #+#             */
/*   Updated: 2024/05/20 22:15:33 by matteo           ###   ########.fr       */
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
	p.initial.cols.reserve(size);
	qDebug() << "p.initial.col_sum";
	for (int j = 0; j < size; j++)
	{
		p.initial.cols[j] = 0;
		for (int i = 0; i < size; i++)
		{
			qDebug() << "summing " << (static_cast<uint64_t>(p.initial.configuration[i][j]) << (i*5));
			p.initial.cols[j] += (
				static_cast<uint64_t>(p.initial.configuration[i][j]) << (i*5)//(i*8)
			);
			// qDebug() << "summing " << (p.initial.configuration[i][p.initial.initial_empty_col] << (i*8));
			// p.initial.col_sum += (
			// 	p.initial.configuration[i][p.initial.initial_empty_col] << (i*8)
			// );
		}
		qDebug() << "Final sum: " << p.initial.cols[j];
	}
	
	
		// goal state
	// for (uint8_t i = 0; i < size; i++) {
	// 	for (uint8_t j = 0; j < size; j++) {
	// 		p.goal[(i*size) + (j+1)] = std::make_pair(i, j);
	// 	}
	// }
	// p.goal.erase(size*size);
	// p.goal[0] = std::make_pair(size - 1, size - 1);
	setAsForwardGoal(p, p.goal, size);


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
	// worse = std::bind(
	// 	&NPuzzle::t_Iordering_func::cmp,
	// 	this->criteria,
	// 	std::placeholders::_1, std::placeholders::_2
	// );
	worse = [](const Node* n1, const Node* n2){
		return n1->f > n2->f;
	};
}

void	NPuzzle::Agent::run()
{
	// this->aStar();
	// this->idaStar();
	this->rbfs();
}
#include <iostream>
void	NPuzzle::Agent::aStar()
{
	OpenSetNodeQueue	frontier{worse, t_frontierNodesEquals{}};
	ClosedSetStateQueue	explored;
	Node*				node = new Node{p};
	node->s 				 = new State{p.initial};
	node->s->hCost 			 = this->criteria->h(node);
	node->f					 = 0 + node->s->hCost;
	
	if (false == solvable(node->s))
	{
		qDebug() << "This puzzle is not solvable";
		return ;
	}
	frontier.push(node);
	while (false == frontier.empty())
	{
		node = frontier.top();
		frontier.pop();
		if (explored.end() != explored.find(node->s))
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

			child->s->hCost = this->criteria->h(child);
			child->f = child->pCost + child->s->hCost;
			if (explored.end() != explored.find(child->s))
			{
				auto	it = explored.find(child->s);

				if ((*it)->configuration == child->s->configuration)
					continue;
				std::cout << "Found two equal states in explored" << std::endl;

				std::cout << "Found one" << std::endl;
				std::cout << "conf.size = " << (*it)->configuration.size() << std::endl;
				for (int i = 0; i < child->s->size; i++)
				{
					for (int j = 0; j < child->s->size; j++)
					{
						std::cout << static_cast<int>((*it)->configuration[i][j]) << " ";
					}
					std::cout << std::endl;
				}
				std::cout << "cols" << std::endl;
				for (int j = 0; j < child->s->size; j++)
				{
					std::cout << "col " << j << " " << (*it)->cols[j] << std::endl;
				}
				std::cout << std::endl;

				std::cout << "child" << std::endl;
				std::cout << "child conf.size = " << child->s->configuration.size() << std::endl;
				for (int i = 0; i < child->s->size; i++)
				{
					for (int j = 0; j < child->s->size; j++)
					{
						std::cout << static_cast<int>(child->s->configuration[i][j]) << " ";
					}
					std::cout << std::endl;
				}
				std::cout << "cols" << std::endl;
				for (int j = 0; j < child->s->size; j++)
				{
					std::cout << "col " << j << " " << child->s->cols[j] << std::endl;
				}
				std::cout << std::endl;
				
				continue ;
			}
			// qDebug() << "pushing to frontier...";
			frontier.push(child);
		}
	}
	qDebug() << "Frontier Empty";
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

void	NPuzzle::Agent::rbfs()
{
	Node*					initial = new Node{p};
	initial->s 				 	 	= new State{p.initial};
	initial->s->hCost 			 	= this->criteria->h(initial);
	initial->f						= 0 + initial->s->hCost;
	int						bound 	= std::numeric_limits<int>::max();// initial->s->hCost;
	t_rbfsIterResult		result;
	ClosedSetStateQueue		explored(t_exploredSet_cmp{});
	
	if (false == solvable(initial->s))
		return ;
	explored.insert(initial->s);
	result = rbfsRec(initial, explored, bound);
	solution = std::move(p.solution(result.leaf));
	qDebug() << "returning solution";
	moves = solution.size();
	qDebug() << "with " << moves << " moves";
}

NPuzzle::Agent::t_rbfsIterResult
NPuzzle::Agent::rbfsRec(
				Node* node,
				ClosedSetStateQueue& explored,
				int bound
)
{
	t_rbfsIterResult	result;
	
	if (p.goalTest(node->s))
		return (t_rbfsIterResult){node->f, true, node};
	
	OpenSetNodeQueue	fringe{worse, t_frontierNodesEquals{}};
	auto				actions = std::move(
		usefulActions(
			node,
			std::move( p.actions(*(node->s)) )
		)
	);
	for (const t_action& a: actions)
	{
		Node*	child = child_node(node, a);
		
		child->s->hCost = this->criteria->h(child);
		if (explored.end() != explored.find(child->s))
		{
			delete child;
		}
		else
		{
			child->f = std::max(
				child->pCost + child->s->hCost, node->f
			);
			fringe.push(child);
		}
	}
	if (fringe.empty())
		return (t_rbfsIterResult){
			std::numeric_limits<int>::max(), false, nullptr
		};
	while (true)
	{
		Node* best = fringe.top();
		
		fringe.pop();
		if (best->f > bound)
		{
			auto	cutoff = best->f;
			delete best;
			for (Node* leaf: fringe)
				delete leaf;
			return (t_rbfsIterResult){cutoff, false, nullptr};
		}
		explored.insert(best->s);
		Node*	alternative = fringe.top();
		result = rbfsRec(best, explored, std::min(bound, alternative->f));
		if (result.solutionFound)
			return result;
		best->f = result.cutoff;
		explored.erase(best->s);
		fringe.push(best);
	}
	return (t_rbfsIterResult){std::numeric_limits<int>::max(), false, nullptr};
}

void	fillGrid(
	std::vector<std::vector<int>>& grid,
	int size,
	size_t offset,
	int nbr
)
{
	if (size <= 1)
		return ;
	//top row
	for (int j = 0 + offset;			j < size + offset;	j++)
	{
		grid[0 + offset][j] = nbr++;
	}
	// right column
	for (int i = (0 + offset)			+ 1;			i < size + offset;	i++)
	{
		grid[i][size + offset - 1] = nbr++;
	}
	//bottom row
	for (int j = (size + offset - 1)	- 1;		j >= static_cast<int>(0 + offset);	j--)
	{
		grid[size + offset - 1][j] = nbr++;
	}
	//left column
	for (int i = (size + offset - 1)	- 1;		i >= static_cast<int>(0 + offset)	+ 1;	i--)
	{
		grid[i][0 + offset] = nbr++;
	}
	fillGrid(grid, size - 2, offset + 1, nbr);
}
#include <iostream>
void	NPuzzle::Agent::setAsForwardGoal(
	Problem& p,
	std::map<uint8_t, std::pair<uint8_t, uint8_t>>& state,
	size_t size
)
{
	qDebug() << "NPuzzle::Agent::setAsForwardGoal";
	std::vector<std::vector<int>>	grid;

	grid.clear();
	grid.resize(size);
	for (int i = 0; i < size; i++) {
		grid[i].resize(size);
		for (int j = 0; j < size; j++) {
			grid[i][j] = 0;
		}
	}

	// filling as grid
	fillGrid(grid, size, 0, 1);
	qDebug() << "SNAIL solution";
	for (int i = 0; i < static_cast<int>(size); i++) {
		for (int j = 0; j < static_cast<int>(size); j++) {
			if (static_cast<int>(size*size) == grid[i][j])
			{
				p.x_empty_at_goal = j;
				p.y_empty_at_goal = i;
				grid[i][j] = 0;
			}
			std::cout << static_cast<int>(grid[i][j]) << " ";
		}
		std::cout << std::endl;
	}
	
	// counting inversions at goal
	p.inversions_at_goal = 0;
	int	k, i, j;
		// counting inversion for the k-th element...
	for (k = 0; k < size*size; k++)
	{
		int		cur_i = k / size;
		int		cur_j = k % size;
		
		if (0 == grid[cur_i][cur_j])
			continue ;
		if (size - 1 == cur_j)
		{
			i = cur_i + 1;
			j = 0;
		}
		else
		{
			i = cur_i;
			j = cur_j + 1;
		}
		for (; i < size; i++)
		{
			for (; j < size; j++)
			{
				if (0 != grid[i][j])
					p.inversions_at_goal += grid[cur_i][cur_j] > grid[i][j];
			}
			j = 0;
		}
	}
	qDebug() << "Inversions at goal: " << p.inversions_at_goal;


	// filling map
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			state[grid[i][j]] = std::make_pair(i, j);
		}
	}
}

bool	NPuzzle::Agent::solvable(State* initial)
{
	if (0 != initial->size % 2)
		return p.inversions_at_goal % 2 == polarity(initial) % 2;
	else
		return (
			(
				p.inversions_at_goal % 2 == polarity(initial) % 2 &&
				-(p.y_empty_at_goal - initial->size) % 2  == (-(initial->i_empty - initial->size)) % 2
			) ||
			(
				p.inversions_at_goal % 2 != polarity(initial) % 2 &&
				(-(p.y_empty_at_goal - initial->size)) % 2 != (-(initial->i_empty - initial->size)) % 2
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
