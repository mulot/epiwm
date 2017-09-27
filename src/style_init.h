/*
**  EPIwm
**  Copyright (C) 1999 Julien Mulot, Thomas Poindessous, Stephane Rault
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation; either version 2 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
**
** style_init.h
** 
** Made by ()
** Login   <mulot_j@epita.fr>
** 
** Started on  Mon May 31 02:03:42 1999 
 ** Last update Wed Aug 11 19:40:57 1999 Julien Mulot
*/

#ifndef   __STYLE_INIT_H__
#define   __STYLE_INIT_H__

#define NO_TITLE 0x01
#define NO_BORDER 0x02
#define STICKY 0x04
#define STAY_ON_TOP 0x08
#define WINDOW_LIST_SKIP 0x10

typedef struct		s_apps_prop
{
  char			*name;
  char			prop;
  struct s_apps_prop	*next;
}			t_apps_prop;

typedef struct		s_stay_on_top
{
  t_win			*win;
  struct s_stay_on_top	*next;
}			t_stay_on_top;

void	check_app_prop(t_win *, char *);
void	init_properties(char *, char *);
char	*read_app_prop(char *);
int	search_app_prop(char *);
void	style_build(FILE *);

t_apps_prop	*gl_apps_prop;
t_stay_on_top	*gl_stay_on_top;

#endif /* __STYLE_INIT_H__ */
