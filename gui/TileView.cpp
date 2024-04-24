/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TileView.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 20:08:42 by matteo            #+#    #+#             */
/*   Updated: 2024/04/24 23:44:17 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TileView.hpp"

TileView::TileView(QWidget* parent): QLabel(parent)
{
	this->setFrameStyle(QFrame::Box | QFrame::Sunken);
	this->setLineWidth(3);
	this->setMidLineWidth(3);
	this->setFixedWidth(50);
}

TileView::~TileView() {}

bool	TileView::hasHeightForWidth() const
{
	return true;
}

int		TileView::heightForWidth(int w) const
{
	return w;
}

void	TileView::resizeEvent(QResizeEvent* event)
{
	double	aspect_ratio = (
		static_cast<double>(this->height())
		/
		static_cast<double>(this->width())
	);

	if (1 != aspect_ratio)
		this->setMaximumHeight(
			static_cast<int>(this->width())
		);
}
