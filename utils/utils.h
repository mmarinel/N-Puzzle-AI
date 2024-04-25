/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matteo <matteo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 22:17:46 by matteo            #+#    #+#             */
/*   Updated: 2024/04/25 16:41:06 by matteo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <QString>

namespace NPuzzle
{

bool	parse_file(QString filepath);
void	generate_board();

}

