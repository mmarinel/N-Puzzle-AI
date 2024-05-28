/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SolveView.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 19:11:22 by matteo            #+#    #+#             */
/*   Updated: 2024/05/28 20:11:09 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <QGuiApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QStackedWidget>
#include <BoardView.hpp>
#include <QTextEdit>
#include <QMovie>
#include <QTimer>

#include "Content.hpp"
#include "Agent.hpp"

#include <ctime>

#define THRESHOLD_FOR_NEW_WINDOW 9
#define THRESHOLD_FOR_NO_GRID 96

class SolveView: public QVBoxLayout
{
	Q_OBJECT
private:
	BoardView*			board;
	QWidget*			second_window;
	BoardView*			new_win_board;
	QTextEdit*			output;
	QPushButton*		solve_btn;
	QPushButton*		playback_btn;
	QPushButton*		play_btn;
	QPushButton*		playforward_btn;
	QLabel*				loading_lbl;
	QMovie*				gif;

	bool				solving;
	bool				executing;

	QHBoxLayout*		board_box;
	QHBoxLayout*		output_box;
	QStackedWidget*		btns_stacked;
	QWidget*			solve_btn_box;
	QWidget*			play_box;
	QWidget*			loading_lbl_box;

	NPuzzle::Agent*		agent;
	struct timespec		before;
	struct timespec		after;
	QTimer				timer;
	Problem::Actions	doing;
	Problem::Actions	undoing;

public:
	SolveView(QWidget* parent=nullptr);
	~SolveView();

	bool			abort();
	void			start();
	void			close();
public slots:
	void			moveTile(
						Problem::Actions& pop_queue,
						Problem::Actions& push_queue
					);
	void			startSolving();
	void			play_stop();
	void			workDone();
};
