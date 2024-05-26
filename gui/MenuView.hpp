/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MenuView.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 12:56:45 by matteo            #+#    #+#             */
/*   Updated: 2024/05/26 12:08:56 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
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
	QComboBox*				choose_heuristic;
	QLabel*					heurstic_lbl;
	QLabel*					size_lbl;

public slots:
	void	setBoardFile();
	void	setAtRandomFile();
	void	setHeuristic(int index);
	void	setRandomGeneratedSize(int n);

public:
	MenuView(QWidget* parent=nullptr);
	~MenuView();

	bool	canGoForward(QString& msg);

	friend class Window;
};
