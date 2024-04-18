/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MenuView.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 13:40:08 by matteo            #+#    #+#             */
/*   Updated: 2024/04/18 20:14:48 by matteo           ###   ########.fr       */
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
	QHBoxLayout*	choose_file_area = new QHBoxLayout{};
	QHBoxLayout*	heuristics_area = new QHBoxLayout{};
	QHBoxLayout*	choose_at_random_area = new QHBoxLayout{};
	QHBoxLayout*	choose_size_area = new QHBoxLayout{};
	
	choose_file = new QPushButton(CHOOSE_FILE_TEXT);
	choose_random = new QCheckBox("...or choose at random");
	choose_size = new QSpinBox();
	size_lbl = new QLabel("Size: ");
	heurstic_lbl = new QLabel("Choose heuristic: ");
	choose_heuristic = new QComboBox{};
	
	choose_heuristic->addItem(
		NPuzzle::to_string(NPuzzle::t_heuristic::MANHATTAN_DISTANCE).c_str()
	);
	choose_heuristic->setPlaceholderText("--Select Heuristic--");
	choose_heuristic->setCurrentIndex(-1);

	choose_size->setMinimum(3);

	// Adding to the vertical centered layout
	choose_file_area->addWidget(choose_file);
	choose_file_area->setContentsMargins(0, 0, 0, 20);
	this->addLayout(choose_file_area);
	choose_at_random_area->addWidget(choose_random);
	choose_size_area->addWidget(size_lbl);
	choose_size_area->addWidget(choose_size);
	choose_at_random_area->addLayout(choose_size_area);
	choose_at_random_area->setSpacing(50);
	this->addLayout(choose_at_random_area);
	heuristics_area->addWidget(heurstic_lbl);
	heuristics_area->addWidget(choose_heuristic);
	this->addLayout(heuristics_area);

	// Setting Visibilities
	this->choose_size->setVisible(false);
	this->choose_size->setDisabled(true);
	this->size_lbl->setVisible(false);
	this->size_lbl->setDisabled(true);
	
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
	QObject::connect(
		choose_size, &QSpinBox::valueChanged,
		this, &MenuView::setRandomGeneratedSize
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
		this->choose_size->setVisible(true);
		this->choose_size->setDisabled(false);
		this->size_lbl->setVisible(true);
		this->size_lbl->setDisabled(false);
	}
	else
	{
		UIState::getInstance().atRandom = false;
		this->choose_file->setDisabled(false);
		this->choose_size->setVisible(false);
		this->choose_size->setDisabled(true);
		this->size_lbl->setVisible(false);
		this->size_lbl->setDisabled(true);
	}
}

void	MenuView::setRandomGeneratedSize(int n)
{
	qDebug() << "new size: " << n;
	UIState::getInstance().size = n;
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
