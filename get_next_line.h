/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angerard <angerard@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:49:54 by angerard          #+#    #+#             */
/*   Updated: 2024/05/21 17:35:09 by angerard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# include <fcntl.h>
# include <stdlib.h>
# include <sys/types.h>
# include <unistd.h>

typedef struct s_list
{
	char			*content;
	struct s_list	*next;
}					t_list;

char				*get_next_line(int fd);
void				read_stash(int fd, t_list **stash);
int					found_newline(t_list *stash);
t_list				*ft_lst_get_last(t_list *stash);
void				add_to_stash(t_list **stash, char *buff, int readed);
void				extract_line(t_list *stash, char **line);
void				generate_line(char **line, t_list *stash);
void				clear_stash(t_list **stash);
size_t				ft_strlen(const char *s);
void				free_stash(t_list *stash);

#endif
