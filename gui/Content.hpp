/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Content.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 19:44:42 by matteo            #+#    #+#             */
/*   Updated: 2024/04/21 23:07:18 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <QStackedWidget>
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QVBoxLayout>

class Content: public QStackedWidget
{
	Q_OBJECT
public:
/**
 * @brief WARNING: this adds a wrapped widget, not the one passed !!
 * 
 * @param parent 
 */
	Content(QWidget* parent = nullptr);
	
	int	addWidget(QWidget* widget);
};
