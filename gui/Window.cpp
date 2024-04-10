/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Window.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 19:35:42 by matteo            #+#    #+#             */
/*   Updated: 2024/04/10 19:39:03 by matteo           ###   ########.fr       */
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

	// Adding fixed components
	forward_btn = new QPushButton{">"};
	backward_btn = new QPushButton{"<"};
	forward_btn->setObjectName("forwardBtn");
	backward_btn->setObjectName("backwardBtn");
	backforward_area->addWidget(backward_btn);
	backforward_area->addWidget(forward_btn);

	// Adding pages
	menu_page = new QWidget{};
	menu_page->setLayout(new MenuView{});

	//Setting content
	content = new QStackedWidget{};
	
	static_cast<MenuView*>(menu_page->layout())
		->index = content->addWidget(menu_page);
	content->setCurrentIndex(
		static_cast<MenuView*>(menu_page->layout())->index
	);

	area->addWidget(content);
	area->addLayout(backforward_area);

	//
	this->setLayout(area);
	
}

Window::~Window() {}
