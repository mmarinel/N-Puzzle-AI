/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gui_utils.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 16:42:04 by matteo            #+#    #+#             */
/*   Updated: 2024/05/26 00:06:44 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <QApplication>
#include <QWidget>
#include <QStringList>
#include <QFile>
#include <QFileInfo>
#include <QtGlobal>

#include "Content.hpp"

namespace NPuzzle
{
void	addStyleSheet(QApplication& app);

}