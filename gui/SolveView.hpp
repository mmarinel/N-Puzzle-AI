/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SolveView.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 19:11:22 by matteo            #+#    #+#             */
/*   Updated: 2024/05/10 18:48:48 by matteo           ###   ########.fr       */
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

#include "Content.hpp"
#include "AStar.hpp"

#define THRESHOLD_FOR_NEW_WINDOW 9
#define THRESHOLD_FOR_NO_GRID 96

class SolveView: public QVBoxLayout
{
	Q_OBJECT
private:
	BoardView*		board;
	QWidget*		second_window;
	BoardView*		new_win_board;
	QTextEdit*		output;
	QPushButton*	solve_btn;
	QPushButton*	playback_btn;
	QPushButton*	play_btn;
	QPushButton*	playforward_btn;

	bool			solving;
	bool			executing;

	QHBoxLayout*	board_box;
	QHBoxLayout*	output_box;
	QStackedWidget*	btns_stacked;
	QWidget*		solve_btn_box;
	QWidget*		play_box;
	NPuzzle::Agent*	agent;

	void			moveTile();
public:
	SolveView(QWidget* parent=nullptr);
	~SolveView();

	bool			abort();
	void			start();
	void			close();
public slots:
	void	startSolving();
	void	play_stop();
};
