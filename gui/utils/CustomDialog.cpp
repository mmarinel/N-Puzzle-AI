/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CustomDialog.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 15:48:11 by matteo            #+#    #+#             */
/*   Updated: 2024/04/18 19:13:24 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CustomDialog.hpp"
#include <QLabel>
#include <QDialogButtonBox>
#include <QLayout>

CustomDialog::CustomDialog(
	const QString& msg,
	QDialogButtonBox::StandardButtons flags,
	QWidget* parent
): QDialog(parent)
{
	QLabel*				label = new QLabel(msg);
	QDialogButtonBox*	b = new QDialogButtonBox(flags);

	connect(
		b, &QDialogButtonBox::accepted,
		this, &QDialog::accept
	);
	connect(
		b, &QDialogButtonBox::rejected,
		this, &QDialog::reject
	);

	this->setLayout(new QVBoxLayout());

	this->layout()->addWidget(label);
	static_cast<QVBoxLayout*>(this->layout())->addSpacing(10);
	this->layout()->addWidget(b);
}
