/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Window.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 19:35:42 by matteo            #+#    #+#             */
/*   Updated: 2024/04/07 17:11:07 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Window.hpp"

#include <QSpacerItem>

Window::Window(): QWidget{}
{
	setFixedSize(
		WIDTH, HEIGHT
	);
	area = new QVBoxLayout{};
	QHBoxLayout*	backforward_area = new QHBoxLayout();

	
	forward_btn = new QPushButton{">"};
	backward_btn = new QPushButton{"<"};
	forward_btn->setObjectName("forwardBtn");
	backward_btn->setObjectName("backwardBtn");
	backforward_area->addWidget(backward_btn);
	backforward_area->addWidget(forward_btn);

	area->addLayout(new MenuView());
	area->addLayout(backforward_area);
	// area->addItem(
	// 	new QSpacerItem(
	// 		WIDTH / 2, HEIGHT
	// 	)
	// );
	// area->addLayout(
	// 	new MenuView()
	// );
	this->setLayout(area);
	
	// ui_area = new QVBoxLayout{};
	// board = new QGridLayout{};
	// buttons_area = new QHBoxLayout{};

	// ui_area->addLayout(board);
	// ui_area->addLayout(buttons_area);

	// play = new QPushButton("|>");
	// move_forward = new QPushButton("+1");
	// move_backward = new QPushButton("-1");

	// buttons_area->addWidget(move_backward);
	// buttons_area->addWidget(play);
	// buttons_area->addWidget(move_forward);


	// this->setLayout(ui_area);
}

Window::~Window() {}
