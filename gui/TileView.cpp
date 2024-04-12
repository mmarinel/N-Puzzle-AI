/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TileView.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 20:08:42 by matteo            #+#    #+#             */
/*   Updated: 2024/04/11 20:40:33 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TileView.hpp"

TileView::TileView(): QLabel()
{
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

// void	TileView::resizeEvent(QResizeEvent* event)
// {
// 	double	aspect_ratio = (
// 		static_cast<double>(this->height())
// 		/
// 		static_cast<double>(this->width())
// 	);
// 	int		target_width;

// 	if (aspect_ratio < 1) // Too wide
// 	{
// 		this->setMaximumWidth(
// 			static_cast<int>(this->height())
// 		);
// 	}
// 	else if (aspect_ratio > 1) // Too tall
// 	{
// 		this->setMaximumHeight(
// 			static_cast<int>(this->width())
// 		);
// 	}
// }
