/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Node.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 10:14:26 by matteo            #+#    #+#             */
/*   Updated: 2024/05/16 22:24:47 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Node.hpp"

Node::Node(const Problem& p): p(p)
{
	this->s = nullptr;
	this->parent = nullptr;
	this->a = t_action::NONE;
	this->pCost = 0;
}

Node*  NPuzzle::child_node(Node* parent, t_action a)
{
	Node*	child = new Node{parent->p};

	child->s = child->p.result(*(parent->s), a);
	child->parent = parent;
	child->a = a;
	child->pCost = parent->pCost + 1;

	return child;
}
