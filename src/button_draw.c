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
** button_draw.c
** 
** Made by (Julien Mulot)
** Login   <mulot_j@epita.fr>
** 
** Started on  Sun Apr 11 17:00:15 1999 Julien Mulot
 ** Last update Mon Oct 23 23:15:35 2000 Julien Mulot
*/


#include "epiwm.h"


#ifdef	  IMLIB
void		draw_button_image(Window button, char *path)
{
  int		width;
  int		height;
  Pixmap	pixmap;
  Pixmap	mask;
  
  if (!(gl_epiwin.titlebar.image =
	Imlib_load_image(gl_imlib_data, path)))
    {
      fprintf(stderr, "cannot load button image %s\n", path);
      exit(ERROR);
    }
  width = gl_epiwin.titlebar.image->rgb_width;
  height = gl_epiwin.titlebar.image->rgb_height;
  Imlib_render(gl_imlib_data, gl_epiwin.titlebar.image, 
	       width, height);  
  pixmap = Imlib_move_image(gl_imlib_data, 
			    gl_epiwin.titlebar.image);
  mask = Imlib_move_mask(gl_imlib_data, 
			 gl_epiwin.titlebar.image);
  XSetWindowBackgroundPixmap(gl_dpy, button, pixmap);
  XClearWindow(gl_dpy, button);
}
#endif


int			set_btn_position(t_win *ewin, char align, int pos)
{
  XWindowAttributes	attrib;
  
  if ((gl_epiwin.titlebar.position == TOP) 
      || (gl_epiwin.titlebar.position == BOTTOM))
    {
      if (align == ALIGN_LEFT)
	{
	  if (gl_epiwin.titlebar.width_mode)
	    {
	      if (gl_epiwin.titlebar.align_shape == ALIGN_LEFT)
		return (pos);
	      else
		{
		  XGetWindowAttributes(gl_dpy, ewin->titlebar_win, &attrib);
		  return (attrib.width - get_titlebar_width(ewin)
			  + ewin->titlebar_border + pos);
		}
	    }
	  else
	    return (pos);
	}
      else if (align == ALIGN_RIGHT)
	{
	  if (gl_epiwin.titlebar.width_mode)
	    {
	      if (gl_epiwin.titlebar.align_shape == ALIGN_LEFT)
		return ((get_titlebar_width(ewin)- pos -gl_epiwin.btn_width 
			 - ewin->titlebar_border >= 0) ?
			(get_titlebar_width(ewin) - pos -gl_epiwin.btn_width
			 - ewin->titlebar_border) : 0);
	      else
		{
		  XGetWindowAttributes(gl_dpy, ewin->titlebar_win, &attrib);
		  return ((attrib.width - pos - gl_epiwin.btn_width >= 0) ?
			  (attrib.width - pos - gl_epiwin.btn_width) : 0);
		  
		}
	    }
	  else
	    {
	      XGetWindowAttributes(gl_dpy, ewin->titlebar_win, &attrib);
	      return ((attrib.width - pos - gl_epiwin.btn_width >= 0) ?
		      (attrib.width - pos - gl_epiwin.btn_width) : 0);
	    }
	}
    }
  else
    {
      if (align == ALIGN_LEFT)
	{
	  if (gl_epiwin.titlebar.width_mode)
	    {
	      if (gl_epiwin.titlebar.align_shape == ALIGN_LEFT)
		return (pos);
	      else
		{
		  XGetWindowAttributes(gl_dpy, ewin->titlebar_win, &attrib);
		  return (attrib.height - get_titlebar_width(ewin) 
			  + ewin->titlebar_border * 2 + pos);
		}
	    }
	  else
	    return (pos);
	}
      else if (align == ALIGN_RIGHT)
	{
	  if (gl_epiwin.titlebar.width_mode)
	    {
	      if (gl_epiwin.titlebar.align_shape == ALIGN_LEFT)
		return ((get_titlebar_width(ewin) -pos -gl_epiwin.btn_height 
			 - ewin->titlebar_border >= 0) ?
			(get_titlebar_width(ewin) -pos - gl_epiwin.btn_height
			 - ewin->titlebar_border) : 0);
	      else
		{
		  XGetWindowAttributes(gl_dpy, ewin->titlebar_win, &attrib);
		  return ((attrib.height - pos - gl_epiwin.btn_height >= 0) ?
			  (attrib.height - pos - gl_epiwin.btn_height) : 0);
		}
	    }
	  else
	    {
	      XGetWindowAttributes(gl_dpy, ewin->titlebar_win, &attrib);
	      return ((attrib.height - pos - gl_epiwin.btn_height >= 0) ?
		      (attrib.height - pos - gl_epiwin.btn_height) : 0);
	    }
	}
    }
  return (0);
}


void	draw_button_max(Window button, char *state)
{
  if (!strcmp("active", state))
    {
#ifdef	  IMLIB
      if (gl_epiwin.titlebar.img_btn_max_active)
	draw_button_image(button, gl_epiwin.titlebar.img_btn_max_active);
      else
	{
#endif
	  XDrawRectangle(gl_dpy, button, gl_epiwin.titlebar.font_active,
			 0, 0, gl_epiwin.btn_width - 1, 
			 gl_epiwin.btn_height - 1);
	  XFillRectangle(gl_dpy, button, gl_epiwin.titlebar.font_active, 0, 0,
			 gl_epiwin.btn_width, gl_epiwin.btn_height / 3);
#ifdef	IMLIB 
	}
#endif
    }
  else
    {
#ifdef	  IMLIB
      if (gl_epiwin.titlebar.img_btn_max_inactive)
	draw_button_image(button, gl_epiwin.titlebar.img_btn_max_inactive);
      else
	{
#endif
	  XDrawRectangle(gl_dpy, button, gl_epiwin.titlebar.font_inactive,
			 0, 0, gl_epiwin.btn_width - 1, 
			 gl_epiwin.btn_height - 1);
	  XFillRectangle(gl_dpy, button, gl_epiwin.titlebar.font_inactive,
			 0, 0, gl_epiwin.btn_width, gl_epiwin.btn_height / 3);
#ifdef	IMLIB 
	}
#endif
    }
}


void	draw_button_max_height(Window button, char *state)
{
  if (!strcmp("active", state))
    {
#ifdef	  IMLIB
      if (gl_epiwin.titlebar.img_btn_max_height_active)
	draw_button_image(button,gl_epiwin.titlebar.img_btn_max_height_active);
      else
	{
#endif
	  XDrawRectangle(gl_dpy, button, gl_epiwin.titlebar.font_active,
			 gl_epiwin.btn_width / 6, 0,
			 2 * gl_epiwin.btn_width / 3 - 1, 
			 gl_epiwin.btn_height - 1);
	  XFillRectangle(gl_dpy, button, gl_epiwin.titlebar.font_active,
			 gl_epiwin.btn_width / 6, 0,
			 2 * gl_epiwin.btn_width / 3,
			 gl_epiwin.btn_height / 3);
#ifdef	IMLIB 
	}
#endif
    }
  else
    {
#ifdef	  IMLIB
      if (gl_epiwin.titlebar.img_btn_max_height_inactive)
	draw_button_image(button, 
			  gl_epiwin.titlebar.img_btn_max_height_inactive);
      else
	{
#endif
	  XDrawRectangle(gl_dpy, button, gl_epiwin.titlebar.font_inactive,
			 gl_epiwin.btn_width / 6, 0,
			 2 * gl_epiwin.btn_width / 3 - 1, 
			 gl_epiwin.btn_height - 1);
	  XFillRectangle(gl_dpy, button, gl_epiwin.titlebar.font_inactive,
			 gl_epiwin.btn_width / 6, 0,
			 2 * gl_epiwin.btn_width / 3,
			 gl_epiwin.btn_height / 3);
#ifdef	IMLIB 
	}
#endif
    }
}


void	draw_button_max_width(Window button, char *state)
{
  if (!strcmp("active", state))
    {
#ifdef	  IMLIB
      if (gl_epiwin.titlebar.img_btn_max_width_active)
	draw_button_image(button, gl_epiwin.titlebar.img_btn_max_width_active);
      else
	{
#endif
	  XDrawRectangle(gl_dpy, button, gl_epiwin.titlebar.font_active,
			 0, gl_epiwin.btn_height / 6,
			 gl_epiwin.btn_width - 1, 
			 2 * gl_epiwin.btn_height / 3 - 1);
	  XFillRectangle(gl_dpy, button, gl_epiwin.titlebar.font_active,
			 0, gl_epiwin.btn_height / 6,
			 gl_epiwin.btn_width, gl_epiwin.btn_height / 3);
#ifdef	IMLIB 
	}
#endif
    }
  else
    {
#ifdef	  IMLIB
      if (gl_epiwin.titlebar.img_btn_max_width_inactive)
	draw_button_image(button,
			  gl_epiwin.titlebar.img_btn_max_width_inactive);
      else
	{
#endif
	  XDrawRectangle(gl_dpy, button, gl_epiwin.titlebar.font_inactive,
			 0, gl_epiwin.btn_height / 6,
			 gl_epiwin.btn_width - 1, 
			 2 * gl_epiwin.btn_height / 3 - 1);
	  XFillRectangle(gl_dpy, button, gl_epiwin.titlebar.font_inactive,
			 0, gl_epiwin.btn_height / 6,
			 gl_epiwin.btn_width, gl_epiwin.btn_height / 3);
#ifdef	IMLIB 
	}
#endif
    }
}


void	draw_button_iconify(Window button, char *state)
{
  if (!strcmp("active", state))
    {
#ifdef	  IMLIB
      if (gl_epiwin.titlebar.img_btn_iconify_active)
	draw_button_image(button, gl_epiwin.titlebar.img_btn_iconify_active);
      else
	{
#endif
	  XDrawRectangle(gl_dpy, button, gl_epiwin.titlebar.font_active,
			 0, 0, gl_epiwin.btn_width - 1, 
			 gl_epiwin.btn_height - 1);
	  XDrawRectangle(gl_dpy, button, gl_epiwin.titlebar.font_active,
			 0, gl_epiwin.btn_height / 2,
			 gl_epiwin.btn_width / 2 - 1, 
			 gl_epiwin.btn_height / 2 - 1);
#ifdef	IMLIB
	}
#endif
    }
  else
    {
#ifdef	  IMLIB
      if (gl_epiwin.titlebar.img_btn_iconify_inactive)
	draw_button_image(button, gl_epiwin.titlebar.img_btn_iconify_inactive);
      else
	{
#endif
	  XDrawRectangle(gl_dpy, button, gl_epiwin.titlebar.font_inactive,
			 0, 0, gl_epiwin.btn_width - 1, 
			 gl_epiwin.btn_height - 1);
	  XDrawRectangle(gl_dpy, button, gl_epiwin.titlebar.font_inactive,
			 0, gl_epiwin.btn_height / 2,
			 gl_epiwin.btn_width / 2 - 1, 
			 gl_epiwin.btn_height / 2 - 1);
#ifdef	IMLIB
	}
#endif
    }
}


void	draw_button_close(Window button, char *state)
{
  if (!strcmp("active", state))
    {
#ifdef	  IMLIB
      if (gl_epiwin.titlebar.img_btn_close_active)
	draw_button_image(button, gl_epiwin.titlebar.img_btn_close_active);
      else
	{
#endif
	  XDrawRectangle(gl_dpy, button, gl_epiwin.titlebar.font_active,
			 0, 0, gl_epiwin.btn_width - 1, 
			 gl_epiwin.btn_height - 1);
#ifdef	IMLIB
	}
#endif
    }
  else
    {
#ifdef	  IMLIB
      if (gl_epiwin.titlebar.img_btn_close_inactive)
	draw_button_image(button, gl_epiwin.titlebar.img_btn_close_inactive);
      else
	{
#endif
	  XDrawRectangle(gl_dpy, button, gl_epiwin.titlebar.font_inactive,
			 0, 0, gl_epiwin.btn_width - 1, 
			 gl_epiwin.btn_height - 1);
#ifdef	IMLIB
	}
#endif
    }
}


void	draw_button_destroy(Window button, char *state)
{
  if (!strcmp("active", state))
    {
#ifdef	  IMLIB
      if (gl_epiwin.titlebar.img_btn_destroy_active)
	draw_button_image(button, gl_epiwin.titlebar.img_btn_destroy_active);
      else
	{
#endif
	  XDrawRectangle(gl_dpy, button, gl_epiwin.titlebar.font_active,
			 0, 0, gl_epiwin.btn_width - 1, 
			 gl_epiwin.btn_height - 1);
#ifdef	IMLIB
	}
#endif
    }
  else
    {
#ifdef	  IMLIB
      if (gl_epiwin.titlebar.img_btn_destroy_inactive)
	draw_button_image(button, gl_epiwin.titlebar.img_btn_destroy_inactive);
      else
	{
#endif
	  XDrawRectangle(gl_dpy, button, gl_epiwin.titlebar.font_inactive,
			 0, 0, gl_epiwin.btn_width - 1, 
			 gl_epiwin.btn_height - 1);
#ifdef	IMLIB
	}
#endif
    }
}


void	draw_button_shade(Window button, char *state)
{
  if (!strcmp("active", state))
    {
#ifdef	  IMLIB
      if (gl_epiwin.titlebar.img_btn_shade_active)
	draw_button_image(button, gl_epiwin.titlebar.img_btn_shade_active);
      else
	{
#endif
	  XFillRectangle(gl_dpy, button, gl_epiwin.titlebar.font_active,
			 0, gl_epiwin.btn_height / 3,
			 gl_epiwin.btn_width, gl_epiwin.btn_height / 3 - 1);
#ifdef	IMLIB
	}
#endif
    }
  else
    {
#ifdef	  IMLIB
      if (gl_epiwin.titlebar.img_btn_shade_inactive)
	draw_button_image(button, gl_epiwin.titlebar.img_btn_shade_inactive);
      else
	{
#endif
	  XFillRectangle(gl_dpy, button, gl_epiwin.titlebar.font_inactive,
			 0, gl_epiwin.btn_height / 3,
			 gl_epiwin.btn_width, gl_epiwin.btn_height / 3 - 1);
#ifdef	IMLIB
	}
#endif
    }
}
