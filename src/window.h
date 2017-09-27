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
** window.h
** 
** Made by (Poindessous Thomas)
** Login   <poinde_t@epita.fr>
** 
** Started on  Sat Jan 16 16:14:51 1999 Poindessous Thomas
 ** Last update Mon Oct 23 22:13:51 2000 Julien Mulot
*/

#ifndef   __WINDOW_H__
#define   __WINDOW_H__

#include <X11/cursorfont.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define NONE 0

typedef	struct	s_titlebar
{
#define ALIGN_LEFT	1
#define ALIGN_CENTER	2
#define ALIGN_RIGHT	3
#define TOP		1
#define BOTTOM		2
#define LEFT		3
#define RIGHT		4
#define IN_PIXEL	1
#define IN_PERCENT	2
  
  char		align;
  char		align_shape;
  char		grad_active_type;
  char		grad_inactive_type;
  int		height;
  int		width;
  int		space;
  char		position;
  char		width_mode;
#ifdef	  IMLIB
  char		*image_active;
  char		*image_inactive;
  char		*img_btn_close_active;
  char		*img_btn_close_inactive;
  char		*img_btn_destroy_active;
  char		*img_btn_destroy_inactive;
  char		*img_btn_max_active;
  char		*img_btn_max_inactive;
  char		*img_btn_shade_active;
  char		*img_btn_shade_inactive;
  char		*img_btn_iconify_active;
  char		*img_btn_iconify_inactive;
  char		*img_btn_max_height_active;
  char		*img_btn_max_height_inactive;
  char		*img_btn_max_width_active;
  char		*img_btn_max_width_inactive;
  char		image_max;
  ImlibImage	*image;
#endif
  Pixel		*pixels_active;
  Pixel		*pixels_inactive;
  GC		font_active;
  GC		font_inactive;
  XColor	color_active;
  XColor	color_inactive;
  XColor	color_stick;
  XFontStruct	*font;
}		t_titlebar;

typedef struct	s_epiwin
{
#define IS_WIN_CHG	0x01
#define OPAQ_MOVE	0x02
#define SMART_PLACE	0x04
#define AUTO_RAISE	0x08
#define FIRST_START	0x10
#define NEW_AUTO_FOCUS	0x20
  
  Cursor	normal_cursor;
  Cursor	resize_S_cursor;
  Cursor	resize_N_cursor;
  Cursor	resize_E_cursor;
  Cursor	resize_W_cursor;
  Cursor	resize_NW_cursor;
  Cursor	resize_NE_cursor;
  Cursor	resize_SW_cursor;
  Cursor	resize_SE_cursor;
  Cursor	move_cursor;
  Cursor	target_cursor;
  Cursor	current_cursor;
  GC		transmask_gc;
  int		border_size;
  int		btn_width;
  int		btn_height;
  char		focus_mode;
  /*
     char		is_window_changing;
     char		opaque_move;
     char		smart_placement;
     char		auto_raise;
     char	        first_start;
     */
  char		prop;
  char		align_btn_max;
  char		align_btn_close;
  char		align_btn_destroy;
  char		align_btn_shade;
  char		align_btn_iconify;
  char		align_btn_max_width;
  char		align_btn_max_height;
  int		pos_btn_max;
  int		pos_btn_close;
  int		pos_btn_destroy;
  int		pos_btn_shade;
  int		pos_btn_iconify;
  int		pos_btn_max_width;
  int		pos_btn_max_height;
  XColor	color_active;
  XColor	color_inactive;
  XColor	color_stick;
  XContext	context;
  t_win		*current;
  t_win		*left_win;
  t_titlebar	titlebar;
  t_workspace	*first_workspace;
  t_workspace	*current_workspace;
  t_win_list	*first_win_group_list;
  int		nb_elt;
  int		virtual_width;
  int		virtual_x;
  int		cursor_position_x;
  int		cursor_position_y;
  int		current_win_width;
  int		current_win_height;
  int		current_win_border_width;
  int		opaque_x;
  int		opaque_y;
  int		opaque_height;
  int		opaque_width;
  char		opaque_mvt;
}		t_epiwin;

typedef struct		s_window_btn
{
  int			x;
  int			y;
  int			height;
  int			width;
  char			border;
  unsigned long		bgcolor;
  unsigned long		fgcolor;
  Cursor		cursor;
  Window		parent;
}			t_window_btn;

Window		add_parent(t_win *, XWindowAttributes *);
Window		build_button(t_win *, char, int);
void		button_init(char *, int, char *, int *);
int		choose_place(t_win *, int *, int *);
int		test_shape(Window);
void		add_shape(t_win *);
void		window_build(FILE *);
Window		add_titlebar_win(t_win *, Window);
Window		add_border_frame(t_win *, Window);
Window		add_btn(t_window_btn *);
void		check_properties(t_win *);
void		init_titlebar_width(char *);
int		get_titlebar_width(t_win *);
void		placement_smart(t_win *);
void		max_fit(t_win *);
void		max_fit_height(t_win *);
char		set_align(char *);
void		set_focus_mode(char *);
void		set_titlebar_position(char *);
void		set_titlebar_btn_size();
int		set_title_position();
void		check_icon(t_win *);
void		create_icon_win_with_pixmap(t_win *, Pixmap *);
void		iconify_win_set_attrib(t_win *);
void		check_icon_size(Pixmap *, int *, int *);
void		create_pixmap_mask(t_win *, Pixmap *);
void		check_size_hints(t_win *);
void		verify_max_min(t_win *);

t_epiwin	gl_epiwin;


#endif /* __WINDOW_H__ */
