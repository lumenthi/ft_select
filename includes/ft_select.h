/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/08 11:22:09 by lumenthi          #+#    #+#             */
/*   Updated: 2018/03/15 22:33:41 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SELECT_H
# define FT_SELECT_H

#define GREEN	"\033[1;32m"
#define BLUE	"\033[4;34m"
#define BLANK	"\033[0m"
#define RED		"\033[1;31m"

typedef struct		s_cursor
{
	int	x;
	int	y;
}					t_cursor;

typedef struct		s_properties
{
	struct termios	*bu;
	int				ttyfd;
	int				w_row;
	int				w_col;
	t_cursor		*cursor;
	int				max_spaces;
	int				current;
}					t_properties;

typedef struct		s_elem
{
	char	*name;
	int		len;
	int		select;
}					t_elem;

t_properties	*data;
t_elem			**elems;

# include "../libft/libft.h"
# include <termios.h>
# include <term.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/ioctl.h>

#endif
