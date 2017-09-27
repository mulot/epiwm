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
** workspace.c
** 
** Made by (Poindessous Thomas)
** Login   <poinde_t@epita.fr>
** 
** Started on  Fri Apr  2 15:29:54 1999 Poindessous Thomas
 ** Last update Wed Apr  5 02:08:49 2000 Mulot Julien
*/

#include "epiwm.h"

void		send_window_to_workspace(t_win	*ewin,
					 int	nb)
{
  Window	dummy_win;
  int		current_nb;
  int		x_return;
  int		y_return;
  int		dummy_int;
  
#warning just a work-around, it must be redesign
  if (ewin == 0)
    return ;
  XGetGeometry(gl_dpy, ewin->parent, &dummy_win, &x_return, &y_return,
	       &dummy_int, &dummy_int, &dummy_int, &dummy_int);
  current_nb = gl_epiwin.virtual_x / gl_scr_info.dpy_width + 1;
  if (nb != current_nb)
    XMoveWindow(gl_dpy, ewin->parent,
		x_return + (nb - current_nb) *
		gl_scr_info.dpy_width, y_return);   
}

int		window_workspace(t_win	*ewin)
{
  Window	dummy_win;
  int		x;
  int		dummy;
  
  XGetGeometry(gl_dpy, ewin->parent, &dummy_win, &x, &dummy, &dummy, &dummy,
	       &dummy, &dummy);
  return ((gl_epiwin.virtual_x + x) / gl_scr_info.dpy_width + 1);
}

void		go_to_nth_workspace(int	nb)
{
  int		current;
  
  current = gl_epiwin.virtual_x / gl_scr_info.dpy_width + 1;
  move_all_windows(gl_scr_info.dpy_width * (current - nb), 0);
  gl_epiwin.virtual_x = (nb - 1) * gl_scr_info.dpy_width;
}

void		workspace_position(t_win	*ewin,
				   int		*left,
				   int		*right)
{
  int		x_return;
  int		y_return;
  int		width;
  int		height;
  int		border;
  int		dummy_int;
  Window	dummy_win;
  int		workspace_nb;
  
  XGetGeometry(gl_dpy, ewin->parent, &dummy_win, &x_return, &y_return,
	       &width, &height, &border, &dummy_int);
  if ((x_return < 0) && !gl_workspace_cycle)
    workspace_nb = (x_return + width + border) / gl_scr_info.dpy_width + 1;
  workspace_nb = x_return / gl_scr_info.dpy_width + 1;
  *left = (workspace_nb - 1) * gl_scr_info.dpy_width;
  *right = workspace_nb * gl_scr_info.dpy_width - 1;
}



void			wait_before_change_workspace(char	direction)
{
  Window		child;
  Window		root;
  int			root_x;
  int			root_y;
  int			win_x;
  int			win_y;
  unsigned int		status;
  int			i;
  struct timeval	time;
  int			is_still_here;
  
  is_still_here = 0;
  set_cursor_pager(direction);
  if (gl_workspace_time_change > 0)
    {
      for (i = 0; i < (gl_workspace_time_change / 5); i++)
	{
	  time.tv_sec = 0;
	  time.tv_usec = 10000;
	  (void) select(1, 0, 0, 0, &time);
	  XQueryPointer(gl_dpy, gl_event.xbutton.window, &root, &child,
			&root_x, &root_y, &win_x, &win_y, &status);
	  is_still_here = compare_mouse_position(direction, root_x, root_y);
	  if (is_still_here == 0)
	    break;
	}
      if (is_still_here == 1)
	go_to_pager(direction, root_x, root_y, gl_workspace_change_percent);
    }
  else
    {
      XQueryPointer(gl_dpy, gl_event.xbutton.window, &root, &child,
		    &root_x, &root_y, &win_x, &win_y, &status);
      go_to_pager(direction, root_x, root_y, gl_workspace_change_percent);
    }
}

void	set_cursor_pager(char	direction)
{
  if (direction == 'l')
    {
      if (((gl_epiwin.virtual_x > 0) || gl_workspace_cycle)
	  && (!gl_current_func))
	{
	  gl_epiwin.current_cursor = gl_epiwin.resize_W_cursor;
	  XDefineCursor(gl_dpy, gl_scr_info.root_win, 
			gl_epiwin.resize_W_cursor);
	}
    }
  else if (direction == 'r')
    {
      if ((((gl_epiwin.virtual_x + gl_scr_info.dpy_width)
	    < gl_epiwin.virtual_width) || gl_workspace_cycle)
	  && (!gl_current_func))
	{
	  gl_epiwin.current_cursor = gl_epiwin.resize_E_cursor;
	  XDefineCursor(gl_dpy, gl_scr_info.root_win, 
			gl_epiwin.resize_E_cursor);
	}
    }
  else if ((gl_epiwin.current_cursor != gl_epiwin.normal_cursor)
	   && (!gl_current_func))
    {
      gl_epiwin.current_cursor = gl_epiwin.normal_cursor;
      XDefineCursor(gl_dpy, gl_scr_info.root_win, 
		    gl_epiwin.normal_cursor);
    }
  
}

int	compare_mouse_position(char	direction,
			       int	root_x,
			       int	root_y)
{
  int	is_still_here;
  
  is_still_here = 0;
  if (direction == 'r')
    if (root_x == gl_scr_info.dpy_width - 1)
      is_still_here = 1;
  if (direction == 'l')
    if (root_x == 0)
      is_still_here = 1;
  return (is_still_here);
}

void	go_to_pager(char	direction,
		    int		current_x,
		    int		current_y,
		    int		step)
{
  if (step == 1)
    step = 100 / gl_scr_info.dpy_width;
  if ((direction == 'l') && ((gl_epiwin.virtual_x > 0) || gl_workspace_cycle))
    {
      if (current_x <= 1)
	XWarpPointer(gl_dpy, None, gl_scr_info.root_win, 0, 0, 0, 0,
		     current_x + gl_scr_info.dpy_width * step / 100 - 2,
		     current_y);
      if ((gl_epiwin.virtual_x == 0) && gl_workspace_cycle)
	gl_epiwin.virtual_x = (gl_epiwin.virtual_width -
			       gl_scr_info.dpy_width * step / 100);
      else
	gl_epiwin.virtual_x = (gl_epiwin.virtual_x -
			       gl_scr_info.dpy_width * step / 100);
      move_all_windows(gl_scr_info.dpy_width * step / 100, 0);
    }
  else if ((direction == 'r') &&
	   (((gl_epiwin.virtual_x +
	      gl_scr_info.dpy_width) < gl_epiwin.virtual_width) ||
	    gl_workspace_cycle))
    {
      if (current_x >= gl_scr_info.dpy_width - 1)
	XWarpPointer(gl_dpy, None, gl_scr_info.root_win, 0, 0, 0, 0,
		     -(current_x - gl_scr_info.dpy_width * step / 100 - 2),
		     current_y);
      if (gl_epiwin.virtual_x >= gl_epiwin.virtual_width)
	gl_epiwin.virtual_x = 0 -
	  (gl_epiwin.virtual_width - gl_epiwin.virtual_x);
      else
	gl_epiwin.virtual_x = (gl_epiwin.virtual_x + gl_scr_info.dpy_width *
			       step / 100);
      move_all_windows(-(gl_scr_info.dpy_width * step / 100), 0);
    }
}

void			move_all_windows(int	width,
					 int	height)
{
  t_win_list		*w_list;
  XWindowAttributes	attrib;
  
  if (gl_epiwin.opaque_mvt)
    XDrawRectangle(gl_dpy, gl_scr_info.root_win, gc_xor,
		   gl_epiwin.opaque_x, gl_epiwin.opaque_y,
		   gl_epiwin.opaque_width,
		   gl_epiwin.opaque_height);
  w_list = gl_epiwin.first_win_group_list;
  while (w_list)
    {
      if ((w_list->p_t_win) && (width != 0 || height != 0))
	{
	  if (!(w_list->p_t_win->prop & STICKY))
	    {
	      XGetWindowAttributes(gl_dpy, w_list->p_t_win->parent, &attrib);
	      if (gl_workspace_cycle &&
		  ((attrib.x + attrib.width + width) >=
		   gl_epiwin.virtual_width) &&
		  (width > 0))
		XMoveWindow(gl_dpy, w_list->p_t_win->parent,
			    attrib.x + width - gl_epiwin.virtual_width,
			    attrib.y);
	      else if (gl_workspace_cycle &&
		       ((attrib.x + attrib.width + width) < 0) &&
		       (width < 0))
		XMoveWindow(gl_dpy, w_list->p_t_win->parent,
			    gl_epiwin.virtual_width +
			    (attrib.x + width),
			    attrib.y);
	      else
		XMoveWindow(gl_dpy, w_list->p_t_win->parent,
			    attrib.x + width, attrib.y);
	    }
	}
      w_list = w_list->next_elt;
    }
  if (gl_epiwin.opaque_mvt)
    XDrawRectangle(gl_dpy, gl_scr_info.root_win, gc_xor,
		   gl_epiwin.opaque_x, gl_epiwin.opaque_y,
		   gl_epiwin.opaque_width,
		   gl_epiwin.opaque_height);
}


