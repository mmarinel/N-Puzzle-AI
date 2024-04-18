/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Window.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 20:24:44 by matteo            #+#    #+#             */
/*   Updated: 2024/04/18 19:19:43 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <QWidget>
#include <QDialogButtonBox>
#include <QStackedWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

#include "CustomDialog.hpp"
#include "MenuView.hpp"
#include "SolveView.hpp"
#include "Page.hpp"
#include "Content.hpp"

#include <QThread>

#define WIDTH 900
#define HEIGHT 800

class Window: public QWidget
{
	Q_OBJECT
private:
	QVBoxLayout*	area;
	Content*		content;
	Page*			menu_page;
	Page*			solve_page;
	Page*			notSolvable_page;
	Page*			solving_page;
	Page*			goal_page;
	QPushButton*	forward_btn;
	QPushButton*	backward_btn;

	CustomDialog*	notice;

public:
	Window();
	~Window();

public slots:
	void	forward();
	void	backward();
};
