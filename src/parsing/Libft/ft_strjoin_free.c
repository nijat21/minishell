/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_free.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 18:48:33 by nismayil          #+#    #+#             */
/*   Updated: 2026/03/08 19:17:31 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin_free(char *s1, char *s2)
{
	char		*str;
	const char	*src_s1;
	const char	*src_s2;

	if (!s1 && !s2)
		return (NULL);
	src_s1 = s1;
	src_s2 = s2;
	if (!src_s1)
		src_s1 = "";
	if (!src_s2)
		src_s2 = "";
	str = ft_strjoin(src_s1, src_s2);
	if (!str)
		return (NULL);
	free(s1);
	free(s2);
	return (str);
}
