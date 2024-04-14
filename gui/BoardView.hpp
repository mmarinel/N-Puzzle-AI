/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BoardView.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 18:38:46 by matteo            #+#    #+#             */
/*   Updated: 2024/04/14 11:20:06 by matteo           ###   ########.fr       */
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
public:
	QGridLayout*		grid;//TODO rimettere private
	BoardView();
	~BoardView();

	virtual void	paintEvent(QPaintEvent* event) override;
protected:
	void	resizeEvent(QResizeEvent* event);
};
