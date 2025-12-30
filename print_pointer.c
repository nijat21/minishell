/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_pointer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:25:02 by nismayil          #+#    #+#             */
/*   Updated: 2025/01/22 23:02:16 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void ft_hex_base16(uintptr_t num, int fd, int *count, char *base)
{
	if (num >= 16)
		ft_hex_base16(num / 16, fd, count, base);
	write(fd, &base[num % 16], 1);
	(*count)++;
}

static int ft_print_pointer_fd(void *ptr, int fd)
{
	uintptr_t address;
	int count;

	count = 0;
	address = (uintptr_t)ptr;
	if (address == 0)
	{
		write(fd, "(nil)", 5);
		count += 5;
	}
	else
	{
		count = 2;
		write(fd, "0x", 2);
		ft_hex_base16(address, fd, &count, "0123456789abcdef");
	}
	return (count);
}

int print_pointer(void *ptr)
{
	return (ft_print_pointer_fd(ptr, 1));
}
