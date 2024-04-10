/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Window.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 20:24:44 by matteo            #+#    #+#             */
/*   Updated: 2024/04/10 18:00:07 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <QWidget>
#include <QStackedWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

#include "MenuView.hpp"

#define WIDTH 900
#define HEIGHT 800

class Window: public QWidget
{
	Q_OBJECT
private:
	QVBoxLayout*	area;
	QStackedWidget*	content;
	QWidget*		menu_page;
	QWidget*		solve_page;
	QWidget*		notSolvable_page;
	QWidget*		solving_page;
	QWidget*		goal_page;
	QPushButton*	forward_btn;
	QPushButton*	backward_btn;

	MenuView*		menu;
	QPushButton*	move_forward;
	QPushButton*	move_backward;
	// QGridLayout*	board;
	// QHBoxLayout*	buttons_area;
	// QPushButton*	play;

public:
	Window();
	~Window();
};
