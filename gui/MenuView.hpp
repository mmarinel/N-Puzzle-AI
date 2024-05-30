/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MenuView.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cy4gate_mmarinelli <cy4gate_mmarinelli@    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 12:56:45 by matteo            #+#    #+#             */
/*   Updated: 2024/05/30 18:13:14 by cy4gate_mma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QRadioButton>
#include <QLabel>

#include "heuristics.hpp"

#define CHOOSE_FILE_TEXT "choose board"

class MenuView: public QVBoxLayout
{
	Q_OBJECT
private:
	QPushButton*			choose_file;
	QCheckBox*				choose_random;
	QSpinBox*				choose_size;
	QRadioButton*			choose_solvability;
	QComboBox*				choose_heuristic;
	QLabel*					heurstic_lbl;
	QLabel*					size_lbl;

public slots:
	void	setBoardFile();
	void	setAtRandomFile();
	void	setHeuristic(int index);
	void	setRandomGeneratedSize(int n);
	void	toggleRandomSolvable();

public:
	MenuView(QWidget* parent=nullptr);
	~MenuView();

	bool	canGoForward(QString& msg);
	void	reset();

	friend class Window;
};
