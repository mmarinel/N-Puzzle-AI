/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CustomDialog.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 15:47:52 by matteo            #+#    #+#             */
/*   Updated: 2024/04/13 15:48:03 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <QDialog>

class CustomDialog: public QDialog
{
	Q_OBJECT
	private:
		std::string		msg;
	
	public:
		CustomDialog(const QString& msg, QWidget* parent = nullptr );
};
