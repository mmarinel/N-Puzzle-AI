/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cy4gate_mmarinelli <cy4gate_mmarinelli@    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 22:17:46 by matteo            #+#    #+#             */
/*   Updated: 2024/06/05 13:17:13 by cy4gate_mma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <QString>

#include <cstdlib>
#include <ctime>

#include "Problem.hpp"

namespace NPuzzle
{

const int	shuffle_moves = 420;//TODO move in UNIFIED GLOBAL config file

typedef struct s_RNG
{
	int	operator() (int n)
	{
		return std::rand() / (1.0f + RAND_MAX) * n;
	}
}	t_RNG;

bool	parse_file(QString filepath);
void	generate_board();


/**
 * @brief this function recursively fills a grid with the goal configuration (SNAIL solution).
 * It does its job by filling the external frame (up-bottom edge + left-right edge)
 * and then considering a smaller grid on the next iteration 
 * @param grid 
 * @param size size of current (sub)grid
 * @param offset offset from the up-bottom and side edges of current iteration (current sub-grid)
 * @param nbr the next tile number to put
 * @param nbr last tile number to put
 * @return (i,j) coordinates of empty tile
 */
std::pair<int, int>
fillGridAsGoal(
	State::t_configuration& grid,
	int size,
	int offset,
	int nbr,
	int last_nbr
);

}//END of NPuzzle namespace

