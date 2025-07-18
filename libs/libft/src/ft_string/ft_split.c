/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:53:34 by hiennguy          #+#    #+#             */
/*   Updated: 2025/02/10 19:56:33 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/libft.h"

static int	count_words(const char *s, char c)
{
	int	in_word;
	int	count;

	in_word = 0;
	count = 0;
	while (*s != '\0')
	{
		if (*s != c)
		{
			if (in_word == 0)
			{
				in_word = 1;
				count++;
			}
		}
		else if (*s == c)
			in_word = 0;
		s++;
	}
	return (count);
}

static char	*copy_word(const char *s, int start, char c)
{
	int		end;
	int		word_len;
	int		i;
	char	*word;

	end = start;
	while (s[end] != '\0' && s[end] != c)
		end++;
	word_len = end - start;
	word = malloc((word_len + 1) * sizeof(char));
	if (word == NULL)
		return (NULL);
	i = 0;
	while (i < word_len)
	{
		word[i] = s[start + i];
		i++;
	}
	word[word_len] = '\0';
	return (word);
}

static void	cleanup(char **all_words)
{
	int	i;

	i = 0;
	while (all_words[i] != NULL)
	{
		free(all_words[i]);
		i++;
	}
	free(all_words);
}

static int	initialize(char const *s, char c, char **all_words)
{
	int	start;
	int	index;

	start = 0;
	index = 0;
	while (s[start] != '\0')
	{
		while (s[start] == c)
			start++;
		if (s[start] == '\0')
			break ;
		all_words[index] = copy_word(s, start, c);
		if (all_words[index] == NULL)
			return (0);
		index++;
		while (s[start] != '\0' && s[start] != c)
			start++;
	}
	all_words[index] = NULL;
	return (1);
}

char	**ft_split(char const *s, char c)
{
	const int	word_count = count_words(s, c);
	char		**all_words;

	all_words = malloc((word_count + 1) * sizeof(char *));
	if (!all_words)
		return (NULL);
	if (initialize(s, c, all_words) == 0)
	{
		cleanup(all_words);
		return (NULL);
	}
	return (all_words);
}
