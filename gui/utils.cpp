/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 16:48:17 by matteo            #+#    #+#             */
/*   Updated: 2024/04/07 16:57:29 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

void	NPuzzle::addStyleSheet(QApplication& app)
{
	const QString	styleSheetPath = ":stylesheet/style";
	QString			styleSheet;

	Q_ASSERT_X(
		QFileInfo(styleSheetPath).isFile(),
		Q_FUNC_INFO,
		"No stylesheet found"
	);
	
	QFile	f(styleSheetPath);
	f.open(QFile::ReadOnly);

	styleSheet = f.readAll();

	app.setStyleSheet(styleSheet);
}
