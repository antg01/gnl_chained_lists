/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angerard <angerard@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:58:25 by angerard          #+#    #+#             */
/*   Updated: 2024/05/22 11:58:50 by angerard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static t_list	*stash;
	char			*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, &line, 0) < 0)
	{
		clear_static_stash(&stash);
		return (NULL);
	}
	if (!stash)
		stash = NULL;
	line = NULL;
	read_stash(fd, &stash);
	if (!stash)
		return (NULL);
	extract_line(stash, &line);
	clear_stash(&stash);
	if (!line || line[0] == '\0')
	{
		free(line);
		free_stash(stash);
		stash = NULL;
		return (NULL);
	}
	return (line);
}

/* Uses read() to add characters to the stash */
void	read_stash(int fd, t_list **stash)
{
	char	*buff;
	int		readed;

	readed = 1;
	while (!found_newline(*stash) && readed != 0)
	{
		buff = malloc(sizeof(char) * (BUFFER_SIZE + 1));
		if (!buff)
			return ;
		readed = read(fd, buff, BUFFER_SIZE);
		if (readed <= 0)
		{
			free(buff);
			return ;
		}
		buff[readed] = '\0';
		add_to_stash(stash, buff, readed);
		free(buff);
	}
}

/* Adds the content of our bufffer to the end of our stash */
void	add_to_stash(t_list **stash, char *buff, int readed)
{
	int		i;
	t_list	*last;
	t_list	*new_node;

	new_node = malloc(sizeof(t_list));
	if (!new_node)
		return ;
	new_node->next = NULL;
	new_node->content = malloc(sizeof(char) * readed + 1);
	if (new_node->content == NULL)
		return ;
	i = 0;
	while (buff[i] && i < readed)
	{
		new_node->content[i] = buff[i];
		i++;
	}
	new_node->content[i] = '\0';
	if (!*stash)
	{
		*stash = new_node;
		return ;
	}
	last = ft_lst_get_last(*stash);
	last->next = new_node;
}

/* Extracts all characters from our stash and stores them in out line.
stopping after the 1st \n  */
void	extract_line(t_list *stash, char **line)
{
	int	i;
	int	j;

	if (!stash)
		return ;
	generate_line(line, stash);
	if (!*line)
		return ;
	j = 0;
	while (stash)
	{
		i = 0;
		while (stash->content[i])
		{
			if (stash->content[i] == '\n')
			{
				(*line)[j++] = stash->content[i];
				break ;
			}
			(*line)[j++] = stash->content[i++];
		}
		stash = stash->next;
	}
	(*line)[j] = '\0';
}

/* After extracting the line that was read,
	we dont need those characters anymore.
This function clears the stash so only the characters
that have not been returned at the end of	get_next_line(void)
remain in our static var  */

void	clear_stash(t_list **stash)
{
	t_list	*last;
	t_list	*clean_node;
	int		i;
	int		j;

	clean_node = malloc(sizeof(t_list));
	if (!stash || !clean_node)
		return ;
	clean_node->next = NULL;
	last = ft_lst_get_last(*stash);
	i = 0;
	while (last->content[i] && last->content[i] != '\n')
		i++;
	if (last->content && last->content[i] == '\n')
		i++;
	clean_node->content = malloc(sizeof(char) * ((ft_strlen(last->content) - i)
				+ 1));
	if (clean_node->content == NULL)
		return ;
	j = 0;
	while (last->content[i])
		clean_node->content[j++] = last->content[i++];
	clean_node->content[j] = '\0';
	free_stash(*stash);
	*stash = clean_node;
}
