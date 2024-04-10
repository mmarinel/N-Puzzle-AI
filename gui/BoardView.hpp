/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BoardView.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 18:38:46 by matteo            #+#    #+#             */
/*   Updated: 2024/04/10 19:06:59 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <QGridLayout>
#include <QWidget>
#include <QLabel>

#include <string>

#include "BoardState.hpp"

#define TILE_OBJ_NAME "Tile"

class BoardView: public QWidget
{
	Q_OBJECT
private:
	const BoardState&	status;
	QGridLayout*		grid;
public:
	BoardView();
	~BoardView();

	virtual void	paintEvent(QPaintEvent* event) override;
};
