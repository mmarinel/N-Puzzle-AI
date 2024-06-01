/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UIState.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 19:42:39 by matteo            #+#    #+#             */
/*   Updated: 2024/05/31 19:52:52 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <QString>
#include "heuristics.hpp"

typedef enum
{
	MENU,
	SOLVE
}	CurrentPage;

class UIState
{
public:
	CurrentPage					currentPage;
	QString						boardFileName;
	bool						atRandom;
	bool						atRandomSolvable;
	size_t						size;
	int							x_empty;
	int							y_empty;
	NPuzzle::t_search_strategy	search_strategy;
	NPuzzle::t_heuristic		h;

private:
	UIState();
public:
	static UIState&	getInstance();
						~UIState();
						
						UIState(const UIState&) = delete;
	UIState&			operator=(const UIState&) = delete;
	void				reset();
};
