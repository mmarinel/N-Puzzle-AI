/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Agent.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 21:13:01 by matteo            #+#    #+#             */
/*   Updated: 2024/05/26 11:16:32 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Agent.hpp"

NPuzzle::Agent::Agent(
		const std::vector<std::vector<Tile> >& config,
		const size_t size,
		uint8_t	x_empty,
		uint8_t	y_empty,
		t_heuristic h
)
: p{}, criteria(nullptr), solution{}, moves{0}
{
	// setting up Problem
		// initial state
	p.initial.configuration = config;
	p.initial.size = size;
	p.initial.i_empty = y_empty;
	p.initial.j_empty = x_empty;
	p.initial.cols.reserve(size);
	for (size_t j = 0; j < size; j++)
	{
		p.initial.cols[j] = 0;
		for (size_t i = 0; i < size; i++)
		{
			p.initial.cols[j] += (
				static_cast<uint64_t>(p.initial.configuration[i][j]) << (i*5)//(i*8)
			);
		}
	}
	
		// goal state
	setAsForwardGoal(p, p.goal, size);
	
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
				continue ;
			}
			frontier.push(child);
		}
	}
	qDebug() << "Frontier Empty";
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

void	NPuzzle::Agent::fillGrid(
	std::vector<std::vector<uint8_t>>& grid,
	int size,
	int offset,
	int nbr
)
{
	if (size <= 1)
		return ;

	int	i, j;

	//top row
	for (j = 0 + offset;			j < size + offset;	j++)
	{
		grid[0 + offset][j] = nbr++;
	}
		// Checking last element of the puzzle...(case N odd)-->in this case, after k iterations, the first element on the top row is the last one
	if (p.initial.size*p.initial.size == grid[0 + offset][0 + offset])
	{
		grid[0 + offset][0 + offset] = 0;
		p.x_empty_at_goal = 0 + offset;
		p.y_empty_at_goal = 0 + offset;
		return ;
	}
	// right column
	for (i = (0 + offset)			+ 1;			i < size + offset;	i++)
	{
		grid[i][size + offset - 1] = nbr++;
	}
	//bottom row
	for (j = (size + offset - 1)	- 1;		j >= (0 + offset);	j--)
	{
		grid[size + offset - 1][j] = nbr++;
	}
		// Checking last element of the puzzle...(case N even)-->-->in this case, after k iterations, the last element on the bottom row is the last one
	if (p.initial.size*p.initial.size == grid[size + offset - 1][j + 1])
	{
		grid[size + offset - 1][j + 1] = 0;
		p.x_empty_at_goal = j + 1;
		p.y_empty_at_goal = size + offset - 1;
		return ;
	}
	//left column
	for (i = (size + offset - 1)	- 1;		i >= (0 + offset)	+ 1;	i--)
	{
		grid[i][0 + offset] = nbr++;
	}
	fillGrid(grid, size - 2, offset + 1, nbr);
}

void	NPuzzle::Agent::setAsForwardGoal(
	Problem& p,
	std::map<uint8_t, std::pair<uint8_t, uint8_t>>& state,
	size_t size
)
{
	std::vector<std::vector<uint8_t>>	grid;

	grid.clear();
	grid.resize(size);
	for (size_t i = 0; i < size; i++) {
		grid[i].resize(size);
	}

	// filling as grid
	fillGrid(grid, size, 0, 1);
	
	// counting inversions at goal
	p.inversions_at_goal = 0;
	size_t	k, i, j;
		// counting inversion for the k-th element...
	for (k = 0; k < size*size; k++)
	{
		size_t		cur_i = k / size;
		size_t		cur_j = k % size;
		
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
	// qDebug() << "Inversions at goal: " << p.inversions_at_goal;


	// filling map
	for (size_t i = 0; i < size; i++) {
		for (size_t j = 0; j < size; j++) {
			state[grid[i][j]] = std::make_pair(i, j);
		}
	}
}

bool	NPuzzle::Agent::solvable(State* initial)
{
	int		_polarity = polarity(initial);
	if (0 != initial->size % 2)
		return p.inversions_at_goal % 2 == _polarity % 2;
	else
		return (
			(
				p.inversions_at_goal % 2 == _polarity % 2 &&
				-(p.y_empty_at_goal - initial->size) % 2  == (-(initial->i_empty - initial->size)) % 2
			) ||
			(
				p.inversions_at_goal % 2 != _polarity % 2 &&
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
				inversions += conf[current_i][current_j] > conf[i][j];
			}
			j = 0;
		}
	}
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
