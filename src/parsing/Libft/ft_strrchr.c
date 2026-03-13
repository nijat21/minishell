/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 13:26:16 by nismayil          #+#    #+#             */
/*   Updated: 2024/11/21 15:49:37 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t	str_len;
	char	*str;

	str = (char *)s;
	str_len = ft_strlen(str);
	if ((unsigned char)c == '\0')
		return (&str[str_len]);
	while (str_len > 0)
	{
		str_len--;
		if ((unsigned char)c == (unsigned char)str[str_len])
			return (&str[str_len]);
	}
	return (NULL);
}
