/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Node.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 10:06:05 by matteo            #+#    #+#             */
/*   Updated: 2024/05/17 19:27:21 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Problem.hpp"

class Node
{
public:
	const Problem&	p;
	State*			s;
	Node*			parent;
	t_action		a;
	int				pCost;

			/**
			 * @brief Creates a root node without state.
			 * To get the initial state, assign just the s pointer
			 * 
			 */
			Node(const Problem& p);
			~Node();
			Node(const Node& n) = delete;
	Node&	operator=(const Node& n) = delete;
			Node(Node&& n) = delete;
};

namespace NPuzzle
{
Node* child_node(Node* parent, t_action a);
}

