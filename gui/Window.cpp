/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Window.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 19:35:42 by matteo            #+#    #+#             */
/*   Updated: 2024/04/21 19:27:21 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Window.hpp"
#include "Page.hpp"
#include "Content.hpp"
#include "UIState.hpp"

#include <QSpacerItem>

Window::Window(): QWidget{}
{
	// Setting windows props
	setFixedSize(
		WIDTH, HEIGHT
	);

	// Creating Widgets
	area 								= new QVBoxLayout{};
	QHBoxLayout*	backforward_area	= new QHBoxLayout();
	forward_btn = new QPushButton{">", this};
	backward_btn = new QPushButton{"<", this};
	forward_btn->setObjectName("forwardBtn");
	backward_btn->setObjectName("backwardBtn");
	backforward_area->addWidget(backward_btn);
	backforward_area->addWidget(forward_btn);

	notice = new CustomDialog{
		"NOTICE\n\
for larger puzzle sizes, the grid may be displayed in a new window\n\
for even larger sizes, the grid may not be displayed at all...please use the output text field",
		QDialogButtonBox::Ok,
		this
	};

	// Adding pages
	menu_page = new Page{};
	menu_page->setLayout(new MenuView{menu_page});
	solve_page = new Page{};
	solve_page->setLayout(new SolveView{solve_page});

	//Setting content
	content = new Content{this};
	
	menu_page->index = content->addWidget(menu_page);
	solve_page->index = content->addWidget(solve_page);
	
	area->addWidget(content);
	area->addLayout(backforward_area);

	// Setting visibilities
	backward_btn->setDisabled(true);
	
	// Setting window
	this->setLayout(area);
	
	//Controller
	QObject::connect(
		forward_btn, &QPushButton::clicked,
		this, &Window::forward
	);
	QObject::connect(
		backward_btn, &QPushButton::clicked,
		this, &Window::backward
	);
}

Window::~Window() {}

void	Window::forward()
{
	if (CurrentPage::MENU == UIState::getInstance().currentPage)
	{
		//? Check if all mandatory fields have been filled in
		if (
			(UIState::getInstance().boardFileName.isEmpty() && false == UIState::getInstance().atRandom) ||
			(NPuzzle::t_heuristic::NONE == UIState::getInstance().h)
		)
		{
			CustomDialog	d(
				"Error\nfill all mandatory fields!",
				QDialogButtonBox::Ok,
				this
			);
			d.exec();
			return ;
		}
		if (nullptr != notice)
		{
			//? Display the notice only the first time
			notice->exec();
			delete notice;
			notice = nullptr;
		}
		//? Opening new window if grid too big
		static_cast<SolveView*>(
			solve_page->layout()
		)->start();
		// Changing page and status
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
		// Tell Solve View to abort computation
		auto aborted = static_cast<SolveView*>(
			solve_page->layout()
		)->abort();
		if (false == aborted)
			return ;
		// Changing page and status
		this->content->setCurrentIndex(
			menu_page->index
		);
		this->backward_btn->setDisabled(true);
		this->forward_btn->setDisabled(false);
		UIState::getInstance().currentPage = CurrentPage::MENU;
	}
}

void	Window::setVisible(bool visible)
{
	// Manage closing of second window too
	if (false == visible)
	{
		static_cast<SolveView*>(
			solve_page->layout()
		)->close();
	}
	QWidget::setVisible(visible);
}
