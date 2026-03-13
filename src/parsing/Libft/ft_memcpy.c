/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 13:20:38 by nismayil          #+#    #+#             */
/*   Updated: 2024/11/21 17:27:07 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*source;
	unsigned char	*destination;

	if (dst == src)
		return (NULL);
	source = (unsigned char *)src;
	destination = (unsigned char *)dst;
	i = 0;
	while (i < n)
	{
		destination[i] = source[i];
		i++;
	}
	return (dst);
}
