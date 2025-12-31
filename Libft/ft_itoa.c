/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 13:17:35 by nismayil          #+#    #+#             */
/*   Updated: 2024/11/21 13:17:37 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_digits(long tp)
{
	size_t	len;

	len = 0;
	if (tp <= 0)
	{
		tp *= -1;
		len++;
	}
	while (tp > 0)
	{
		tp /= 10;
		len++;
	}
	return (len);
}

static char	*create_string(long temp, size_t len)
{
	char	*arr;
	size_t	i;

	arr = malloc((len + 1) * sizeof(char));
	if (!arr)
		return (NULL);
	arr[len] = '\0';
	i = len - 1;
	if (temp == 0)
		arr[0] = '0';
	else if (temp < 0)
	{
		temp *= -1;
		arr[0] = '-';
	}
	while (temp > 0)
	{
		arr[i] = temp % 10 + '0';
		i--;
		temp /= 10;
	}
	return (arr);
}

char	*ft_itoa(int n)
{
	char	*arr;
	long	temp;
	size_t	len;

	temp = n;
	len = 0;
	len = count_digits(temp);
	arr = create_string(temp, len);
	return (arr);
}
