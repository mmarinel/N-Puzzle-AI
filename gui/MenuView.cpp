/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MenuView.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 13:40:08 by matteo            #+#    #+#             */
/*   Updated: 2024/04/12 16:51:50 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MenuView.hpp"
#include "Window.hpp"
#include "UIState.hpp"

#include <QSpacerItem>
#include <QFileDialog>
#include <QDebug>

MenuView::MenuView(): QVBoxLayout{}
{
	// Adding widgets
	QHBoxLayout*	heuristics_area = new QHBoxLayout();
	choose_file = new QPushButton(CHOOSE_FILE_TEXT);
	choose_random = new QCheckBox("...or choose at random");
	heuristics_area = new QHBoxLayout{};
	heurstic_lbl = new QLabel("Choose heuristic: ");
	choose_heuristic = new QComboBox{};
	
	choose_heuristic->addItem(
		NPuzzle::to_string(NPuzzle::t_heuristic::MANHATTAN_DISTANCE).c_str()
	);
	choose_heuristic->setPlaceholderText("--Select Heuristic--");
	choose_heuristic->setCurrentIndex(-1);

	// Adding to the vertical centered layout
	this->addWidget(choose_file);
	this->addWidget(choose_random);
	heuristics_area->addWidget(heurstic_lbl);
	heuristics_area->addWidget(choose_heuristic);
	this->addLayout(heuristics_area);

	// Controller
	QObject::connect(
		choose_file, &QPushButton::clicked,
		this, &MenuView::setBoardFile
	);
	QObject::connect(
		choose_random, &QCheckBox::clicked,
		this, &MenuView::setAtRandomFile
	);
	QObject::connect(
		choose_heuristic, &QComboBox::activated,
		this, &MenuView::setHeuristic
	);
}

MenuView::~MenuView() {}

void	MenuView::setBoardFile()
{
	UIState::getInstance().boardFileName = QFileDialog::getOpenFileName(
		this->choose_file, tr("Choose file"), "~", tr("Board files (*.txt)")
	);
	if (UIState::getInstance().boardFileName.isEmpty())
		return ;
	this->choose_file->setText(
		UIState::getInstance().boardFileName.split("/").last()
	);
	this->choose_file->setToolTip(UIState::getInstance().boardFileName);
	this->update();
}

void	MenuView::setAtRandomFile()
{
	this->choose_file->setText(CHOOSE_FILE_TEXT);
	this->choose_file->setToolTip("");
	if (this->choose_random->isChecked())
	{
		UIState::getInstance().atRandom = true;
		this->choose_file->setDisabled(true);
	}
	else
	{
		UIState::getInstance().atRandom = false;
		this->choose_file->setDisabled(false);
	}
}

void	MenuView::setHeuristic(int index)
{
	if (
		NPuzzle::to_string(
			NPuzzle::t_heuristic::MANHATTAN_DISTANCE
		) ==
		choose_heuristic->itemText(
			choose_heuristic->currentIndex()
		).toStdString()
	)
	{
		qDebug() << "Chosen Manhattan Distance";
		UIState::getInstance().h = NPuzzle::t_heuristic::MANHATTAN_DISTANCE;
	}
}
