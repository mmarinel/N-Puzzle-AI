/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MenuView.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cy4gate_mmarinelli <cy4gate_mmarinelli@    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 13:40:08 by matteo            #+#    #+#             */
/*   Updated: 2024/05/30 18:22:05 by cy4gate_mma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MenuView.hpp"
#include "Window.hpp"
#include "UIState.hpp"
#include "utils.h"

#include <QSpacerItem>
#include <QFileDialog>
#include <QDebug>
#include <QtGlobal>

MenuView::MenuView(QWidget* parent): QVBoxLayout{parent}
{
	// Adding widgets
	QHBoxLayout*	choose_file_area = new QHBoxLayout{};
	QHBoxLayout*	heuristics_area = new QHBoxLayout{};
	QHBoxLayout*	choose_at_random_area = new QHBoxLayout{};
	QHBoxLayout*	choose_size_area = new QHBoxLayout{};
	
	choose_file = new QPushButton(
		CHOOSE_FILE_TEXT,
		static_cast<QWidget*>(parent)
	);
	choose_random = new QCheckBox(
		"...or choose at random",
		static_cast<QWidget*>(parent)
	);
	choose_size = new QSpinBox(
		static_cast<QWidget*>(parent)
	);
	choose_solvability = new QRadioButton(
		"Solvable",
		static_cast<QWidget*>(parent)
	);
	size_lbl = new QLabel(
		"Size: ",
		static_cast<QWidget*>(parent)
	);
	heurstic_lbl = new QLabel(
		"Choose heuristic: ",
		static_cast<QWidget*>(parent)
	);
	choose_heuristic = new QComboBox{
		static_cast<QWidget*>(parent)
	};
	
	for (auto& hName: NPuzzle::hfromString)
	{
		choose_heuristic->addItem(
			hName.first.c_str()
		);
	}
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
	choose_at_random_area->addWidget(choose_solvability);
	choose_at_random_area->setSpacing(50);
	this->addLayout(choose_at_random_area);
	heuristics_area->addWidget(heurstic_lbl);
	heuristics_area->addWidget(choose_heuristic);
	this->addLayout(heuristics_area);

	// Setting Visibilities
	this->choose_size->setVisible(false);
	this->choose_size->setDisabled(true);
	this->choose_solvability->setVisible(false);
	this->choose_solvability->setDisabled(true);
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
	QObject::connect(
		choose_solvability, &QAbstractButton::clicked,
		this, &MenuView::toggleRandomSolvable
	);
}

MenuView::~MenuView() {}

void	MenuView::setBoardFile()
{
	QString		boardFileName = QFileDialog::getOpenFileName(
		qobject_cast<QWidget*>(this->parent()),
		tr("Choose file"), "~", tr("Board files (*.txt)")
	);
	// didn't choose no file
	if (boardFileName.isEmpty())
		return ;
	// invalid file
	if (false == NPuzzle::parse_file(boardFileName))
	{
		CustomDialog	d{
			"Error\nCould not parse file",
			QDialogButtonBox::Cancel,
			qobject_cast<QWidget*>(this->parent())
		};

		d.exec();
		return ;
	}
	UIState::getInstance().boardFileName = boardFileName;
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
		setRandomGeneratedSize(choose_size->text().toInt());
		this->choose_file->setDisabled(true);
		this->choose_size->setVisible(true);
		this->choose_size->setDisabled(false);
		this->choose_solvability->setVisible(true);
		this->choose_solvability->setDisabled(false);
		this->size_lbl->setVisible(true);
		this->size_lbl->setDisabled(false);
	}
	else
	{
		UIState::getInstance().atRandom = false;
		this->choose_file->setDisabled(false);
		this->choose_size->setVisible(false);
		this->choose_size->setDisabled(true);
		this->choose_solvability->setVisible(false);
		this->choose_solvability->setDisabled(true);
		this->size_lbl->setVisible(false);
		this->size_lbl->setDisabled(true);
	}
}

void	MenuView::setRandomGeneratedSize(int n)
{
	BoardState::getInstance().setSize(n);
}

void	MenuView::toggleRandomSolvable()
{
	UIState::getInstance().atRandomSolvable = ! UIState::getInstance().atRandomSolvable;
}

void	MenuView::setHeuristic(int index)
{
	Q_UNUSED(index);
	UIState::getInstance().h = NPuzzle::hfromString.at(
		choose_heuristic->itemText(
			choose_heuristic->currentIndex()
		).toStdString()
	);
}

bool	MenuView::canGoForward(QString& msg)
{
	auto&	ui_state = UIState::getInstance();
	auto&	board_state = BoardState::getInstance();
	
	if (
		NPuzzle::t_heuristic::CORNER_TILES == ui_state.h &&
		3 == board_state.size
	)
	{
		msg = "Corner Tiles heuristic only available for sizes greater than 3";
		return false;
	}
	return true;
}

void	MenuView::reset()
{
	choose_file->setText(CHOOSE_FILE_TEXT);
	choose_file->setToolTip("");
	if (choose_random->isChecked())
	{
		choose_random->setChecked(false);
		setAtRandomFile();
	}
	choose_solvability->setChecked(false);
	choose_heuristic->setPlaceholderText("--Select Heuristic--");
	choose_heuristic->setCurrentIndex(-1);
}
