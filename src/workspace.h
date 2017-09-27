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
 ** workspace.h
 ** 
 ** Made by (Poindessous Thomas)
 ** Login   <poinde_t@epita.fr>
 ** 
 ** Started on  Fri Apr  2 15:48:02 1999 Poindessous Thomas
 ** Last update Mon Sep 13 17:28:47 1999 mulot
 */

#ifndef   __WORKSPACE_H__
#define   __WORKSPACE_H__

typedef struct		s_workspace
{
  char			*name;
  struct s_workspace	*next;
  struct s_workspace	*prev;
  t_icon_list		*first_icon;
  t_icon_list		*last_icon;
}			t_workspace;

void			workspace_new(char *);
void			workspace_remove();
void			send_window_to_workspace(t_win *, int);
void			workspace_build(FILE *);
void			wait_before_change_workspace(char);
int			compare_mouse_position(char, int, int);
void			go_to_pager(char, int, int, int);
void			set_cursor_pager(char);
void			move_all_windows(int, int);
int			window_workspace(t_win *);
void			go_to_nth_workspace(int);
void			workspace_position(t_win *,  int *, int *);
void			set_workspace_menu_width(char *);

int			gl_workspace_change_percent;
int			gl_workspace_time_change;
int			gl_workspace_to_send;
int			gl_workspace_cycle;
#endif /* __WORKSPACE_H__ */








