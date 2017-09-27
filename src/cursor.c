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
** cursor.c
** 
** Made by (Stephane RAULT)
** Login   <rault_s@epita.fr>
** 
** Started on  Mon May 10 16:43:30 1999 Stephane RAULT
 ** Last update Mon Oct 23 22:49:37 2000 Julien Mulot
*/


#include "epiwm.h"


void			set_border_cursor(t_win *ewin, Window border)
{
  XWindowAttributes	attrib;
  
  if (!gl_current_func)
    {
      XGetWindowAttributes(gl_dpy, border, &attrib);
      if (gl_epiwin.current_cursor != gl_epiwin.normal_cursor)
	{
	  gl_epiwin.current_cursor = gl_epiwin.normal_cursor;
	  XDefineCursor(gl_dpy, border, gl_epiwin.normal_cursor);
	}
      if (gl_event.xmotion.x < ewin->border_size)
	{
	  if (gl_event.xmotion.y > (attrib.height - 5))
	    {
	      gl_epiwin.current_cursor = gl_epiwin.resize_SW_cursor;
	      XDefineCursor(gl_dpy, border, gl_epiwin.resize_SW_cursor);
	    }
	  else
	    {
	      gl_epiwin.current_cursor = gl_epiwin.resize_W_cursor;
	      XDefineCursor(gl_dpy, border, gl_epiwin.resize_W_cursor);
	    }
	}
      if (gl_event.xmotion.x > (attrib.width - ewin->border_size))
	{
	  if (gl_event.xmotion.y > (attrib.height - 5))
	    {
	      gl_epiwin.current_cursor = gl_epiwin.resize_SE_cursor;
	      XDefineCursor(gl_dpy, border, gl_epiwin.resize_SE_cursor);
	    }
	  else
	    {
	      gl_epiwin.current_cursor = gl_epiwin.resize_E_cursor;
	      XDefineCursor(gl_dpy, border, gl_epiwin.resize_E_cursor);
	    }
	}
      if (gl_event.xmotion.y > (attrib.height - ewin->border_size))
	{
	  if (gl_event.xmotion.x < 5)
	    {
	      gl_epiwin.current_cursor = gl_epiwin.resize_SW_cursor;
	      XDefineCursor(gl_dpy, border, gl_epiwin.resize_SW_cursor);
	    }
	  else if (gl_event.xmotion.x > (attrib.width - 5))
	    {
	      gl_epiwin.current_cursor = gl_epiwin.resize_SE_cursor;
	      XDefineCursor(gl_dpy, border, gl_epiwin.resize_SE_cursor);
	    }
	  else
	    {
	      gl_epiwin.current_cursor = gl_epiwin.resize_S_cursor;
	      XDefineCursor(gl_dpy, border, gl_epiwin.resize_S_cursor);
	    }
	}
    }
}


void			set_titlebar_cursor(t_win *ewin, Window titlebar)
{
  XWindowAttributes   attrib;
  
  if (!gl_current_func)
    {
      XDefineCursor(gl_dpy, titlebar, gl_epiwin.normal_cursor);
      XGetWindowAttributes(gl_dpy, titlebar, &attrib);
      if (gl_event.xmotion.y < ewin->border_size)
	{
	  if (gl_event.xmotion.x < 5)
	    {
	      gl_epiwin.current_cursor = gl_epiwin.resize_NW_cursor;
	      XDefineCursor(gl_dpy, titlebar, gl_epiwin.resize_NW_cursor);
	    }
	  else if (gl_event.xmotion.x > (attrib.width - 5))
	    {
	      gl_epiwin.current_cursor = gl_epiwin.resize_NE_cursor;
	      XDefineCursor(gl_dpy, titlebar, gl_epiwin.resize_NE_cursor);
	    }
	  else
	    {
	      gl_epiwin.current_cursor = gl_epiwin.resize_N_cursor;
	      XDefineCursor(gl_dpy, titlebar, gl_epiwin.resize_N_cursor);
	    }
	}
    }
}
