/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heuristics.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 15:40:52 by matteo            #+#    #+#             */
/*   Updated: 2024/04/07 22:13:46 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heuristics.hpp"


const std::string		NPuzzle::to_string(t_heuristic h)
{
	switch (h)
	{
	case t_heuristic::MANHATTAN_DISTANCE:
		return "Manhattan Distance";
		
	default:
		return "";
		break;
	}
}

const NPuzzle::t_heuristic	NPuzzle::to_heuristicId(const std::string& name)
{
	if ("Manhattan Distance" == name)
		return NPuzzle::t_heuristic::MANHATTAN_DISTANCE;
	else
		return NPuzzle::t_heuristic::NONE;
}
