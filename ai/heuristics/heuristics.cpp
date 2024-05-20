/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heuristics.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 15:40:52 by matteo            #+#    #+#             */
/*   Updated: 2024/05/20 21:53:22 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heuristics.hpp"

#include <algorithm>
#include <cmath>

uint8_t	NPuzzle::t_manhattan_score::operator()(const Node* n) const
{
	uint8_t					score;
	std::pair<uint8_t, uint8_t>	goalPosition;
	
	if (-1 != n->s->hCost)
		score = n->s->hCost;
	else
	{
		if (nullptr == n->parent)
		{
			score = 0;
			for (uint8_t i = 0; i < n->s->size; i++)
			{
				for (uint8_t j = 0; j < n->s->size; j++)
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
			uint8_t		x_movedTile = n->parent->s->j_empty;
			uint8_t		y_movedTile = n->parent->s->i_empty;
			uint8_t		movedTile = n->s->configuration[y_movedTile][x_movedTile];
						goalPosition = n->p.goal.at(movedTile);
			uint8_t		oldScore_movedTile;
			uint8_t		newScore_movedTile;

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

uint8_t	NPuzzle::t_linear_conflict_score::operator()(const Node* n) const
{
	int					score;
	std::pair<uint8_t, uint8_t>	goalPosition;
	
	if (-1 != n->s->hCost)
		score = n->s->hCost;
	else
	{
		if (nullptr == n->parent)
		{
			score = 0;
			for (uint8_t i = 0; i < n->s->size; i++)
			{
				for (uint8_t j = 0; j < n->s->size; j++)
				{
					if (0 != n->s->configuration[i][j])
					{
						goalPosition = n->p.goal.at(n->s->configuration[i][j]);

						score += (
							std::abs(i - goalPosition.first) +
							std::abs(j - goalPosition.second)
						);
						
						// Counting inversions...
						if (goalPosition.first == i)//Counting inversions on row
						{
							for (int kj = 0; kj < n->s->size; kj++)
							{
								int		goalRowforTile = n->p.goal.at(n->s->configuration[i][kj]).first;
								int		goalColforTile = n->p.goal.at(n->s->configuration[i][kj]).second;
								if (i != goalRowforTile)
									continue ;
								if (
									(kj < j && goalColforTile > goalPosition.second ) ||
									(kj > j && goalColforTile < goalPosition.second )
								)
									score += 1;
							}
						}
						else if (goalPosition.second == j)//Counting inversions on column
						{
							for (int ki = 0; ki < n->s->size; ki++)
							{
								int		goalRowforTile = n->p.goal.at(n->s->configuration[ki][j]).first;
								int		goalColforTile = n->p.goal.at(n->s->configuration[ki][j]).second;
								if (j != goalColforTile)
									continue ;
								if (
									(ki < i && goalRowforTile > goalPosition.first ) ||
									(ki > i && goalRowforTile < goalPosition.first )
								)
									score += 1;
							}
						}
					}
				}
			}
		}
		else
		{
			uint8_t		x_movedTile = n->parent->s->j_empty;
			uint8_t		y_movedTile = n->parent->s->i_empty;
			uint8_t		movedTile = n->s->configuration[y_movedTile][x_movedTile];
						goalPosition = n->p.goal.at(movedTile);
			uint8_t		oldScore_movedTile;
			uint8_t		newScore_movedTile;

			oldScore_movedTile = (
				std::abs(n->s->i_empty - goalPosition.first) +
				std::abs(n->s->j_empty - goalPosition.second)
			);
			newScore_movedTile = (
				std::abs(y_movedTile - goalPosition.first) +
				std::abs(x_movedTile - goalPosition.second)
			);
			score = n->parent->s->hCost - oldScore_movedTile + newScore_movedTile;

			// Adjusting linear conflict
			int	adjustment = 0;
			int	goalCol = goalPosition.second;
			int	goalRow = goalPosition.first;
			int	movedTileJ;
			int	movedTileI;
			int	increment = 0;
			if (n->parent->s->j_empty != n->s->j_empty)//Horizontal Move
			{
				if (n->parent->s->j_empty == goalCol)//Adding conflicts
				{
					movedTileJ = n->parent->s->j_empty;
					movedTileI = n->parent->s->i_empty;
					increment = +2;
				}
				else if (n->s->j_empty == goalCol)//Removing conflicts
				{
					movedTileJ = n->s->j_empty;
					movedTileI = n->s->i_empty;
					increment = -2;
				}
				for (int i = 0; i < n->s->size && increment != 0; i++)
				{
					int		goalRowforTile = n->p.goal.at(n->s->configuration[i][goalCol]).first;
					int		goalColforTile = n->p.goal.at(n->s->configuration[i][goalCol]).second;
					if (movedTileJ != goalColforTile)
						continue ;
					if (
						(i < movedTileI && goalRowforTile > goalPosition.first ) ||
						(i > movedTileI && goalRowforTile < goalPosition.first )
					)
						adjustment = adjustment + increment;
				}
			}
			else//vertical move
			{
				if (n->parent->s->i_empty == goalRow)//Adding conflicts
				{
					movedTileJ = n->parent->s->j_empty;
					movedTileI = n->parent->s->i_empty;
					increment = +2;
				}
				else if (n->s->i_empty == goalRow)//Removing conflicts
				{
					movedTileJ = n->s->j_empty;
					movedTileI = n->s->i_empty;
					increment = -2;
				}
				for (int j = 0; j < n->s->size && increment != 0; j++)
				{
					int		goalColforTile = n->p.goal.at(n->s->configuration[goalRow][j]).second;
					int		goalRowforTile = n->p.goal.at(n->s->configuration[goalRow][j]).first;
					if (movedTileI != goalRowforTile)
						continue ;
					if (
						(j < movedTileJ && goalColforTile > goalPosition.second ) ||
						(j > movedTileJ && goalColforTile < goalPosition.second )
					)
						adjustment = adjustment + increment;
				}
			}
			score += adjustment;
		}
		const_cast<Node*>(n)->s->hCost = score;
	}
	return score;
}

uint8_t	NPuzzle::t_misplaced_tiles_score::operator()(const Node* n) const
{
	uint8_t					score = 0;
	std::pair<uint8_t, uint8_t>	goalPosition;
	
	for (uint8_t i = 0; i < n->s->size; i++)
	{
		for (uint8_t j = 0; j < n->s->size; j++)
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

uint8_t	NPuzzle::t_gaschnig_score::operator()(const Node* n) const
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

uint8_t	NPuzzle::t_coalesce_score::operator()(const Node* n) const
{
	uint8_t	_manhattan_score = t_manhattan_score::getInstance() (n);
	uint8_t	_misplaced_tiles = t_misplaced_tiles_score::getInstance() (n);
	uint8_t	_gaschnig_score = t_gaschnig_score::getInstance() (n);

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

const NPuzzle::t_linear_conflict_score&	NPuzzle::t_linear_conflict_score::getInstance()
{
	static NPuzzle::t_linear_conflict_score	i{};

	return i;
}

NPuzzle::t_linear_conflict_score::t_linear_conflict_score() {}

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
