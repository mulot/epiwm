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
 ** mouse_functions.c
 ** 
 ** Made by (Poindessous Thomas)
 ** Login   <poinde_t@epita.fr>
 ** 
 ** Started on  Sun Apr  4 15:48:03 1999 Poindessous Thomas
 ** Last update Thu Apr  6 18:35:30 2000 Mulot Julien
 */

#include "epiwm.h"

void			mouse_motion_notify(ewin)
t_win			*ewin;
{
  int			x_root;
  int			y_root;
  t_mouse_move_win	list;
  XWindowAttributes	attrib;
  t_coord		*coord;
  
  list.button = &gl_event.xbutton;
  list.ewin = ewin;
  if (ewin)
    {
      if (gl_epiwin.prop & IS_WIN_CHG)
	{
	  if ((ewin->titlebar_win == gl_win_move.win)
	      && (Button1Mask == (list.button->state & Button1Mask)))
	    {
	      x_root = gl_event.xmotion.x_root - gl_win_move.mouse_x;
	      y_root = gl_event.xmotion.y_root - gl_win_move.mouse_y;
	      clientSetState(ewin, WithdrawnState);
	      if (gl_epiwin.prop & OPAQ_MOVE)
		{
		  XGetWindowAttributes(gl_dpy, ewin->parent, &attrib);
		  win_opaque_resize(ewin, x_root, y_root,
				    gl_epiwin.opaque_width,
				    gl_epiwin.opaque_height);
		}
	      else
		{
		  coord = window_on_border(ewin, x_root, y_root);
		  XMoveWindow(gl_dpy, ewin->parent, coord->x, coord->y);
		  if (coord->x == x_root)
		    gl_epiwin.cursor_position_x = -1;
		  if (coord->y == y_root)
		    gl_epiwin.cursor_position_y = -1;
		  free (coord);
		}
	      mouse_on_border_notify();
	    }
	  else if (ewin->border_frame == gl_win_move.win)
	    {
	      x_root = gl_event.xmotion.x_root - gl_win_move.win_x;
	      y_root = gl_event.xmotion.y_root - gl_win_move.win_y;
	      coord = window_on_border(ewin, x_root, y_root);
	      win_resize_all(ewin);
	      if (coord->x == x_root)
		gl_epiwin.cursor_position_x = -1;
	      if (coord->y == y_root)
		gl_epiwin.cursor_position_y = -1;
	      free (coord);
	      mouse_on_border_notify();
	    }
	  else if (ewin->iconify_title_win == gl_win_move.win)
	    {
	      x_root = gl_event.xmotion.x_root - gl_win_move.win_x;
	      y_root = gl_event.xmotion.y_root - gl_win_move.win_y;
	      XGetWindowAttributes(gl_dpy, ewin->iconify_win, &attrib);
	      XMoveWindow(gl_dpy, ewin->iconify_title_win, x_root, y_root);
	      XMoveWindow(gl_dpy, ewin->iconify_win, x_root,
			  y_root - attrib.height);
	    }
	  else if (ewin->iconify_win == gl_win_move.win)
	    {
	      x_root = gl_event.xmotion.x_root - gl_win_move.win_x;
	      y_root = gl_event.xmotion.y_root - gl_win_move.win_y;
	      XGetWindowAttributes(gl_dpy, ewin->iconify_win, &attrib);
	      XMoveWindow(gl_dpy, ewin->iconify_win, x_root, y_root);
	      XMoveWindow(gl_dpy, ewin->iconify_title_win, x_root,
			  y_root + attrib.height);
	    }
	}
      else
	{
	  if ((gl_epiwin.current_cursor != gl_epiwin.normal_cursor)
	      && (!gl_current_func))
	    {
	      gl_epiwin.current_cursor = gl_epiwin.normal_cursor;
	      XDefineCursor(gl_dpy, ewin->win, gl_epiwin.normal_cursor);
	      XDefineCursor(gl_dpy, gl_scr_info.root_win,
			    gl_epiwin.normal_cursor);
	    }
	  if (ewin->border_frame == gl_event.xmotion.window)
	    set_border_cursor(ewin, ewin->border_frame);
	  if (ewin->titlebar_win == gl_event.xmotion.window)
	    set_titlebar_cursor(ewin, ewin->titlebar_win);
	  mouse_on_border_notify();
	}
    }
}

void	win_resize_all(ewin)
t_win	*ewin;		
{
  int	width_inc;
  int	height_inc;
  
  width_inc = ewin->hints.width_inc;
  height_inc = ewin->hints.height_inc;
  if (gl_win_move.win_x <= ewin->border_size)
    {
      if (gl_win_move.win_y <= gl_win_move.height - ewin->border_size)
	{
	  if (gl_win_move.win_y < ewin->border_size)
	    win_resize_up_left(ewin, width_inc, height_inc);
	  else
	    win_resize_left(ewin, width_inc, height_inc);
	}
      else
	win_resize_down_left(ewin, width_inc, height_inc);
    }
  else if (gl_win_move.win_x >= gl_win_move.width - ewin->border_size)
    {
      if (gl_win_move.win_y <= gl_win_move.height - ewin->border_size)
	{
	  if (gl_win_move.win_y < ewin->border_size)
	    win_resize_up_right(ewin, width_inc, height_inc);
	  else
	    win_resize_right(ewin, width_inc, height_inc);
	}
      else
	win_resize_down_right(ewin, width_inc, height_inc);
    }
  else if (gl_win_move.win_x > ewin->border_size &&
	   gl_win_move.win_x < gl_win_move.width - ewin->border_size)
    {
      if (gl_win_move.win_y >= gl_win_move.height - ewin->border_size)
	win_resize_down(ewin, width_inc, height_inc);
      else if (gl_win_move.win_y < ewin->border_size)
	win_resize_up(ewin, width_inc, height_inc);
    }
  else 
    return ;
  if (ewin->size & SHAPE)
    add_shape(ewin);
  if (gl_epiwin.titlebar.width)
    draw_titlebar_shape(ewin);
  send_configure_notify(ewin);
}

void			win_resize_left(ewin, width_inc, height_inc)
t_win			*ewin;
int			width_inc;
int			height_inc;
{
  XWindowAttributes	attrib;
  int			width;
  
  XGetWindowAttributes(gl_dpy, ewin->parent, &attrib);
  width = (((int) (attrib.x - gl_event.xmotion.x_root))
	   / width_inc) * width_inc;
  if (width == 0)
    return ;
  if (attrib.width + width >= 1)
    window_group_resize(ewin, attrib.x - width, attrib.y,
			attrib.width + width, attrib.height);
  else
    window_group_resize(ewin, attrib.x + attrib.width, attrib.y,
			1, attrib.height);
}

void	win_resize_right(ewin, width_inc, height_inc)
t_win	*ewin;
int	width_inc;
int	height_inc;
{
  XWindowAttributes	attrib;
  int			width;
  
  XGetWindowAttributes(gl_dpy, ewin->parent, &attrib);
  width = (((int) (gl_event.xmotion.x_root - attrib.x
		   - attrib.width))
	   / width_inc) * width_inc;
  if (width == 0)
    return ;
  if (attrib.width + width >= 1)
    window_group_resize(ewin, attrib.x, attrib.y,
			attrib.width + width, attrib.height);
  else
    window_group_resize(ewin, attrib.x, attrib.y, 1, attrib.height);
}

void	win_resize_up(ewin, width_inc, height_inc)
t_win	*ewin;
int	width_inc;
int	height_inc;
{
  XWindowAttributes	attrib;
  int			height;
  
  XGetWindowAttributes(gl_dpy, ewin->parent, &attrib);
  height = (((int) (attrib.y - gl_event.xmotion.y_root))
	    / height_inc) * height_inc;
  if (height == 0)
    return ;
  if (attrib.height + height >= 1)
    window_group_resize(ewin, attrib.x, attrib.y - height,
			attrib.width, attrib.height + height);
  else
    window_group_resize(ewin, attrib.x, attrib.y + attrib.height
			- ewin->hints.min_height
			+ 2*(ewin->border_size + ewin->titlebar_border)
			+ ewin->titlebar_height + ewin->bf_border,
			attrib.width, 1);   
}

void			win_resize_down(ewin, width_inc, height_inc)
t_win			*ewin;
int			width_inc;
int			height_inc;
{
  XWindowAttributes	attrib;
  int			height;
  
  XGetWindowAttributes(gl_dpy, ewin->parent, &attrib);
  height = (((int) (gl_event.xmotion.y_root - attrib.y - attrib.height))
	    / height_inc) * height_inc;
  if (height == 0)
    return ;
  if (attrib.height + height >= 1)
    window_group_resize(ewin, attrib.x, attrib.y,
			attrib.width, attrib.height + height);
  else
    window_group_resize(ewin, attrib.x, attrib.y, attrib.width, 1);
}

void	win_resize_up_left(ewin, width_inc, height_inc)
t_win	*ewin;
int	width_inc;
int	height_inc;
{
  XWindowAttributes	attrib;
  int			height;
  int			width;
  
  XGetWindowAttributes(gl_dpy, ewin->parent, &attrib);
  height = (((int) (attrib.y - gl_event.xmotion.y_root))
	    / height_inc) * height_inc;
  width = (((int) (attrib.x - gl_event.xmotion.x_root))
	   / width_inc) * width_inc;
  if (height == 0 && width == 0)
    return ;
  if (attrib.width + width >= 1 && attrib.height + height >= 1)
    window_group_resize(ewin, attrib.x - width,  attrib.y - height,
			attrib.width + width, attrib.height + height);
  else if (attrib.width + width < 1 && attrib.height + height >= 1)
    window_group_resize(ewin, attrib.x + attrib.width, attrib.y - height,
			1, attrib.height + height);
  else if (attrib.width + width >= 1 && attrib.height + height < 1)
    window_group_resize(ewin, attrib.x - width, attrib.y + attrib.height
			- ewin->hints.min_height
			+ 2*(ewin->border_size + ewin->titlebar_border)
			+ ewin->titlebar_height + ewin->bf_border,
			attrib.width + width, 1);
  else if (attrib.width + width < 1 && attrib.height + height < 1)
    window_group_resize(ewin, attrib.x + attrib.width,
			attrib.y + attrib.height
			- ewin->hints.min_height
			+ 2*(ewin->border_size + ewin->titlebar_border)
			+ ewin->titlebar_height + ewin->bf_border,
			1, 1);   
}

void	win_resize_up_right(ewin, width_inc, height_inc)
t_win	*ewin;
int	width_inc;
int	height_inc;
{
  XWindowAttributes	attrib;
  int			height;
  int			width;
  
  XGetWindowAttributes(gl_dpy, ewin->parent, &attrib);
  height = (((int) (attrib.y - gl_event.xmotion.y_root))
	    / height_inc) * height_inc;
  width = (((int) (attrib.x - gl_event.xmotion.x_root))
	   / width_inc) * width_inc;
  if (height == 0 && width == 0)
    return ;
  if ((width < 0))/* && (height > 0))*/
    window_group_resize(ewin, attrib.x, attrib.y - height,
			-width, attrib.height + height);
  /*
     else if ((width < 0) && (height < 0))
     window_group_resize(ewin, attrib.x, attrib.y - height,
     attrib.width + width, attrib.height + height);
     */
}

void			win_resize_down_left(ewin, width_inc, height_inc)
t_win			*ewin;
int			width_inc;
int			height_inc;
{
  XWindowAttributes	attrib;
  int			width;
  int			height;
   
  XGetWindowAttributes(gl_dpy, ewin->parent, &attrib);
  width = (((int) (attrib.x - gl_event.xmotion.x_root))
	   / width_inc) * width_inc;
  height = (((int) (gl_event.xmotion.y_root - attrib.y - attrib.height))
	    / height_inc) * height_inc;
  if (width == 0 && height == 0)
    return ;
  if (attrib.width + width >= 1 && attrib.height + height >= 1)
    window_group_resize(ewin, attrib.x - width, attrib.y,
			attrib.width + width, attrib.height + height);
  else if (attrib.width + width < 1 && attrib.height + height >= 1)
    window_group_resize(ewin, attrib.x + attrib.width, attrib.y,
			1, attrib.height + height);
  else if (attrib.width + width >= 1 && attrib.height + height < 1)
    window_group_resize(ewin, attrib.x - width, attrib.y,
			attrib.width + width, 1);
  else if (attrib.width + width < 1 && attrib.height + height < 1)
    window_group_resize(ewin, attrib.x + attrib.width, attrib.y,
			1, 1);   
}

void			win_resize_down_right(ewin, width_inc, height_inc)
t_win			*ewin;
int			width_inc;
int			height_inc;
{
  XWindowAttributes	attrib;
  int			height;
  int			width;
   
  XGetWindowAttributes(gl_dpy, ewin->parent, &attrib);
  height = (((int) (gl_event.xmotion.y_root - attrib.y - attrib.height))
	    / height_inc) * height_inc;
  width = (((int) (gl_event.xmotion.x_root - attrib.x
		   - attrib.width))
	   / width_inc) * width_inc;
  if (height == 0 && width == 0)
    return ;
  if (attrib.height + height >= 1 && attrib.width + width >= 1)
    window_group_resize(ewin, attrib.x, attrib.y,
			attrib.width + width, attrib.height + height);
  else if (attrib.height + height >= 1 && attrib.width + width < 1)
    window_group_resize(ewin, attrib.x, attrib.y,
			1, attrib.height + height);
  else if (attrib.height + height < 1 && attrib.width + width >= 1)
    window_group_resize(ewin, attrib.x, attrib.y,
			attrib.width + width, 1);
  else if (attrib.height + height < 1 && attrib.width + width < 1)
    window_group_resize(ewin, attrib.x, attrib.y,
			1, 1);   
}

void	win_opaque_resize(ewin, parent_x, parent_y,
			  parent_width, parent_height)
t_win	*ewin;
int	parent_x;
int	parent_y;
int	parent_width;
int	parent_height;
{
  if (gl_epiwin.opaque_mvt == 0)
    {
      XGrabServer(gl_dpy);
      XGrabPointer(gl_dpy, ewin->win, False,
		   ButtonMotionMask | ButtonReleaseMask,
		   GrabModeAsync, GrabModeAsync, None, None, CurrentTime);
      gl_epiwin.opaque_mvt = 1;
    }
  else
    XDrawRectangle(gl_dpy, gl_scr_info.root_win, gc_xor,
		   gl_epiwin.opaque_x, gl_epiwin.opaque_y,
		   gl_epiwin.opaque_width,
		   gl_epiwin.opaque_height);
  gl_epiwin.opaque_x = parent_x;
  gl_epiwin.opaque_y = parent_y;
  gl_epiwin.opaque_width = parent_width;
  gl_epiwin.opaque_height = parent_height;
  XDrawRectangle(gl_dpy, gl_scr_info.root_win, gc_xor,
		 gl_epiwin.opaque_x, gl_epiwin.opaque_y,
		 gl_epiwin.opaque_width,
		 gl_epiwin.opaque_height);
}





