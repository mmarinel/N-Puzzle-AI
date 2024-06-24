/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TileView.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmarinel <mmarinel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 20:06:37 by matteo            #+#    #+#             */
/*   Updated: 2024/06/24 18:15:00 by mmarinel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <QLabel>

class TileView: public QLabel
{
	Q_OBJECT
private:
public:

	TileView(QWidget* parent=nullptr);
	~TileView();

	bool	hasHeightForWidth() const override;
	int		heightForWidth(int w) const override;

protected:
	void	resizeEvent(QResizeEvent* event) override;
};
