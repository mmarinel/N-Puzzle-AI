/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TileView.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 20:06:37 by matteo            #+#    #+#             */
/*   Updated: 2024/04/11 20:40:20 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <QLabel>

class TileView: public QLabel
{
	Q_OBJECT
private:
public:

	TileView();
	~TileView();

	bool	hasHeightForWidth() const override;
	int		heightForWidth(int w) const override;

// protected:
// 	void	resizeEvent(QResizeEvent* event);
};
