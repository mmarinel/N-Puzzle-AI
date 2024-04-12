/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SolveView.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 19:11:22 by matteo            #+#    #+#             */
/*   Updated: 2024/04/11 19:18:23 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <BoardView.hpp>
#include <QTextEdit>

class SolveView: public QVBoxLayout
{
	Q_OBJECT
private:
	BoardView*		board;
	QPushButton*	solve_btn;
	QTextEdit*		output;
public:
	SolveView();
	~SolveView();
};
