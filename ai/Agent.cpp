/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Agent.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cy4gate_mmarinelli <cy4gate_mmarinelli@    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 21:13:01 by matteo            #+#    #+#             */
/*   Updated: 2024/06/05 10:22:59 by cy4gate_mma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Agent.hpp"
#include "utils.h"

#include "UIState.hpp"
#include "BoardState.hpp"

#include <memory>

template <>
unsigned long long		NPuzzle::Agent::OpenSetNodeQueue::nbr_selected = 0;

NPuzzle::Agent::Agent(): p{}, criteria(nullptr), solution{}, moves{0}
{
	size_t	size = BoardState::getInstance().size;
	
	// setting up Problem
		// initial state
	p.initial.configuration = BoardState::getInstance().board;
	p.initial.size = size;
	p.initial.i_empty = BoardState::getInstance().y_empty;
	p.initial.j_empty = BoardState::getInstance().x_empty;
		// compatct representation of state
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
	this->criteria = NPuzzle::hToFunc.at(UIState::getInstance().h);
	worse = std::bind(
		&t_Iordering_func::cmp,
		this->criteria,
		std::placeholders::_1, std::placeholders::_2
	);
}

NPuzzle::Agent::~Agent()
{
	//TODO ...possible cleanup function to call ?
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
		// qDebug() << "This puzzle is not solvable";
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
			// qDebug() << "returning solution";
			emit workDone();
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
	// qDebug() << "Frontier Empty";
}

void	NPuzzle::Agent::rbfs()
{
	std::unique_ptr<Node>			initial{new Node{p}};
	initial->s 						= new State{p.initial};
	initial->s->hCost 				= this->criteria->h(initial.get());
	initial->f						= 0 + initial->s->hCost;
	int						bound	= std::numeric_limits<int>::max();// initial->s->hCost;
	t_rbfsIterResult		result;
	ClosedSetStateQueue		explored(t_exploredSet_cmp{});
	
	if (solvable(initial->s))
	{
		explored.insert(initial->s);
		result = rbfsRec(initial.get(), explored, bound);
		if (result.failure)
			return ;
		solution = std::move(result.actions);
		// qDebug() << "returning solution";
		moves = solution.size();
		// qDebug() << "with " << moves << " moves";
	}
	emit workDone();
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
		return (t_rbfsIterResult){node->f, true, std::move(p.solution(node)), false};
	
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
			std::numeric_limits<int>::max(), false, Problem::Actions{}, false
		};
	std::unique_ptr<Node>	best;
	while (true)
	{
		// TODO valutare se spostarlo nel successivo if perché calcolarlo ad ogni iterazione è costoso
		// TODO (anche se più sicuro)
		if (this->isInterruptionRequested())
			return (t_rbfsIterResult){bound, false, Problem::Actions{}, true};
		best.reset(fringe.top());
		fringe.pop();
		
		if (best->f > bound)
		{
			auto	cutoff = best->f;
			return (t_rbfsIterResult){cutoff, false, Problem::Actions{}, false};
		}
		explored.insert(best->s);
		Node*	alternative = fringe.top();
		result = rbfsRec(best.get(), explored, std::min(bound, alternative->f));
		if (result.solutionFound)
			return result;
		best->f = result.cutoff;
		explored.erase(best->s);
		fringe.push(best.get());
		best.release();
	}
	return (t_rbfsIterResult){std::numeric_limits<int>::max(), false, Problem::Actions{}, false};
}

bool	NPuzzle::Agent::solvable(State* initial)
{
	auto
	_inversions = Problem::polarity(initial->configuration, initial->size);
	auto
		size = initial->size;
	int
		blankAtGoal_row_bottom_based = size - p.y_empty_at_goal;//1-indexing
	int
		blankAtInitial_row_bottom_based = size - initial->i_empty;
	
	p.inversions_at_initial = _inversions.first;
	p.polarity_at_initial = _inversions.second;
	
	if (0 != initial->size % 2)
		return p.polarity_at_goal == p.polarity_at_initial;
	else
		return (
			(
				p.polarity_at_goal == p.polarity_at_initial &&
				blankAtGoal_row_bottom_based % 2  == blankAtInitial_row_bottom_based % 2
			) ||
			(
				p.polarity_at_goal != p.polarity_at_initial &&
				blankAtGoal_row_bottom_based % 2 != blankAtInitial_row_bottom_based % 2
			)
		);
}




/* ******************************************** */
/*                                              */
/*            Secondary functions               */
/*                                              */
/* ******************************************** */
void	NPuzzle::Agent::setAsForwardGoal(
	Problem& p,
	std::map<int, std::pair<int, int>>& state,
	size_t size
)
{
	State::t_configuration	grid;

	grid.clear();
	grid.resize(size);
	for (size_t i = 0; i < size; i++) {
		grid[i].resize(size);
	}

	// filling as grid
	auto	blank_pos = NPuzzle::fillGridAsGoal(
		grid, size, 0, 1, p.initial.size*p.initial.size
	);
	p.y_empty_at_goal = blank_pos.first;
	p.x_empty_at_goal = blank_pos.second;
	
	// counting inversions at goal
	auto	_inversions = Problem::polarity(grid, size);
	p.inversions_at_goal = _inversions.first;
	p.polarity_at_goal = _inversions.second;
	// qDebug() << "Inversions at goal: " << p.inversions_at_goal;

	// filling map
	for (size_t i = 0; i < size; i++) {
		for (size_t j = 0; j < size; j++) {
			state[grid[i][j]] = std::make_pair(i, j);
		}
	}
}

const std::vector<t_action>
NPuzzle::Agent::usefulActions(
	const Node* node,
	std::vector<t_action>&& actions
)
{
	t_action	inverse_a = Problem::inverseAction(node->a);

	for (size_t i = 0; i < actions.size(); i++)
	{
		if (inverse_a == actions[i])
		{
			actions.erase(actions.begin() + i);
			break ;
		}
	}
	return actions;
}

const Problem&		NPuzzle::Agent::problem()
{
	return p;
}
