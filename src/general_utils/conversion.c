/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conversion.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 07:32:14 by olacerda          #+#    #+#             */
/*   Updated: 2026/03/25 04:41:55 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <utils.h>
#include <built-ins.h>

char	*int_to_ascii(int nbr)
{
	char *res;
	int		size;
	long	temp;
	int		minus;

	size = 0;
	temp = 0;
	minus = 1;
	if ((nbr < 0) && ++(size))                                          
		minus = -1;
	while (++(size) && (nbr != 0))
	{
		temp = ((temp * 10) + ((nbr % 10) * (minus * ((nbr % 10) != 0))));
		nbr /= 10;
	}
	res = malloc((size + 1) * sizeof(char));
	if(!res)
		return (NULL);
	nbr = -1 + (minus == -1);
	while ((++nbr < (size + 1)) && ((temp != 0) || (temp == 0 && nbr == 0)))
	{
		res[nbr] = ((temp % 10) + 48);
		temp /= 10;
	}
	return (res[nbr] = '\0', (void)((minus == -1) && (res[0] = '-')), res);
}

int	ascii_to_int(char *string)
{
	long	result;
	int	minus;
	int	idx;

	if (!string)
		return (-1);
	idx = 0;
	while ((string[idx]) && ((string[idx] >= 9 &&
		(string[idx] <= 13)) || (string[idx] == 32)))
		idx++;
	minus = 0;
	if ((string[idx] == '+') || (string[idx] == '-'))
	{
		if (string[idx] == '-')
			minus = 1;
		idx++;
	}
	result = 0;
	while ((string[idx]) && ((string[idx] >= '0') && (string[idx] <= '9')))
		result = ((result * 10) + (string[idx++] - 48));
	if (minus == true)
		result = -result;
	if ((result < (1 << 31)) || (result > ((1L << 31) - 1)))
		return (-1);
	return (result);
}

int	ascii_to_long(char *string)
{
	long	result;
	int	minus;
	int	index;

	if (!string || (is_overflow_long(string) == true))
		return (-1);
	index = 0;
	while ((string[index]) && ((string[index] >= 9 
		&& (string[index] <= 13)) || (string[index] == 32)))
		index++;
	minus = 0;
	if ((string[index] == '+') || (string[index] == '-'))
	{
		if (string[index] == '-')
			minus = 1;
		index++;
	}
	result = 0;
	while ((string[index]) && ((string[index] >= '0')
		&& (string[index] <= '9')))
		result = ((result * 10) + (string[index++] - 48));
	if (minus == true)
		result = -result;
	return (result);
}
