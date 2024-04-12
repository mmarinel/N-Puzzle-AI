/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Window.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 19:35:42 by matteo            #+#    #+#             */
/*   Updated: 2024/04/12 16:50:27 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Window.hpp"
#include "Page.hpp"
#include "Content.hpp"

#include <QSpacerItem>

Window::Window(): QWidget{}
{
	setFixedSize(
		WIDTH, HEIGHT
	);
	area 								= new QVBoxLayout{};
	QHBoxLayout*	backforward_area	= new QHBoxLayout();

	// Adding fixed components
	forward_btn = new QPushButton{">"};
	backward_btn = new QPushButton{"<"};
	forward_btn->setObjectName("forwardBtn");
	backward_btn->setObjectName("backwardBtn");
	backforward_area->addWidget(backward_btn);
	backforward_area->addWidget(forward_btn);

	// Adding pages
	menu_page = new Page{};
	menu_page->setLayout(new MenuView{});
	solve_page = new Page{};
	solve_page->setLayout(new SolveView{});

	//Setting content
	content = new Content{};
	
	menu_page->index = content->addWidget(menu_page);
	solve_page->index = content->addWidget(solve_page);
	
	content->setCurrentIndex(menu_page->index);

	area->addWidget(content);
	area->addLayout(backforward_area);

	//Controller
	QObject::connect(
		forward_btn, &QPushButton::clicked,
		[this](){
			this->content->setCurrentIndex(
				solve_page->index
			);
		}
	);

	//
	this->setLayout(area);
	
}

Window::~Window() {}
