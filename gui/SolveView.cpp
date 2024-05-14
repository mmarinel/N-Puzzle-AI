/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SolveView.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 19:18:31 by matteo            #+#    #+#             */
/*   Updated: 2024/05/11 19:14:59 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SolveView.hpp"
#include "Window.hpp"
#include "CustomDialog.hpp"
#include "UIState.hpp"
#include "utils.h"

#include <QDialogButtonBox>

//? EXPERIMENT FOR AI ACTUATION
#include <QTimer>
#include <functional>

SolveView::SolveView(QWidget* parent): QVBoxLayout{parent},
solving{false},
executing{false}
{
	// Creating widgets
	
	// Creating detached board
	second_window = new QWidget{};
	new_win_board = new BoardView{};
	QHBoxLayout*	new_win_board_box = (
		new QHBoxLayout{}
	);
	QScrollArea*	new_window_scroll_area = (
		new QScrollArea{second_window}
	);
	new_win_board->setSizePolicy(
		QSizePolicy::Maximum, QSizePolicy::Maximum
	);
	new_window_scroll_area->setWidget(new_win_board);
	new_window_scroll_area->setWindowState(Qt::WindowFullScreen);
	new_win_board_box->addWidget(new_window_scroll_area);
	new_win_board_box->setAlignment(Qt::AlignCenter);
	second_window->setLayout(new_win_board_box);
	second_window->setWindowFlags(
		Qt::Window |
		Qt::WindowMinimizeButtonHint |
		Qt::WindowMaximizeButtonHint
	);

	// Creating in-window board
	board_box = new QHBoxLayout{};
	board = new BoardView(
		static_cast<QWidget*>(parent)
	);
	board->setSizePolicy(
		QSizePolicy::Maximum, QSizePolicy::Maximum
	);
	board_box->addWidget(board);
	board_box->setContentsMargins(-1, -1, -1, 50);

	// Creating play buttons
	btns_stacked = new QStackedWidget{
		static_cast<QWidget*>(parent)
	};

	solve_btn_box = new QWidget{};
	solve_btn_box->setLayout(new QHBoxLayout{});
	solve_btn = new QPushButton{"Solve", solve_btn_box};
	solve_btn	->setFixedWidth(WIDTH / 3);
	solve_btn	->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
	solve_btn_box->layout()->addWidget(solve_btn);
	
	play_box = new QWidget{};
	play_box->setLayout(new QHBoxLayout{});
	playback_btn = new QPushButton{"-1", play_box};
	play_btn = new QPushButton{">", play_box};
	playforward_btn = new QPushButton{"+1", play_box};
	playback_btn->setObjectName("playBackButton");
	play_btn->setObjectName("playButton");
	playforward_btn->setObjectName("playForwardButton");
	play_box->layout()->setAlignment(Qt::AlignCenter);
	play_box->layout()->addWidget(playback_btn);
	play_box->layout()->addWidget(play_btn);
	play_box->layout()->addWidget(playforward_btn);

	btns_stacked->addWidget(solve_btn_box);
	btns_stacked->addWidget(play_box);

	// Creating output log widget
	output_box = new QHBoxLayout{};
	output = new QTextEdit(
		static_cast<QWidget*>(parent)
	);
	output->setReadOnly(true);
	output->setFixedWidth(WIDTH - 50);
	output		->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
	output		->setFrameStyle(QFrame::Box | QFrame::Sunken);
	output		->setLineWidth(3);
	output		->setMidLineWidth(3);
	output_box->addWidget(output);

	// Adding to the vertical centered layout
	this->addLayout(board_box);
	this->addWidget(btns_stacked);
	this->addLayout(output_box);

	// Setting visibilities
	second_window->setVisible(false);
	board->setVisible(false);
	
	// Controller
	QObject::connect(
		this->solve_btn, &QPushButton::clicked,
		this, &SolveView::startSolving
	);
	QObject::connect(
		this->play_btn, &QPushButton::clicked,
		this, &SolveView::play_stop
	);

	//? EXPERIMENT FOR AI ACTUATION
	// QTimer::singleShot(
	// 	3500, [this](){
	// 		static_cast<QLabel*>(
	// 			this->board->grid->itemAtPosition(6, 7)->widget()
	// 		)->setFocus(Qt::FocusReason::ActiveWindowFocusReason);
	// 		BoardState::getInstance().swap(7,6,7,7);
	// 		QTimer::singleShot(
	// 			2000, [this](){
	// 				static_cast<QLabel*>(
	// 					this->board->grid->itemAtPosition(6, 7)->widget()
	// 				)->setFocus(Qt::FocusReason::NoFocusReason);
	// 				static_cast<QLabel*>(
	// 					this->board->grid->itemAtPosition(6, 6)->widget()
	// 				)->setFocus(Qt::FocusReason::ActiveWindowFocusReason);
	// 			}
	// 		);
	// 	}
	// );
}

SolveView::~SolveView()
{
	delete second_window;
}

// SLOTS

void	SolveView::startSolving()
{
	qDebug() << "SolveView::startSolving";
	this->solving = true;

	btns_stacked->setCurrentIndex(1);

	agent = new NPuzzle::Agent(
		BoardState::getInstance().board,
		BoardState::getInstance().size,
		BoardState::getInstance().x_empty,
		BoardState::getInstance().y_empty,
		UIState::getInstance().h
	);
	agent->run();
	agent->wait();
	QTimer::singleShot(
		1000, std::bind(&SolveView::moveTile, this)
	);
	//TODO
	//TODO 1. disable inappropriate buttons &&  Add Loading gif
	//TODO 1.1 	only leave buttons like the "close" button to not freeze the app
	//TODO 3. Connect SLOT for work done
	//TODO 2. Start AI QThread

	//TODO this will be inside mentioned SLOT
	// this->insertLayout(
	// 	BoardState::getInstance().size >= THRESHOLD_FOR_NEW_WINDOW
	// 	? 0 : 1
	// 	, play_box
	// );
}

void			SolveView::moveTile()
{
	qDebug() << "Moving Tile";
	if (agent->solution.empty())
		return ;
	auto		a = agent->solution.top();
	qDebug() << "action: " << actionToString(a).c_str() ;
	BoardState&	state = BoardState::getInstance();
	Tile&		empty_tile = (
			state.board[state.y_empty][state.x_empty]
	);
	agent->solution.pop();
	
	if ( t_action::UP == a )
	{
		Tile&	tile = (
			state.board[state.y_empty - 1][state.x_empty]
		);
		state.y_empty = state.y_empty - 1;
		state.x_empty = state.x_empty;
		empty_tile = tile;
		tile = 0;
	}
	if ( t_action::DOWN == a )
	{
		Tile&	tile = (
			state.board[state.y_empty + 1][state.x_empty]
		);
		state.y_empty = state.y_empty + 1;
		state.x_empty = state.x_empty;
		empty_tile = tile;
		tile = 0;
	}
	if ( t_action::LEFT == a )
	{
		Tile&	tile = (
			state.board[state.y_empty][state.x_empty - 1]
		);
		state.y_empty = state.y_empty;
		state.x_empty = state.x_empty - 1;
		empty_tile = tile;
		tile = 0;
	}
	if ( t_action::RIGHT == a )
	{
		Tile&	tile = (
			state.board[state.y_empty][state.x_empty + 1]
		);
		state.y_empty = state.y_empty;
		state.x_empty = state.x_empty + 1;
		empty_tile = tile;
		tile = 0;
	}
	this->board->repaint();
	QTimer::singleShot(
		1000, std::bind(&SolveView::moveTile, this)
	);
}

void	SolveView::play_stop()
{
	if (executing)
	{
		this->playforward_btn->setDisabled(false);
		this->playback_btn->setDisabled(false);
		this->play_btn->setText(">");
		executing = false;
	}
	else
	{
		this->playforward_btn->setDisabled(true);
		this->playback_btn->setDisabled(true);
		this->play_btn->setText("||");
		executing = true;
	}
}

bool	SolveView::abort()
{
	//Show dialog for confirmation
	CustomDialog	d{
		"Are you sure you want to abort AI computation?",
		QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
		qobject_cast<QWidget*>(this->parent())
	};

	if (QDialog::Rejected == d.exec())
		return false ;

	//TODO handle reset of AI state? && BoardState
	
	this->solving = false;
	this->executing = false;

	//Resetting solve button
	btns_stacked->setCurrentIndex(0);

	//Resetting play-stop button
	this->playforward_btn->setDisabled(false);
	this->playback_btn->setDisabled(false);
	this->play_btn->setText(">");

	//resetting boards
	second_window->setVisible(false);
	board->setVisible(false);

	return true;
}

void	SolveView::start()
{
	if (BoardState::getInstance().size < THRESHOLD_FOR_NO_GRID)
	{
		if (UIState::getInstance().atRandom)
			NPuzzle::generate_board();
		if (BoardState::getInstance().size >= THRESHOLD_FOR_NEW_WINDOW)
		{
			new_win_board->setup();
			output->setMinimumHeight(HEIGHT - 200);
			second_window->show();
		}
		else
		{
			board->setup();
			output->setMinimumHeight(HEIGHT / 4.5f);
			board->setVisible(true);
		}
	}
}

void	SolveView::close()
{
	if (second_window)
		second_window->setVisible(false);
}
