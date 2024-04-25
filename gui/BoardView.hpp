/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BoardView.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 18:38:46 by matteo            #+#    #+#             */
/*   Updated: 2024/04/25 23:57:25 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <QScrollArea>
#include <QGridLayout>
#include <QWidget>
#include <QLabel>

#include <string>

#include "BoardState.hpp"
#include "UIState.hpp"
#include "TileView.hpp"

#define TILE_OBJ_NAME "Tile"

class BoardView: public QWidget
{
	Q_OBJECT
private:
	BoardState&			status;
	UIState&			ui_status;
	QGridLayout*		grid;

	void	reset();
public:
	BoardView(QWidget* parent = nullptr);
	~BoardView();

	void			setup();
	virtual void	paintEvent(QPaintEvent* event) override;
};
