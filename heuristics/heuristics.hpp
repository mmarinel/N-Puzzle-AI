/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heuristics.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 13:01:18 by matteo            #+#    #+#             */
/*   Updated: 2024/04/07 20:49:11 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

namespace NPuzzle
{
enum class t_heuristic
{
	MANHATTAN_DISTANCE,
	NONE
};

const std::string		to_string(t_heuristic h);
const t_heuristic		to_heuristicId(const std::string& name);
}
