/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 12:58:27 by nismayil          #+#    #+#             */
/*   Updated: 2025/12/15 12:58:28 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char *ft_strjoin3(char const *s1, char const *s2, char const *s3)
{
    char *str;
    char *start;

    if (!s1 && !s2 && !s3)
        return (NULL);
    if (!s1)
        s1 = "";
    if (!s2)
        s2 = "";
    if (!s3)
        s3 = "";
    str = malloc(ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3) + 1);
    if (!str)
        return (NULL);
    start = str;
    while (*s1)
        *str++ = *s1++;
    while (*s2)
        *str++ = *s2++;
    while (*s3)
        *str++ = *s3++;
    *str = '\0';
    return (start);
}