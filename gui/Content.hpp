/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Content.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 19:44:42 by matteo            #+#    #+#             */
/*   Updated: 2024/04/10 20:09:44 by matteo           ###   ########.fr       */
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
	
	int	addWidget(QWidget* widget);
};
