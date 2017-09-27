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
** epiwm.h
** 
** Made by ()
** Login   <mulot_j@epita.fr>
** 
** Started on  Mon Dec 28 03:19:58 1998 
 ** Last update Mon Oct 23 21:46:12 2000 Julien Mulot
*/

#ifndef   __EPIWM_H__
#define   __EPIWM_H__
#ifdef HAVE_CONFIG_H
#include "../config.h"
#else
#define VERSION "0.5.6"
#ifdef LOC
#define CONFIG_PATH LOC
#endif
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#ifdef	  IMLIB
#include <Imlib.h>
#endif
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <X11/Xproto.h>
#include <X11/keysym.h>
#include <X11/Xresource.h>
#include <X11/Xatom.h>
#include <X11/extensions/shape.h>

#define ERROR	-1
#define OK	0
#define LINE_LEN	300

typedef struct	s_hints
{
  int		min_width;
  int		min_height;
  int		max_width;
  int		max_height;
  int		width_inc;
  int		height_inc;
}		t_hints;

typedef struct	s_win
{
#define MAX		0x01
#define MAX_WIDTH	0x02
#define MAX_HEIGHT	0x04
#define SHAPE		0x08
  Window	win;
  Window	parent;
  Window	border_frame;
  Window	button_close;
  Window	button_destroy;
  Window	button_iconify;
  Window	button_max;
  Window	button_shade;
  Window	button_max_width;
  Window	button_max_height;
  Window	iconify_win;
  Window	iconify_title_win;
  Window	titlebar_win;
  t_hints	hints;
  int		win_width;
  int		win_height;
  int		win_x;
  int		win_y;
  int		shade_width;
  int		shade_height;
  char		is_shade;
  /*
     char		is_max;
     char		is_max_width;
     char		is_max_height;
     char		is_win_shape;
     */
  char		size;
  char		state;
  char		prop;
  char		active;
  char		border_size;
  char		titlebar_height;
  char		bf_border;
  char		titlebar_border;
}		t_win;

typedef struct	s_win_list
{
  t_win		*p_t_win;
  struct s_win_list	*next_elt; 
}		t_win_list;

void		help();
void		event_type();
void		handle_events();
void		handle_window(Window);
void		options(int , char **);
void		chaos(); 
t_win		*add_border(Window);
t_win		*search_elt(Window);
void		remove_elt(Window);
void		add_elt(t_win *);
t_win		*create_win(Window);
void		add_signals();
void		tab_events();
int		rev_str(char *);

Atom		EPIWM_WM_STATE;
Atom		EPIWM_WM_CHANGE_STATE;
Atom		EPIWM_WM_PROTOCOLS;
Atom		EPIWM_WM_TAKE_FOCUS;
Atom		EPIWM_WM_DELETE_WINDOW;
Atom		EPIWM_WM_NAME;
Atom		EPIWM_WM_ICON_NAME;
Atom		EPIWM_WM_SAVE_YOURSELF;
Atom		EPIWM_MOTIF_WM_HINTS;
char		**gl_argv;

#ifdef __GNOME__
#include "gnome.h"
#endif
#include "style_init.h"
#include "window_draw.h"
#include "events.h"
#include "init.h"
#include "key.h"
#include "misc.h"
#include "menu.h"
#include "iconify.h"
#include "mouse_events.h"
#include "screen.h"
#include "workspace.h"
#include "window.h"
#include "window_events.h"
#include "window_state.h"
#include "mouse_functions.h"
#include "icon_styles.h"
#include "window_functions.h"
#include "button_draw.h"
#include "epiwm_functions.h"
#include "cursor.h"
#include "motif.h"
#include "xmalloc.h"
#endif /* __EPIWM_H__ */



