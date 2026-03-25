/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 02:46:06 by username          #+#    #+#             */
/*   Updated: 2026/03/25 03:14:51 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "minishell.h"

//-allocation.c ----------------------------------------------------------------
char	*string_duplicate(char *string);
void	*re_allocker(void *pointer, long size, long new_size, long type_size);
char	**split_line(char *string);
int		realloc_appender(char **string, char *to_append);

//-allocation_free.c -----------------------------------------------------------
int		free_array_string(char **array, int size); // Size '0' (zero) for default.
void	del(void *content);

//-char.c ----------------------------------------------------------------------
int	is_alphanumerical_or_underline(char xar);
int	is_numerical(char xar);

//-conversion.c ----------------------------------------------------------------
int		ascii_to_int(char *string);
char	*int_to_ascii(int nbr); // Careful, MALLOC'ed string. Free it.
int		ascii_to_long(char *string);

//-error.c ---------------------------------------------------------------------
void	put_error(char *problem);
void	put_comand_error(char *comand, char *problem);
void	put_multiple_error(char **elements, char *problem);

//-get_next_line.c -------------------------------------------------------------
char	*get_next_line(int fd);

//-string_advanced.c -----------------------------------------------------------
int		string_compare(char *str1, char *str2);
int		string_cat(char *string1, int size, char *string2);
int		string_swap(char **str1, char **str2);
int		array_string_lenght(char **args);
void	print_nbr(int number);

//-string_basic.c --------------------------------------------------------------
int	string_zero(char *string, int size);
int	string_print(char *string, int have_break_line);
int	string_length(char *string);
int	string_copy(char *str1, char *str2);
int	string_have_equal(char *string);

#endif
