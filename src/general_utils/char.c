/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olacerda <olacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 07:33:02 by olacerda          #+#    #+#             */
/*   Updated: 2026/03/04 07:39:01 by olacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <utils.h>

int	is_numerical(char xar)
{
	if (!xar)
		return (0);
	if ((xar >= '0') && (xar <= '9'))
		return (1);
	return (0);
}

int	is_alphanumerical_or_underline(char xar)
{
	if (!xar)
		return (0);
	if (((xar >= 'A') && (xar <= 'Z')) || ((xar >= 'a') && (xar <= 'z'))
		|| ((xar >= '0') && (xar <= '9')) || (xar == '_'))
		return (1);
	return (0);
}