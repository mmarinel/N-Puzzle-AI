/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SearchStrategy.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 22:18:44 by matteo            #+#    #+#             */
/*   Updated: 2024/06/01 20:25:12 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SearchStrategy.hpp"

#include <QtGlobal>

#include <algorithm>
#include <limits>

NPuzzle::ISearchStrategy*
NPuzzle::ISearchStrategy::getStrategy(t_search_strategy strategy)
{
	
	switch (strategy)
	{
	case NPuzzle::t_search_strategy::INFORMED:
		return &RbfsInformedStrategy::getInstance();
	case NPuzzle::t_search_strategy::GREEDY:
		return &GreedyStrategy::getInstance();
	case NPuzzle::t_search_strategy::UNIFORM_COST:
		return &UniformCostStrategy::getInstance();
	default:
		break;
	}
	return nullptr;
}

void
NPuzzle::ISearchStrategy::setHeuristic(NPuzzle::t_Iordering_func* h)
{
	this->heuristic = h;
}

int
NPuzzle::RbfsInformedStrategy::score(Node* n)
{
	return n->f;
}

void
NPuzzle::RbfsInformedStrategy::setScore(Node* n)
{
	n->s->hCost = heuristic->h(n);
	n->f = std::max(
		n->pCost + n->s->hCost, nullptr == n->parent ? 0 : n->parent->f
	);
}

void
NPuzzle::RbfsInformedStrategy::updateScore(Node* n, int score, int cutoff_node_pCost)
{
	Q_UNUSED(cutoff_node_pCost);
	n->f = score;
}

bool
NPuzzle::RbfsInformedStrategy::cmp(const Node* n1, const Node* n2)
{
	return n1->f > n2->f;
}


int
NPuzzle::GreedyStrategy::score(Node* n)
{
	// return n->s->hCost;
	return n->f;
}

void
NPuzzle::GreedyStrategy::setScore(Node* n)
{
	// n->s->hCost = heuristic->h(n);
	// n->f = n->s->hCost;


	n->s->hCost = heuristic->h(n);
	if (
		nullptr == n->parent ||
		heuristic->h(n->parent) == n->parent->f
	)
	{
		n->f = n->s->hCost;
	}
	else
	{
		if (std::numeric_limits<int>::max() == n->parent->f)
			n->f = n->parent->f;
		else
			n->f = std::max(
				n->s->hCost, nullptr == n->parent ? 0 : n->parent->f - 1// 1 + n->parent->f ?
			);
	}
}

void
NPuzzle::GreedyStrategy::updateScore(Node* n, int score, int cutoff_node_pCost)
{Q_UNUSED(cutoff_node_pCost);

	if (std::numeric_limits<int>::max() == score)
		n->f = score;
	else
		n->f = std::max(
			n->f,
			(cutoff_node_pCost - n->pCost) + score
		);
}

bool
NPuzzle::GreedyStrategy::cmp(const Node* n1, const Node* n2)
{
	// return n1->s->hCost > n2->s->hCost;
	return n1->f > n2->f;
}


int
NPuzzle::UniformCostStrategy::score(Node* n)
{
	// return n->pCost;
	return n->f;
}

void
NPuzzle::UniformCostStrategy::setScore(Node* n)
{
	Q_UNUSED(n);
	n->f =  std::max(
		n->pCost,
		nullptr == n->parent ? 0 : n->parent->f
	);
	// g already set at Node creation (child node)
}

void
NPuzzle::UniformCostStrategy::updateScore(Node* n, int score, int cutoff_node_pCost)
{
	Q_UNUSED(n);
	Q_UNUSED(score);
	Q_UNUSED(cutoff_node_pCost);
	// n->pCost = score;
	n->f = score;
}

bool
NPuzzle::UniformCostStrategy::cmp(const Node* n1, const Node* n2)
{
	// return n1->pCost > n2->pCost;
	return n1->f > n2->f;
}


// Strategies Life-Cycle

NPuzzle::RbfsInformedStrategy::RbfsInformedStrategy() {}

NPuzzle::RbfsInformedStrategy&
NPuzzle::RbfsInformedStrategy::getInstance()
{
	static NPuzzle::RbfsInformedStrategy	i{};

	return i;
}

NPuzzle::GreedyStrategy::GreedyStrategy() {}

NPuzzle::GreedyStrategy&
NPuzzle::GreedyStrategy::getInstance()
{
	static NPuzzle::GreedyStrategy	i{};

	return i;
}

NPuzzle::UniformCostStrategy::UniformCostStrategy() {}

NPuzzle::UniformCostStrategy&
NPuzzle::UniformCostStrategy::getInstance()
{
	static NPuzzle::UniformCostStrategy	i{};

	return i;
}
