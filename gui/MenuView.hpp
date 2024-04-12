/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MenuView.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 12:56:45 by matteo            #+#    #+#             */
/*   Updated: 2024/04/10 22:01:19 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>

#include "heuristics.hpp"

#define CHOOSE_FILE_TEXT "choose board"

class MenuView: public QVBoxLayout
{
	Q_OBJECT
private:
	QPushButton*			choose_file;
	QCheckBox*				choose_random;
	QComboBox*				choose_heuristic;
	QLabel*					heurstic_lbl;

public slots:
	void	setBoardFile();
	void	setAtRandomFile();
	void	setHeuristic(int index);

public:
	MenuView();
	~MenuView();

	friend class Window;
};
