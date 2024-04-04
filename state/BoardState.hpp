/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BoardState.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 20:27:38 by matteo            #+#    #+#             */
/*   Updated: 2024/04/04 21:03:14 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

class BoardState
{
public:
	int		size;
	int		x_empty;
	int		y_empty;
	int**	board;

private:
	BoardState();
	static BoardState*	state;
public:
	static BoardState&	getInstance();
						~BoardState();
						
						BoardState(const BoardState&)= delete;
	BoardState&			operator=(const BoardState&) = delete;
};