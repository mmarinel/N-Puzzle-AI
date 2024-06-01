/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SearchStrategy.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 22:06:26 by matteo            #+#    #+#             */
/*   Updated: 2024/06/01 01:47:14 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "heuristics.hpp"
#include "Node.hpp"

namespace NPuzzle
{

class ISearchStrategy
{
protected:
	const NPuzzle::t_Iordering_func*		heuristic;
public:
	static ISearchStrategy*		getStrategy(t_search_strategy strategy);

	virtual int		score(Node* n) = 0;
	virtual void	setScore(Node* n) = 0;
	virtual void	updateScore(Node* n, int score, int cutoff_node_pCost) = 0;
			void	setHeuristic(NPuzzle::t_Iordering_func* h);
	virtual bool	cmp(const Node*, const Node*) = 0;
};

class	RbfsInformedStrategy: public ISearchStrategy
{
private:
	RbfsInformedStrategy();
public:
	static RbfsInformedStrategy&	getInstance();
	int	score(Node* n) override;
	void	setScore(Node* n) override;
	void	updateScore(Node* n, int score, int cutoff_node_pCost) override;
	bool	cmp(const Node*, const Node*) override;
};

class	GreedyStrategy: public ISearchStrategy
{
private:
	GreedyStrategy();
public:
	static GreedyStrategy&	getInstance();
	int		score(Node* n) override;
	void	setScore(Node* n) override;
	void	updateScore(Node* n, int score, int cutoff_node_pCost) override;
	bool	cmp(const Node*, const Node*) override;
};

class	UniformCostStrategy: public ISearchStrategy
{
private:
	UniformCostStrategy();
public:
	static UniformCostStrategy&	getInstance();
	int		score(Node* n) override;
	void	setScore(Node* n) override;
	void	updateScore(Node* n, int score, int cutoff_node_pCost) override;
	bool	cmp(const Node*, const Node*) override;
};

}//End of NPuzzle namespace
