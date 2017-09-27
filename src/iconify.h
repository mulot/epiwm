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
 ** iconify.h
 ** 
 ** Made by (Poindessous Thomas)
 ** Login   <poinde_t@epita.fr>
 ** 
 ** Started on  Wed Mar  3 15:46:58 1999 Poindessous Thomas
 ** Last update Tue Sep 21 11:20:46 1999 mulot
 */

#ifndef   __ICONIFY_H__
#define   __ICONIFY_H__

typedef struct	s_icon
{
#define	TOP_LEFT 1
#define	TOP_RIGHT 2
#define	BOTTOM_LEFT 3
#define	BOTTOM_RIGHT 4
#define	LEFT_TOP 5
#define	LEFT_BOTTOM 6
#define	RIGHT_TOP 7
#define	RIGHT_BOTTOM 8
  
  int		width;
  int		height;
  char		*image_active;
  char		*image_inactive;
  char		position;
  char		showed;
#ifdef	  IMLIB
  ImlibImage	*image;
#endif
  XColor	color_active;
  XColor	color_inactive;
  XFontStruct	*font;
  GC		font_active;
  GC		font_inactive;
}		t_icon;

typedef struct		s_icon_list
{
  struct s_icon_list	*next;
  struct s_icon_list	*prec;
  t_win			*win;
  char			nb;
  int			width;
  int			height;
}			t_icon_list;

void		draw_icon(Window, int, char *);
void		draw_icon_image(Window, char *);
t_icon_list	*iconify_search_elt(t_win *);
void		iconify_re_number(t_icon_list *);
void		iconify_re_draw(t_icon_list *);
void		iconify_add_elt(t_win *, int, int);
void		iconify_rm_elt(t_win *);
Window		add_iconify_win(t_win *);
Window		icon_create_win(int, int, int, int);
void		icon_calculate_width_height(t_win *, int *, int *);

t_icon		gl_icon;

#endif /* __ICONIFY_H__ */
