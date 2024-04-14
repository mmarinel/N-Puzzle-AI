/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SolveView.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 19:11:22 by matteo            #+#    #+#             */
/*   Updated: 2024/04/14 22:24:43 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <QGuiApplication>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <BoardView.hpp>
#include <QTextEdit>

#define THRESHOLD_FOR_NEW_WINDOW 9
#define THRESHOLD_FOR_NO_GRID 96

class SolveView: public QVBoxLayout
{
	Q_OBJECT
private:
	BoardView*		board;
	QWidget*		second_window;
	BoardView*		new_win_board;
	QPushButton*	solve_btn;
	QTextEdit*		output;
	QPushButton*	play_btn;
	QPushButton*	playback_btn;
	QPushButton*	playforward_btn;
	bool			solving;
	bool			executing;

	QHBoxLayout*	board_box;
	QHBoxLayout*	solve_btn_box;
	QHBoxLayout*	output_box;
	QHBoxLayout*	play_box;
public:
	SolveView();
	~SolveView();

public slots:
	void	startSolving();
	void	play_stop();
	void	abort();
};
