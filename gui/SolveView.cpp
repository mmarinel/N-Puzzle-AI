/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SolveView.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmarinel <mmarinel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 19:18:31 by matteo            #+#    #+#             */
/*   Updated: 2024/06/24 18:53:04 by mmarinel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SolveView.hpp"
#include "Window.hpp"
#include "CustomDialog.hpp"
#include "UIState.hpp"
#include "utils.h"
#include "gui_utils.hpp"
#include "Node.hpp"

#include <QDialogButtonBox>
#include <QApplication>

#include <functional>
#include <sstream>

/**
 * @brief key=Velocity value=Interval length
 * 
 */
std::map<std::string, int>
SolveView::velocities = {
	{"SLOW", 1000},
	{"NORMAL", 500},
	{"FAST", 250},
	{"SUPER FAST", 100},
};

SolveView::SolveView(QWidget* parent): QVBoxLayout{parent},
solving{false},
executing{false},
agent{nullptr},
doing{},
undoing{}
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

	loading_lbl_box = new QWidget{};
	loading_lbl_box->setLayout(new QHBoxLayout{});
	loading_lbl = new QLabel{loading_lbl_box};
	loading_lbl->setFixedWidth(WIDTH / 18);
	loading_lbl->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
	loading_lbl->setObjectName("loadingLabel");
	gif = new QMovie(":/images/loading", QByteArray(), loading_lbl_box);
	loading_lbl->setMovie(gif);
	gif->start();
	loading_lbl_box->layout()->addWidget(loading_lbl);
	
	btns_stacked->addWidget(solve_btn_box);
	btns_stacked->addWidget(play_box);
	btns_stacked->addWidget(loading_lbl_box);

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
	output		->setWordWrapMode(QTextOption::NoWrap);
	output		->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	output		->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	output_box->addWidget(output);

	speedBox = new QWidget{};
	speedBox->setLayout(new QHBoxLayout{});
	speed_combo = new QComboBox{
		static_cast<QWidget*>(parent)
	};

	for (auto& pair: SolveView::velocities)
	{
		speed_combo->addItem(
			pair.first.c_str()
		);
	}
	speed_combo->setFixedWidth(WIDTH / 8);
	speed_combo->setCurrentIndex(0);
	speedBox->layout()->addWidget(speed_combo);
	speedBox->layout()->setAlignment(Qt::AlignCenter);
	
	// Adding to the vertical centered layout
	this->addLayout(board_box);
	this->addWidget(btns_stacked);
	this->addWidget(speedBox);
	this->addLayout(output_box);

	// Setting visibilities
	second_window->setVisible(false);
	board->setVisible(false);
	speed_combo->setVisible(false);
	
	// Controller
	QObject::connect(
		this->solve_btn, &QPushButton::clicked,
		this, &SolveView::startSolving
	);
	QObject::connect(
		this->playback_btn, &QPushButton::clicked,
		[this]() -> void {
			executing = false;
			moveTile(undoing, doing);
		}
	);
	QObject::connect(
		this->play_btn, &QPushButton::clicked,
		this, &SolveView::play_stop
	);
	QObject::connect(
		this->playforward_btn, &QPushButton::clicked,
		[this]() -> void {
			executing = false;
			moveTile(doing, undoing);
		}
	);
	QObject::connect(
		speed_combo, &QComboBox::activated,
		[this]() {
			int		new_interval = SolveView::velocities.at(
				speed_combo->itemText(
					speed_combo->currentIndex()
				).toStdString()
			);

			timer.setInterval(new_interval);
		}
	);
	QObject::connect(
		qApp, &QCoreApplication::aboutToQuit,
		[this](){
			if (agent)
			{
				QObject::disconnect(
					agent, nullptr,
					nullptr, nullptr
				);
				agent->requestInterruption();
				agent->wait();
				delete agent;
				agent = nullptr;
			}
			timer.stop();
			QObject::disconnect(
				&timer, nullptr,
				nullptr, nullptr
			);
		}
	);
}

SolveView::~SolveView()
{
	delete second_window;
}

// SLOTS

void	SolveView::workDone()
{
		//Calculating Time
	clock_gettime(CLOCK_MONOTONIC, &after);
		// replacing loading animation with interactive buttons
	btns_stacked->setCurrentIndex(1);
	agent->wait(QDeadlineTimer::Forever);
		// showing velocity combo box
	speed_combo->setVisible(true);

	QString		oldText = output->toPlainText();
	output->setText("");

	//Calculating Metrics
	uint64_t	before_ns = (before.tv_sec * 1000000000) + before.tv_nsec;
	uint64_t	after_ns = (after.tv_sec * 1000000000) + after.tv_nsec;
	int64_t		elapsed_sec = (after_ns - before_ns) / 1000000000;
	int64_t		elapsed_ns = (after_ns - before_ns) % 1000000000;
	int64_t		elapsed_minutes = elapsed_sec / 60;
				elapsed_sec = elapsed_sec % 60;
	int64_t		elapsed_hours = elapsed_minutes / 60;
				elapsed_minutes = elapsed_minutes % 60;
	int64_t		elapsed_ms = (elapsed_ns) / 1000000;
				elapsed_ns = (elapsed_ns) % 1000000;

	std::stringstream	ss;
	if (agent->solution.empty())
	{
		auto color = output->textColor();
		output->setTextColor(Qt::darkRed);
		output->append("Not Solvable");
		output->setTextColor(color);
		ss << "inversions at goal state: " << agent->problem().inversions_at_goal << std::endl;
		ss << "inversions at initial state: " << agent->problem().inversions_at_initial << std::endl;
		ss << "(read README.md NPuzzle chapter to understand why it is unsolvable)" << std::endl;
		ss << std::endl;
	}
	else
	{
		auto color = output->textColor();
		output->setTextColor(Qt::darkGreen);
		output->append("Solvable");
		ss << "with " << agent->moves << " moves" << std::endl << std::endl;
		output->setTextColor(color);
	}
	
	ss << "Elapsed time" << std::endl;
	ss << "hrs: " << elapsed_hours << std::endl;
	ss << "mins: " << elapsed_minutes << std::endl;
	ss << "sec: " << elapsed_sec << std::endl;
	ss << "ms: " << elapsed_ms << std::endl;
	ss << "ns: " << elapsed_ns << std::endl << std::endl;
	ss << "Space Complexity: " << Node::max_instances << " nodes" << std::endl;
	ss << "Time Complexity: " << NPuzzle::Agent::get_nbr_selected() << " nodes" << std::endl;
	ss << "Nodes instances at end of program: " << Node::instances << std::endl;
	output->append(ss.str().c_str());
	output->append(oldText);

	// deleting the agent
	QObject::disconnect(
		agent, nullptr,
		nullptr, nullptr
	);
	this->doing = std::move(agent->solution);
	delete agent;
	agent = nullptr;
}

void	SolveView::startSolving()
{
	this->solving = true;
	btns_stacked->setCurrentIndex(2);

	agent = new NPuzzle::Agent();
	QObject::connect(
		agent, &NPuzzle::Agent::workDone,
		this, &SolveView::workDone
	);
	clock_gettime(CLOCK_MONOTONIC, &before);
	agent->start();
	
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

void			SolveView::moveTile(
		Problem::Actions& pop_queue,
		Problem::Actions& push_queue
)
{
	if (pop_queue.empty())
	{
		if (executing)
		{
			timer.stop();
			QObject::disconnect(
				&timer, nullptr,
				nullptr, nullptr
			);
			play_stop();
		}
		output->append("<<<<< END OF SEARCH >>>>>");
		output->append("\n");
		output->append("\n");
		return ;
	}
	output->append("Next State");
	
	BoardState&		state = BoardState::getInstance();
	t_action		a = pop_queue.top();
	pop_queue.pop();
	push_queue.push(Problem::inverseAction(a));
	
	if ( t_action::UP == a )
	{
		state.swap(
			state.x_empty, state.y_empty, state.x_empty, state.y_empty - 1
		);
		state.y_empty = state.y_empty - 1;
		state.x_empty = state.x_empty;
	}
	if ( t_action::DOWN == a )
	{
		state.swap(
			state.x_empty, state.y_empty, state.x_empty, state.y_empty + 1
		);
		state.y_empty = state.y_empty + 1;
		state.x_empty = state.x_empty;
	}
	if ( t_action::LEFT == a )
	{
		state.swap(
			state.x_empty, state.y_empty, state.x_empty - 1, state.y_empty
		);
		state.y_empty = state.y_empty;
		state.x_empty = state.x_empty - 1;
	}
	if ( t_action::RIGHT == a )
	{
		state.swap(
			state.x_empty, state.y_empty, state.x_empty + 1, state.y_empty
		);
		state.y_empty = state.y_empty;
		state.x_empty = state.x_empty + 1;
	}
	if (BoardState::getInstance().size < THRESHOLD_FOR_NO_GRID)
	{
		if (BoardState::getInstance().size < THRESHOLD_FOR_NEW_WINDOW)
			this->board->repaint();
		else
			this->new_win_board->repaint();
	}
	output->append(
		std::move(BoardState::getInstance().toString())
	);
}

void	SolveView::play_stop()
{
	if (executing)
	{
		this->playforward_btn->setDisabled(false);
		this->playback_btn->setDisabled(false);
		this->play_btn->setText(">");
		QObject::disconnect(
			&timer, nullptr,
			nullptr, nullptr
		);
		timer.stop();
		executing = false;
	}
	else
	{
		this->playforward_btn->setDisabled(true);
		this->playback_btn->setDisabled(true);
		this->play_btn->setText("||");
		QObject::connect(
			&timer, &QTimer::timeout,
			[this]() -> void {
				moveTile(doing, undoing);
			}
		);
		int		interval = SolveView::velocities.at(
			speed_combo->itemText(
				speed_combo->currentIndex()
			).toStdString()
		);
		timer.setInterval(interval);
		timer.start();
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
	if (agent)
	{
		QObject::disconnect(
			agent, nullptr,
			nullptr, nullptr
		);
		agent->requestInterruption();
		agent->wait();
		delete agent;
		agent = nullptr;
	}

	timer.stop();
	QObject::disconnect(
		&timer, nullptr,
		nullptr, nullptr
	);

	//Resetting variables
	this->solving = false;
	this->executing = false;
	while (false == doing.empty())
		doing.pop();
	while (false == undoing.empty())
		undoing.pop();

	// Resetting line edit
	output->clear();

	//Resetting solve button
	btns_stacked->setCurrentIndex(0);

	//Resetting play-stop button
	this->playforward_btn->setDisabled(false);
	this->playback_btn->setDisabled(false);
	this->play_btn->setText(">");

	//resetting boards
	second_window->setVisible(false);
	board->setVisible(false);
	board->reset();
	new_win_board->reset();
	//Resetting state
	BoardState::getInstance().reset();
	UIState::getInstance().reset();
	
	return true;
}

void	SolveView::start()
{
	// generate board if random generation was required
	if (UIState::getInstance().atRandom)
		NPuzzle::generate_board();
	
	// Display in-window or out-window board VIEW
	if (BoardState::getInstance().size < THRESHOLD_FOR_NO_GRID)
	{
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
	else
		output->setMinimumHeight(HEIGHT - 200);
	
	// Display taken choices
	std::stringstream	ss;
	
	if (UIState::getInstance().atRandom)
		ss << "Chosen file: at random" << std::endl;
	else
		ss << "Chosen file: " << UIState::getInstance().boardFileName.toStdString() << std::endl;
	for (const auto& pair: NPuzzle::hfromString)
	{
		if (pair.second == UIState::getInstance().h)
		{
			ss << "Chosen heuristic: " << pair.first << std::endl;
			break ;
		}
	}
	output->setText(ss.str().c_str());
	
	// disabling velocity combo box
	speed_combo->setVisible(false);
}

void	SolveView::close()
{
	if (second_window)
		second_window->setVisible(false);
}
