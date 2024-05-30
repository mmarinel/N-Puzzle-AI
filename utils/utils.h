/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cy4gate_mmarinelli <cy4gate_mmarinelli@    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 22:17:46 by matteo            #+#    #+#             */
/*   Updated: 2024/05/30 15:48:14 by cy4gate_mma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <QString>

#include <cstdlib>
#include <ctime>

#include "Problem.hpp"

namespace NPuzzle
{

const int	shuffle_moves = 42;//TODO move in config file

typedef struct s_RNG
{
	int	operator() (int n)
	{
		return std::rand() / (1.0f + RAND_MAX) * n;
	}
}	t_RNG;

bool	parse_file(QString filepath);
void	generate_board();

}

