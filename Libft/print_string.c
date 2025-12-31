/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_string.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:25:37 by nismayil          #+#    #+#             */
/*   Updated: 2025/01/22 23:02:29 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int print_string(char *str)
{
	if (!str)
		str = "(null)";
	if (*str == '\0')
		return (0);
	ft_putstr_fd(str, 1);
	return (ft_strlen(str));
}
