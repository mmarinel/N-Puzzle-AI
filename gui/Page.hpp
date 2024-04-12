/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Page.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 21:58:47 by matteo            #+#    #+#             */
/*   Updated: 2024/04/11 19:28:40 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <QWidget>

class Page: public QWidget
{
	Q_OBJECT
public:
	int		index;//Index in parent layout or StackedWidget
};
