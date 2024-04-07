/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Window.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 20:24:44 by matteo            #+#    #+#             */
/*   Updated: 2024/04/07 15:56:25 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <QWidget>
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
	QPushButton*	forward_btn;
	QPushButton*	backward_btn;

	QVBoxLayout*	ui_area;
	MenuView*		menu;
	QGridLayout*	board;
	QHBoxLayout*	buttons_area;
	QPushButton*	play;
	QPushButton*	move_forward;
	QPushButton*	move_backward;

public:
	Window();
	~Window();
};
