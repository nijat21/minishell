/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_unsigned_number.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:25:17 by nismayil          #+#    #+#             */
/*   Updated: 2025/01/22 23:02:41 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void ft_putnbr_unsigned_fd(unsigned int n, int *count, int fd)
{
	char temp;

	if (fd < 0)
		return;
	if (n >= 10)
		ft_putnbr_unsigned_fd(n / 10, count, fd);
	temp = n % 10 + '0';
	write(fd, &temp, 1);
	(*count)++;
}

int print_unsigned_number(unsigned int n)
{
	int count;

	count = 0;
	ft_putnbr_unsigned_fd(n, &count, 1);
	return (count);
}
