/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 16:42:04 by matteo            #+#    #+#             */
/*   Updated: 2024/04/10 21:53:44 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <QApplication>
#include <QWidget>
#include <QStringList>
#include <QFile>
#include <QFileInfo>

#include "Content.hpp"

namespace NPuzzle
{
void	addStyleSheet(QApplication& app);

void	addPage(Content* content, QWidget* page);

}