/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Content.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 19:55:50 by matteo            #+#    #+#             */
/*   Updated: 2024/04/10 20:09:06 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Content.hpp"
#include "Window.hpp"

int		Content::addWidget(QWidget* widget)
{
	QSpacerItem*		vert1 = new QSpacerItem(
		WIDTH / 4,
		HEIGHT
	);
	QSpacerItem*		vert2 = new QSpacerItem(
		WIDTH / 4,
		HEIGHT
	);
	QSpacerItem*		hoz1 = new QSpacerItem(
		WIDTH / 4,
		HEIGHT / 3
	);
	QSpacerItem*		hoz2 = new QSpacerItem(
		WIDTH / 4,
		HEIGHT / 3
	);
	QHBoxLayout*		l2 = new QHBoxLayout{};
	QVBoxLayout*		l1 = new QVBoxLayout{};
	QWidget*			w = new QWidget{};

	l1->addItem(hoz1);
	l1->addWidget(widget);
	l1->addItem(hoz2);

	l2->addItem(vert1);
	l2->addLayout(l1);
	l2->addItem(vert2);

	w->setLayout(l2);

	return QStackedWidget::addWidget(w);
}
