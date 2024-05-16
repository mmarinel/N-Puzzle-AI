/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heuristics.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 15:40:52 by matteo            #+#    #+#             */
/*   Updated: 2024/05/16 21:58:13 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heuristics.hpp"

#include <algorithm>
#include <cmath>

int	NPuzzle::t_manhattan_score::operator()(const Node* n) const
{
	int					score;
	std::pair<int, int>	goalPosition;
	
	if (-1 != n->s->hCost)
		score = n->s->hCost;
	else
	{
		if (nullptr == n->parent)
		{
			score = 0;
			for (int i = 0; i < n->s->size; i++)
			{
				for (int j = 0; j < n->s->size; j++)
				{
					if (0 != n->s->configuration[i][j])
					{
						goalPosition = n->p.goal.at(n->s->configuration[i][j]);

						score += (
							std::abs(i - goalPosition.first) +
							std::abs(j - goalPosition.second)
						);
					}
				}
			}
		}
		else
		{
			int		x_movedTile = n->parent->s->j_empty;
			int		y_movedTile = n->parent->s->i_empty;
			int		movedTile = n->s->configuration[y_movedTile][x_movedTile];
					goalPosition = n->p.goal.at(n->s->configuration[y_movedTile][x_movedTile]);
			int		oldScore_movedTile;
			int		newScore_movedTile;

			oldScore_movedTile = (
				std::abs(n->s->i_empty - goalPosition.first) +
				std::abs(n->s->j_empty - goalPosition.second)
			);
			newScore_movedTile = (
				std::abs(y_movedTile - goalPosition.first) +
				std::abs(x_movedTile - goalPosition.second)
			);
			score = n->parent->s->hCost - oldScore_movedTile + newScore_movedTile;
		}
		const_cast<Node*>(n)->s->hCost = score;
	}
	return score;
}

int	NPuzzle::t_misplaced_tiles_score::operator()(const Node* n) const
{
	int					score = 0;
	std::pair<int, int>	goalPosition;
	
	for (int i = 0; i < n->s->size; i++)
	{
		for (int j = 0; j < n->s->size; j++)
		{
			if (0 == n->s->configuration[i][j])
				continue ;
			
			goalPosition = n->p.goal.at(n->s->configuration[i][j]);

			score += (
				goalPosition.first != i ||
				goalPosition.second != j
			);
		}
	}
	return score;
}

int	NPuzzle::t_gaschnig_score::operator()(const Node* n) const
{
	auto	blank_goal_pos = n->p.goal.at(0);
	
	if (
		0 != n->s->configuration
		[blank_goal_pos.first]
		[blank_goal_pos.second]
	)
	{
		return t_misplaced_tiles_score::getInstance() (n);
	}
	else
	{
		return t_misplaced_tiles_score::getInstance() (n) + 1;
	}
}

int	NPuzzle::t_coalesce_score::operator()(const Node* n) const
{
	int	_manhattan_score = t_manhattan_score::getInstance() (n);
	int	_misplaced_tiles = t_misplaced_tiles_score::getInstance() (n);
	int	_gaschnig_score = t_gaschnig_score::getInstance() (n);

	return (
		std::max(
			{
				_manhattan_score,
				_misplaced_tiles,
				_gaschnig_score
			}
		)
	);
}


//* Functors life cycle

const NPuzzle::t_manhattan_score&	NPuzzle::t_manhattan_score::getInstance()
{
	static NPuzzle::t_manhattan_score	i{};

	return i;
}

NPuzzle::t_manhattan_score::t_manhattan_score() {}

const NPuzzle::t_misplaced_tiles_score&	NPuzzle::t_misplaced_tiles_score::getInstance()
{
	static NPuzzle::t_misplaced_tiles_score	i{};

	return i;
}

NPuzzle::t_misplaced_tiles_score::t_misplaced_tiles_score() {}

const NPuzzle::t_gaschnig_score&	NPuzzle::t_gaschnig_score::getInstance()
{
	static NPuzzle::t_gaschnig_score	i{};

	return i;
}

NPuzzle::t_gaschnig_score::t_gaschnig_score() {}

const NPuzzle::t_coalesce_score&	NPuzzle::t_coalesce_score::getInstance()
{
	static NPuzzle::t_coalesce_score	i{};

	return i;
}

NPuzzle::t_coalesce_score::t_coalesce_score() {}
