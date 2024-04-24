/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BoardView.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 18:38:46 by matteo            #+#    #+#             */
/*   Updated: 2024/04/24 23:42:34 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <QScrollArea>
#include <QGridLayout>
#include <QWidget>
#include <QLabel>

#include <string>

#include "BoardState.hpp"
#include "TileView.hpp"

#define TILE_OBJ_NAME "Tile"

class BoardView: public QWidget
{
	Q_OBJECT
private:
	const BoardState&	status;
	QGridLayout*		grid;

	void	reset();
public:
	BoardView(QWidget* parent = nullptr);
	~BoardView();

	void			setup();
	virtual void	paintEvent(QPaintEvent* event) override;
};
