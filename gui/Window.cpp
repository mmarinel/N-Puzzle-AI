/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Window.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 19:35:42 by matteo            #+#    #+#             */
/*   Updated: 2024/04/18 19:22:28 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Window.hpp"
#include "Page.hpp"
#include "Content.hpp"
#include "UIState.hpp"

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
	backward_btn->setDisabled(true);

	notice = new CustomDialog{
		"NOTICE\n\
for larger puzzle sizes, the grid may be displayed in a new window\n\
for even larger sizes, the grid may not be displayed at all...please use the output text field",
		QDialogButtonBox::Ok,
		this
	};

	// Adding pages
	menu_page = new Page{};
	menu_page->setLayout(new MenuView{});
	solve_page = new Page{};
	solve_page->setLayout(new SolveView{});

	//Setting content
	content = new Content{};
	
	menu_page->index = content->addWidget(menu_page);
	solve_page->index = content->addWidget(solve_page);
	
	// content->setCurrentIndex(menu_page->index);

	area->addWidget(content);
	area->addLayout(backforward_area);

	//Controller
	QObject::connect(
		forward_btn, &QPushButton::clicked,
		this, &Window::forward
	);
	QObject::connect(
		backward_btn, &QPushButton::clicked,
		this, &Window::backward
	);

	//
	this->setLayout(area);
}

Window::~Window() {}

void	Window::forward()
{
	if (CurrentPage::MENU == UIState::getInstance().currentPage)
	{
		if (nullptr != notice)
		{
			//? Display the notice only the first time
			notice->exec();
			delete notice;
			notice = nullptr;
		}
		this->backward_btn->setDisabled(false);
		this->forward_btn->setDisabled(true);
		this->content->setCurrentIndex(
			solve_page->index
		);
		UIState::getInstance().currentPage = CurrentPage::SOLVE;
	}
}

void	Window::backward()
{
	if (CurrentPage::SOLVE == UIState::getInstance().currentPage)
	{
		static_cast<SolveView*>(
			solve_page->layout()
		)->abort();
		this->content->setCurrentIndex(
			menu_page->index
		);
		this->backward_btn->setDisabled(true);
		this->forward_btn->setDisabled(false);
		UIState::getInstance().currentPage = CurrentPage::MENU;
	}
}
