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
 ** menu.h
 ** 
 ** Made by (Julien Mulot)
 ** Login   <mulot_j@epita.fr>
 ** 
 ** Started on  Thu Feb 18 17:05:48 1999 Julien Mulot
 ** Last update Wed Apr  5 03:32:37 2000 Mulot Julien
 */

#ifndef   __MENU_H__
#define   __MENU_H__
#include "epiwm.h"

#define MENU_MINI_WIDTH 100
#define MENU_WIDTH_SPACE 27
#define MENU_HEIGHT_SPACE 8
#define MENU_TASK_MAX_WIDTH 200
#define CMD 1
#define FUNC 2
#define GOTO 1
#define SENDTO 2

typedef struct		s_menu
{
  int			width;
  short			height;
  short			font_head;
  int			task_width;
  int			prop_width_root;
  int			prop_width_win;
  int			workspace_width;
  char			**current_elt;
  char			**current_menu;
  char			*current_prop;
  char			*current_workspace;
  char			*lngst_str;
  char			*lngst_str_prop;
  char			grad_type;
  char			prop_type;
  char			workspace_type;
  char			here_prop;
  char			here_task;
  char			auto_menu;
  Window		*current_task;
  Window		menu_prop;
  Window		menu_task;
  Window		workspace;
  Pixel			*pixels;
  XFontStruct		*font;
  GC			font_inactive;
  GC			font_active;
  XColor		color;
#ifdef	  IMLIB
  ImlibImage		*image;
  char			*image_name;
#endif
}			t_menu;

typedef	struct		s_menu_task
{
  t_win			*elt;
  struct s_menu_task	*next_elt;
}			t_menu_task;

typedef	struct		s_menu_apps
{
  char			*elt;
  char			*cmd;
  void			(*func)();
  struct s_menu_apps	*next_elt;
  struct s_menu_apps	*menu;
}			t_menu_apps;

typedef struct		s_menu_pos
{
  char			*menu_name;
  char			here;
  char			stick;
  short			nb_elt;
  short			height;
  short			win_pos;
  Window   		win;
  t_menu_apps		**pos;
  struct s_menu_pos	*next;
}			t_menu_pos;

void	add_menu_elt(t_menu_pos **, char *, char *, char);
void	add_menu_sub(t_menu_apps **, t_menu_pos **, char *);
void	add_menu_task_elt(t_win *);
void	remove_menu_task_elt(t_win *);
void	destroy_menu(t_menu_pos **, char *);
void	draw_image_menu(Window);
void	draw_menu(t_menu_apps **, t_menu_pos **);
void	draw_menu_prop(char);
void	draw_menu_task();
void	draw_menu_workspace();
void	draw_triangle(Window, int, int, char *);
char	*extract_name(char *);
char	*extract_str(char *, char *);
void	destroy_other_menus(t_menu_apps *, char *);
void	elt_detect(char *, char *);
void	menu_apps(char *, int, int);
void	menu_app_update(t_menu_apps **, t_menu_pos **);
void	menu_click(t_menu_pos *, int, unsigned int);
void	menu_build(FILE *);
void	menu_detect(char **, char *, char *);
void	menu_display(Window, int, int, unsigned int);
void	menu_move(t_menu_pos *, int);
void	menu_prop(t_win *, int, int, char);
void	menu_prop_move(int);
int	menu_set_height(t_menu_pos *);
void	menu_task(int, int);
void	menu_task_click(int);
void	menu_prop_click(int);
void	menu_workspace_click(int);
void	menu_task_move(int);
void	menu_task_update();
void	menu_prop_update();
void	menu_workspace(int, char *);
void	menu_workspace_move(int);
void	menu_workspace_update();
void	set_dim_menu();
void	sub_menu_detect(char *, char *);

t_menu_apps	*gl_menu_apps;
t_menu_task	*gl_menu_task;
t_menu		gl_menu;
t_menu_pos	*menu_pos;
#endif /* __MENU_H__ */
