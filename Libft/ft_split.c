/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 13:22:36 by nismayil          #+#    #+#             */
/*   Updated: 2024/11/21 13:22:38 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_words(char const *s, char c)
{
	size_t	in_word;
	size_t	count;

	in_word = 0;
	count = 0;
	while (*s)
	{
		if (*s != c)
		{
			if (!in_word)
			{
				count++;
				in_word = 1;
			}
		}
		else
			in_word = 0;
		s++;
	}
	return (count);
}

static char	*allocate_assign(char ***res, char const *start, size_t len,
		size_t i)
{
	size_t	j;

	j = 0;
	(*res)[i] = (char *)malloc((len + 1) * sizeof(char));
	if (!(*res)[i])
	{
		while ((*res)[--i])
			free((*res)[i]);
		free((*res));
		return (NULL);
	}
	while (start[j] && j < len)
	{
		(*res)[i][j] = start[j];
		j++;
	}
	(*res)[i][j] = '\0';
	return ((*res)[i]);
}

static void	iterate_word(char ***res, char const **s, char c, size_t *i)
{
	size_t		len;
	char const	*start;

	start = *s;
	len = 0;
	while (**s && **s != c)
	{
		len++;
		(*s)++;
	}
	(*res)[*i] = allocate_assign(res, start, len, *i);
	(*i)++;
}

char	**ft_split(char const *s, char c)
{
	char	**res;
	size_t	i;

	if (!s)
		return (NULL);
	res = (char **)malloc((count_words(s, c) + 1) * sizeof(char *));
	if (!res)
		return (NULL);
	i = 0;
	while (*s)
	{
		if (*s != c)
		{
			iterate_word(&res, &s, c, &i);
		}
		else
			s++;
	}
	res[i] = NULL;
	return (res);
}
