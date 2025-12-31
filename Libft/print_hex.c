/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_hex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:24:31 by nismayil          #+#    #+#             */
/*   Updated: 2025/01/22 23:01:57 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void ft_puthex_fd(unsigned int num, int fd, int *count, char *base)
{
	if (num >= 16)
		ft_puthex_fd(num / 16, fd, count, base);
	write(fd, &base[num % 16], 1);
	(*count)++;
}

int print_hex(unsigned int n, char *base)
{
	int count;

	count = 0;
	ft_puthex_fd(n, 1, &count, base);
	return (count);
}
