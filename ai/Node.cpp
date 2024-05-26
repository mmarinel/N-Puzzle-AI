/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Node.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 10:14:26 by matteo            #+#    #+#             */
/*   Updated: 2024/05/26 17:46:51 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Node.hpp"

unsigned long long		Node::instances = 0;
unsigned long long		Node::max_instances = 0;

Node::Node(const Problem& p): p(p)
{
	this->s = nullptr;
	this->parent = nullptr;
	this->a = t_action::NONE;
	this->pCost = 0;

	Node::instances += 1;
	if (Node::instances > Node::max_instances)
		Node::max_instances = Node::instances;
}

Node::~Node()
{
	Node::instances -= 1;
	delete this->s;
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
