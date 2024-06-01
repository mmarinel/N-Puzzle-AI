/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Agent.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 21:13:01 by matteo            #+#    #+#             */
/*   Updated: 2024/06/01 18:10:53 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Agent.hpp"
#include "utils.h"
#include "UIState.hpp"
#include "BoardState.hpp"

#include <memory>

template <>
unsigned long long		NPuzzle::Agent::OpenSetNodeQueue::nbr_selected = 0;

NPuzzle::Agent::Agent()
: p{}, criteria(nullptr), solution{}, moves{0}
{
	// setting up Problem
		// initial state
	p.initial.configuration = BoardState::getInstance().board;
	p.initial.size = BoardState::getInstance().size;
	p.initial.i_empty = BoardState::getInstance().y_empty;
	p.initial.j_empty = BoardState::getInstance().x_empty;
	p.initial.cols.reserve(BoardState::getInstance().size);
	for (int j = 0; j < BoardState::getInstance().size; j++)
	{
		p.initial.cols[j] = 0;
		for (int i = 0; i < BoardState::getInstance().size; i++)
		{
			p.initial.cols[j] += (
				static_cast<uint64_t>(p.initial.configuration[i][j]) << (i*5)//(i*8)
			);
		}
	}
	
		// goal state
	setAsForwardGoal(p, p.goal, BoardState::getInstance().size);
	
	// setting A* specific politics
	this->criteria = (
		NPuzzle::ISearchStrategy::getStrategy(
			UIState::getInstance().search_strategy
		)
	);
	this->criteria->setHeuristic(
		const_cast<NPuzzle::t_Iordering_func*>(
			NPuzzle::hToFunc.at(UIState::getInstance().h)
		)
	);
	worse = std::bind(
		&NPuzzle::ISearchStrategy::cmp,
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
	this->criteria->setScore(node);
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
			emit workDone();
			return;
		}
		explored.insert(node->s);
		for (const t_action& a: usefulActions(node,  p.actions(*(node->s))))
		{
			Node*	child = child_node(node, a);

			this->criteria->setScore(child);
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
	std::unique_ptr<Node>			initial{new Node{p}};
	initial->s 						= new State{p.initial};
	int						bound	= std::numeric_limits<int>::max();// initial->s->hCost;
	t_rbfsIterResult		result;
	ClosedSetStateQueue		explored(t_exploredSet_cmp{});
	
	this->criteria->setScore(initial.get());
	if (solvable(initial->s))
	{
		explored.insert(initial->s);
		result = rbfsRec(initial.get(), explored, bound);
		if (result.failure)
			return ;
		solution = std::move(result.actions);
		qDebug() << "returning solution";
		moves = solution.size();
		qDebug() << "with " << moves << " moves";
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
		return (t_rbfsIterResult){
			(t_cutoff){this->criteria->score(node), node->pCost},
			true, std::move(p.solution(node)), false
		};
	
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
		
		this->criteria->setScore(child);
		if (explored.end() != explored.find(child->s))
		{
			delete child;
		}
		else
		{
			fringe.push(child);
		}
	}
	if (fringe.empty())
		return (t_rbfsIterResult){
			(t_cutoff){std::numeric_limits<int>::max(), node->pCost},
			false, Problem::Actions{}, false
		};
	std::unique_ptr<Node>	best;
	while (true)
	{
		// TODO valutare se spostarlo nel successivo if perché calcolarlo ad ogni iterazione è costoso
		// TODO (anche se più sicuro)
		if (this->isInterruptionRequested())
			return (t_rbfsIterResult){
				(t_cutoff){bound, node->pCost},
				false, Problem::Actions{}, true
			};
		best.reset(fringe.top());
		fringe.pop();
		
		if (this->criteria->score(best.get()) > bound)
		{
			auto	cutoff_val = this->criteria->score(best.get());
			return (t_rbfsIterResult){
				(t_cutoff){cutoff_val, best->pCost},
				false, Problem::Actions{}, false
			};
		}
		explored.insert(best->s);
		Node*	alternative = fringe.top();
		result = rbfsRec(best.get(), explored, std::min(bound, this->criteria->score(alternative)));
		if (result.solutionFound)
			return result;
		// this->criteria->score(best) = result.cutoff;//TODO update
		this->criteria->updateScore(
			best.get(),
			result.cutoff.cutoff, result.cutoff.cutoff_node_pCost
		);
		explored.erase(best->s);
		fringe.push(best.get());
		best.release();
	}
	return (t_rbfsIterResult){
		(t_cutoff){std::numeric_limits<int>::max(), node->pCost},
		false, Problem::Actions{}, false
	};
}

bool	NPuzzle::Agent::solvable(State* initial)
{
	auto
	_inversions = Problem::polarity(initial->configuration, initial->size);
	auto
		size = initial->size;
	uint8_t
		blankAtGoal_row_bottom_based = size - p.y_empty_at_goal;//1-indexing
	uint8_t
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
	std::map<uint8_t, std::pair<uint8_t, uint8_t>>& state,
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
	qDebug() << "Inversions at goal: " << p.inversions_at_goal;

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

const Problem&		NPuzzle::Agent::problem()
{
	return p;
}
