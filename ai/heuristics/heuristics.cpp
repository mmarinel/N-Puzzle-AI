/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heuristics.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmarinel <mmarinel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 15:40:52 by matteo            #+#    #+#             */
/*   Updated: 2024/06/02 22:45:55 by mmarinel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heuristics.hpp"

#include <algorithm>
#include <cmath>

uint8_t	NPuzzle::t_manhattan_score::operator()(const Node* n) const
{
	uint8_t						score;
	std::pair<uint8_t, uint8_t>	goalPosition;
	
	if (-1 != n->s->hManhattan)
		score = n->s->hManhattan;
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
			score = n->parent->s->hManhattan - oldScore_movedTile + newScore_movedTile;
		}
		const_cast<Node*>(n)->s->hManhattan = score;
		const_cast<Node*>(n)->s->hCost = n->s->hManhattan;
	}
	return score;
}

uint8_t	NPuzzle::t_linear_conflict_score::operator()(const Node* n) const
{
	int							score;
	int							adjustment;
	std::pair<uint8_t, uint8_t>	goalPosition;
	int							manhattan_score;
	
	if (-1 != n->s->hLinearConflict)
		score = n->s->hLinearConflict;
	else
	{
		if (nullptr == n->parent)
		{
			adjustment = 0;
			manhattan_score = 0;
			for (uint8_t i = 0; i < n->s->size; i++)
			{
				for (uint8_t j = 0; j < n->s->size; j++)
				{
					if (0 != n->s->configuration[i][j])
					{
						goalPosition = n->p.goal.at(n->s->configuration[i][j]);
						manhattan_score += (
							std::abs(i - goalPosition.first) +
							std::abs(j - goalPosition.second)
						);
						
						// Counting inversions...
						if (goalPosition.first == i)//Counting inversions on row
						{
							for (int kj = j + 1; kj < n->s->size; kj++)
							{
								if (0 == n->s->configuration[i][kj])
									continue ;
								int		goalRowforTile = n->p.goal.at(n->s->configuration[i][kj]).first;
								int		goalColforTile = n->p.goal.at(n->s->configuration[i][kj]).second;
								if (i != goalRowforTile)
									continue ;
								if (
									// (kj < j && goalColforTile > goalPosition.second ) ||
									(kj > j && goalColforTile < goalPosition.second )
								)
								{
									adjustment += 2;
									break;//I just need to know if I gotta move
										//(i.e.: if there is at least one conflict ahead)
								}
							}
						}
						if (goalPosition.second == j)//Counting inversions on column
						{
							for (int ki = i + 1; ki < n->s->size; ki++)
							{
								if (0 == n->s->configuration[ki][j])
									continue ;
								int		goalRowforTile = n->p.goal.at(n->s->configuration[ki][j]).first;
								int		goalColforTile = n->p.goal.at(n->s->configuration[ki][j]).second;
								if (j != goalColforTile)
									continue ;
								if (
									// (ki < i && goalRowforTile > goalPosition.first ) ||
									(ki > i && goalRowforTile < goalPosition.first )
								)
								{
									adjustment += 2;
									break;//I just need to know if I gotta move
										//(i.e.: if there is at least one conflict ahead)
								}
							}
						}
					}
				}
			}
			const_cast<Node*>(n)->s->hManhattan = manhattan_score;
		}
		else
		{
			// Adjusting linear conflict
				//old adjustment
			adjustment = n->parent->s->hLinearConflict - n->parent->s->hManhattan;
			goalPosition = n->p.goal.at(n->s->configuration[n->parent->s->i_empty][n->parent->s->j_empty]);
			int	goalCol = goalPosition.second;
			int	goalRow = goalPosition.first;
			int	movedTileJ;
			int	movedTileI;
			int	increment = 0;
			if (n->parent->s->j_empty != n->s->j_empty)//Horizontal Move
			{
				/**
				 * Horizontal adjustment for tile has not changed since moving one tile
				 * horizontally onto the blank doesn't change the relative order
				 * with respect to other tiles...
				 * 
				 * When moving horizontally/vertically, we only need to update the adjustment
				 * relative to column/rows.
				 */
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
				// STEP 1: I need to know how many tiles behind me need/needed to move to make place for me
				int	i;
				for (i = 0; i < movedTileI && increment != 0; i++)
				{
					if (0 == n->s->configuration[i][goalCol])
						continue ;
					int		goalRowforTile = n->p.goal.at(n->s->configuration[i][goalCol]).first;
					int		goalColforTile = n->p.goal.at(n->s->configuration[i][goalCol]).second;
					if (movedTileJ != goalColforTile)
						continue ;
					if (
						(i < movedTileI && goalRowforTile > goalPosition.first )// ||
						// (i > movedTileI && goalRowforTile < goalPosition.first )
					)
					{
						adjustment = adjustment + increment;
					}
				}
				// STEP 2: I need to know if I need/needed to move to make place for some tiles currently ahead of me
						//(i.e.: if there is at least one conflict ahead)
				for (; i < n->s->size && increment != 0; i++)
				{
					if (0 == n->s->configuration[i][goalCol])
						continue ;
					int		goalRowforTile = n->p.goal.at(n->s->configuration[i][goalCol]).first;
					int		goalColforTile = n->p.goal.at(n->s->configuration[i][goalCol]).second;
					if (movedTileJ != goalColforTile)
						continue ;
					if (
						// (i < movedTileI && goalRowforTile > goalPosition.first ) ||
						(i > movedTileI && goalRowforTile < goalPosition.first )
					)
					{
						adjustment = adjustment + increment;
						break;
					}
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
				// STEP 1: I need to know how many tiles behind me need/needed to move to make place for me
				int	j;
				for (j = 0; j < movedTileJ && increment != 0; j++)
				{
					if (0 == n->s->configuration[goalRow][j])
						continue ;
					int		goalColforTile = n->p.goal.at(n->s->configuration[goalRow][j]).second;
					int		goalRowforTile = n->p.goal.at(n->s->configuration[goalRow][j]).first;
					if (movedTileI != goalRowforTile)
						continue ;
					if (
						(j < movedTileJ && goalColforTile > goalPosition.second ) //||
						// (j > movedTileJ && goalColforTile < goalPosition.second )
					)
					{
						adjustment = adjustment + increment;
					}
				}
				// STEP 2: I need to know if I need/needed to move to make place for some tiles currently ahead of me
						//(i.e.: if there is at least one conflict ahead)
				for (; j < n->s->size && increment != 0; j++)
				{
					if (0 == n->s->configuration[goalRow][j])
						continue ;
					int		goalColforTile = n->p.goal.at(n->s->configuration[goalRow][j]).second;
					int		goalRowforTile = n->p.goal.at(n->s->configuration[goalRow][j]).first;
					if (movedTileI != goalRowforTile)
						continue ;
					if (
						// (j < movedTileJ && goalColforTile > goalPosition.second ) ||
						(j > movedTileJ && goalColforTile < goalPosition.second )
					)
					{
						adjustment = adjustment + increment;
						break;
					}
				}
			}
			manhattan_score = t_manhattan_score::getInstance()(n);
		}
		const_cast<Node*>(n)->s->hLinearConflict = manhattan_score + adjustment;
		const_cast<Node*>(n)->s->hCost = n->s->hLinearConflict;
		score = n->s->hLinearConflict;
	}
	return score;
}

uint8_t	NPuzzle::t_corner_tiles_score::operator()(const Node* n) const
{
	int		score;
	int		adjustment;

	if (-1 != n->s->hCornerTiles)
		score = n->s->hCornerTiles;
	else
	{
		auto	tileInTopLeft = n->s->configuration[0][0];
		auto	tileInTopLeftGoalPos = n->p.goal.at(tileInTopLeft);
		auto	tileInTopRight = n->s->configuration[0][n->s->size - 1];
		auto	tileInTopRightGoalPos = n->p.goal.at(tileInTopRight);
		auto	tileInBottomLeft = n->s->configuration[n->s->size - 1][0];
		auto	tileInBottomLeftGoalPos = n->p.goal.at(tileInBottomLeft);
		auto	tileInBottomRight = n->s->configuration[n->s->size - 1][n->s->size - 1];
		auto	tileInBottomRightGoalPos = n->p.goal.at(tileInBottomRight);
		
		score = 0;
		adjustment = 0;
		//top-left corner
		if (std::pair<uint8_t, uint8_t>(0, 0) != tileInTopLeftGoalPos)
		{
			if (
				std::pair<uint8_t, uint8_t>{0, 1} == n->p.goal.at(n->s->configuration[0][1]) &&
				0 != tileInTopLeftGoalPos.first
			)//next-to tile in next col cannot have row conflicts because is in its goal pos
			//and the only conflict that can have is if the tile behind was in its goal row but wrong column
			//since the tile behind is the corner tile, if it is in the right row it cannot be in its right column
			//since the corner tile is not in its goal position.
				adjustment += 2;
			if (
				std::pair<uint8_t, uint8_t>{1, 0} == n->p.goal.at(n->s->configuration[1][0]) &&
				0 != tileInTopLeftGoalPos.second
			)//next-to tile in next row cannot have row conflicts because is in its goal pos
				adjustment += 2;
		}
		//top-right corner
		if (std::pair<uint8_t, uint8_t>(0, n->s->size - 1) != tileInTopRightGoalPos)
		{
			if (
				std::pair<uint8_t, uint8_t>{0, (n->s->size - 1) - 1} == n->p.goal.at(n->s->configuration[0][(n->s->size - 1) - 1]) &&
				0 != tileInTopRightGoalPos.first
			)//next-to tile in next col cannot have row conflicts because is in its goal pos
				adjustment += 2;
			if (
				std::pair<uint8_t, uint8_t>{1, n->s->size - 1} == n->p.goal.at(n->s->configuration[1][n->s->size - 1]) &&
				n->s->size - 1 != tileInTopRightGoalPos.second
			)//next-to tile in next row cannot have row conflicts because is in its goal pos
				adjustment += 2;
		}
		//bottom-left corner
		if (std::pair<uint8_t, uint8_t>(n->s->size - 1, 0) != tileInBottomLeftGoalPos)
		{
			if (
				std::pair<uint8_t, uint8_t>{(n->s->size - 1) - 1, 0} == n->p.goal.at(n->s->configuration[(n->s->size - 1) - 1][0]) &&
				0 != tileInBottomLeftGoalPos.second
			)//next-to tile in next row cannot have row conflicts because is in its goal pos
				adjustment += 2;
			if (
				std::pair<uint8_t, uint8_t>{n->s->size - 1, 1} == n->p.goal.at(n->s->configuration[n->s->size - 1][1]) &&
				n->s->size - 1 != tileInBottomLeftGoalPos.first
			)//next-to tile in next col cannot have row conflicts because is in its goal pos
				adjustment += 2;
		}
		//bottom-right corner
		if (std::pair<uint8_t, uint8_t>(n->s->size - 1, n->s->size - 1) != tileInBottomRightGoalPos)
		{
			if (
				std::pair<uint8_t, uint8_t>{(n->s->size - 1) - 1, n->s->size - 1} == n->p.goal.at(n->s->configuration[(n->s->size - 1) - 1][n->s->size - 1]) &&
				n->s->size - 1 != tileInBottomRightGoalPos.second
			)//next-to tile in next row cannot have row conflicts because is in its goal pos
				adjustment += 2;
			if (
				std::pair<uint8_t, uint8_t>{n->s->size - 1, (n->s->size - 1) - 1} == n->p.goal.at(n->s->configuration[n->s->size - 1][(n->s->size - 1) - 1]) &&
				n->s->size - 1 != tileInBottomRightGoalPos.first
			)//next-to tile in next col cannot have row conflicts because is in its goal pos
				adjustment += 2;
		}

		score = t_linear_conflict_score::getInstance()(n) + adjustment;
		const_cast<Node*>(n)->s->hCornerTiles = score;
		const_cast<Node*>(n)->s->hCost = n->s->hCornerTiles;
	}
	return score;
}

uint8_t	NPuzzle::t_misplaced_tiles_score::operator()(const Node* n) const
{
	uint8_t						score = 0;
	std::pair<uint8_t, uint8_t>	goalPosition;
	
	if (-1 != n->s->hMisplacedTiles)
		score = n->s->hMisplacedTiles;
	else
	{
		if (nullptr == n->parent)
		{
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
		}
		else
		{
			int		misplaced_count;
			std::pair<uint8_t, uint8_t>
			previousMovedTilePos = {n->s->i_empty, n->s->j_empty};
			std::pair<uint8_t, uint8_t>
			newMovedTilePos = {n->parent->s->i_empty, n->parent->s->j_empty};
			auto	movedTile = n->s->configuration[newMovedTilePos.first][newMovedTilePos.second];

			misplaced_count = 0;
			if (
				previousMovedTilePos == n->p.goal.at(movedTile)
			)
				misplaced_count++;
			else if (
				newMovedTilePos == n->p.goal.at(movedTile)
			)
				misplaced_count--;
			
			score = n->parent->s->hMisplacedTiles + misplaced_count;
		}
		const_cast<Node*>(n)->s->hMisplacedTiles = score;
		const_cast<Node*>(n)->s->hCost = n->s->hMisplacedTiles;
	}
	return score;
}

uint8_t	NPuzzle::t_gaschnig_score::operator()(const Node* n) const
{
	int		score;

	if (-1 != n->s->hGaschnig)
		score = n->s->hGaschnig;
	else
	{
		auto	blank_goal_pos = n->p.goal.at(0);
		int		misplaced_count = t_misplaced_tiles_score::getInstance() (n);

		if (0 == misplaced_count)
			score = 0;
		else if (
			0 != n->s->configuration
			[blank_goal_pos.first]
			[blank_goal_pos.second]
		)
		{
			score = misplaced_count;
		}
		else
		{
			score = misplaced_count + 1;
		}

		const_cast<Node*>(n)->s->hGaschnig = score;
		const_cast<Node*>(n)->s->hCost = n->s->hGaschnig;
	}
	return score;
}

uint8_t	NPuzzle::t_coalesce_score::operator()(const Node* n) const
{
	int		score;

	if (-1 != n->s->hMax)
		score = n->s->hMax;
	else
	{
		int	_manhattan_score = t_manhattan_score::getInstance() (n);//TODO replace with corner tiles since it dominates
		int	_misplaced_tiles = t_misplaced_tiles_score::getInstance() (n);
		int	_gaschnig_score = t_gaschnig_score::getInstance() (n);

		score = (
			std::max(
				{
					_manhattan_score,
					_misplaced_tiles,
					_gaschnig_score
				}
			)
		);

		const_cast<Node*>(n)->s->hMax = score;
		const_cast<Node*>(n)->s->hCost = n->s->hMax;
	}
	return score;
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

const NPuzzle::t_corner_tiles_score&	NPuzzle::t_corner_tiles_score::getInstance()
{
	static NPuzzle::t_corner_tiles_score	i{};

	return i;
}

NPuzzle::t_corner_tiles_score::t_corner_tiles_score() {}

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
