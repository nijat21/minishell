/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin3_free.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 19:01:37 by nismayil          #+#    #+#             */
/*   Updated: 2026/03/08 19:20:49 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin3_free(char *s1, char *s2, char *s3)
{
	char		*str;
	const char	*src_s1;
	const char	*src_s2;
	const char	*src_s3;

	if (!s1 && !s2 && !s3)
		return (NULL);
	src_s1 = s1;
	src_s2 = s2;
	src_s3 = s3;
	if (!src_s1)
		src_s1 = "";
	if (!src_s2)
		src_s2 = "";
	if (!src_s3)
		src_s3 = "";
	str = ft_strjoin3(src_s1, src_s2, src_s3);
	if (!str)
		return (NULL);
	free(s1);
	free(s2);
	free(s3);
	return (str);
}
