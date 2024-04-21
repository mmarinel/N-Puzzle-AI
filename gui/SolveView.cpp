/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SolveView.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 19:18:31 by matteo            #+#    #+#             */
/*   Updated: 2024/04/21 23:11:27 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SolveView.hpp"
#include "Window.hpp"
#include "CustomDialog.hpp"

#include <QDialogButtonBox>

//? EXPERIMENT FOR AI ACTUATION
// #include <QTimer>

SolveView::SolveView(QWidget* parent): QVBoxLayout{parent},
solving{false},
executing{false}
{
	// Creating widgets
	second_window = nullptr;
	new_win_board = nullptr;
	board = nullptr;
	if (BoardState::getInstance().size < THRESHOLD_FOR_NO_GRID)
	{
		if (BoardState::getInstance().size >= THRESHOLD_FOR_NEW_WINDOW)
		{
			second_window = new QWidget{};
			new_win_board = new BoardView{};
			new_win_board->setSizePolicy(
				QSizePolicy::Expanding, QSizePolicy::Expanding
			);
			QHBoxLayout*	new_win_board_box = new QHBoxLayout{};
			QScrollArea*	new_window_scroll_area = new QScrollArea{second_window};
			
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
		}
		else
		{
			board_box = new QHBoxLayout{};
			board = new BoardView(
				static_cast<QWidget*>(parent)
			);
			board->setSizePolicy(
				QSizePolicy::Expanding, QSizePolicy::Preferred
			);
			board_box->addWidget(board);
			board_box->setContentsMargins(-1, -1, -1, 50);
		}
	}

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

	output_box = new QHBoxLayout{};
	output = new QTextEdit(
		static_cast<QWidget*>(parent)
	);
	output->setText(
		"Lorem ipsum dolor sit amet consectetur adipisicing elit. Maxime mollitia, \
molestiae quas vel sint commodi repudiandae consequuntur voluptatum laborum \
numquam blanditiis harum quisquam eius sed odit fugiat iusto fuga praesentium \
optio, eaque rerum! Provident similique accusantium nemo autem. Veritatis \
obcaecati tenetur iure eius earum ut molestias architecto voluptate aliquam \
nihil, eveniet aliquid culpa officia aut! Impedit sit sunt quaerat, odit, \
tenetur error, harum nesciunt ipsum debitis quas aliquid. Reprehenderit, \
quia. Quo neque error repudiandae fuga? Ipsa laudantium molestias eos \
sapiente officiis modi at sunt excepturi expedita sint? Sed quibusdam \
recusandae alias error harum maxime adipisci amet laborum. Perspiciatis \
minima nesciunt dolorem! Officiis iure rerum voluptates a cumque velit  \
quibusdam sed amet tempora. Sit laborum ab, eius fugit doloribus tenetur  \
fugiat, temporibus enim commodi iusto libero magni deleniti quod quam \
consequuntur! Commodi minima excepturi repudiandae velit hic maxime \
doloremque. Quaerat provident commodi consectetur veniam similique ad \
earum omnis ipsum saepe, voluptas, hic voluptates pariatur est explicabo \
fugiat, dolorum eligendi quam cupiditate excepturi mollitia maiores labore \
suscipit quas? Nulla, placeat. Voluptatem quaerat non architecto ab laudantium \
modi minima sunt esse temporibus sint culpa, recusandae aliquam numquam \
totam ratione voluptas quod exercitationem fuga. Possimus quis earum veniam \
quasi aliquam eligendi, placeat qui corporis!"
	);
	output->setReadOnly(true);
	output->setFixedWidth(WIDTH - 50);
	if (board)
		output->setMinimumHeight(HEIGHT / 4.5f);
	else
		output->setMinimumHeight(HEIGHT - 200);
	output		->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
	output		->setFrameStyle(QFrame::Box | QFrame::Sunken);
	output		->setLineWidth(3);
	output		->setMidLineWidth(3);
	output_box->addWidget(output);

	// Adding to the vertical centered layout
	if (board)
	{
		this->addLayout(board_box);
	}
	this->addWidget(btns_stacked);
	this->addLayout(output_box);

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
	this->solving = true;

	btns_stacked->setCurrentIndex(1);
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

	//TODO handle reset of AI state?
	
	this->solving = false;
	this->executing = false;

	//Resetting solve button
	btns_stacked->setCurrentIndex(0);

	//Resetting play-stop button
	this->playforward_btn->setDisabled(false);
	this->playback_btn->setDisabled(false);
	this->play_btn->setText(">");

	//deleting second window if created
	if (second_window)
		second_window->setVisible(false);

	return true;
}

void	SolveView::start()
{
	if (second_window)
		second_window->show();
}

void	SolveView::close()
{
	if (second_window)
		second_window->setVisible(false);
}
