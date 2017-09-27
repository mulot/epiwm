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
** window_draw.c
** 
** Made by (mulot)
** Login   <mulot_j@epita.fr>
** 
** Started on  Thu Apr  8 21:35:18 1999 mulot
 ** Last update Mon Oct 30 21:49:56 2000 Julien Mulot
*/

#include "epiwm.h"

void			draw_titlebar_shape(ewin)
     t_win			*ewin;
{
  XRectangle		rect[2];
  XWindowAttributes	attrib;
  char			nb_rect;
  
  if (ewin->size & SHAPE)
    nb_rect = 1;
  else
    nb_rect = 2;
  XGetWindowAttributes(gl_dpy, ewin->parent, &attrib);
  if (gl_epiwin.titlebar.position == TOP
      || ((gl_epiwin.titlebar.position == BOTTOM) &&
	  ((ewin->is_shade == 2) || (ewin->is_shade == 3))))
    {
      if (gl_epiwin.titlebar.align_shape == ALIGN_LEFT)
	{
	  rect[0].x = 0;
	  rect[0].y = 0;
	  rect[0].width = get_titlebar_width(ewin) + ewin->titlebar_border;
	  rect[0].height = ewin->titlebar_height + ewin->titlebar_border * 2;
	}
      else
	{
	  rect[0].x = attrib.width - get_titlebar_width(ewin)
	    - ewin->titlebar_border;
	  rect[0].y = 0;
	  rect[0].width = get_titlebar_width(ewin) + ewin->titlebar_border;
	  rect[0].height = ewin->titlebar_height + ewin->titlebar_border * 2;
	}
      XGetWindowAttributes(gl_dpy, ewin->win, &attrib);
      rect[1].x = 0;
      rect[1].y = ewin->titlebar_height + ewin->titlebar_border;
      rect[1].width = attrib.width + ewin->bf_border * 2
	+ attrib.border_width * 2 + ewin->border_size*2;
      rect[1].height = attrib.height + ewin->bf_border * 2
	+ attrib.border_width * 2 + ewin->border_size*2;
      XShapeCombineShape(gl_dpy, ewin->parent, ShapeBounding,
			 ewin->border_size + attrib.border_width
			 + ewin->bf_border,
			 ewin->border_size + attrib.border_width
			 + ewin->titlebar_height
			 + 2*ewin->titlebar_border,
			 ewin->win, ShapeBounding, ShapeSet);
    }
  else if ((gl_epiwin.titlebar.position == LEFT)
	   || ((gl_epiwin.titlebar.position == RIGHT) &&
	       ((ewin->is_shade == 2) || (ewin->is_shade == 3))))
    {
      if (gl_epiwin.titlebar.align_shape == ALIGN_LEFT)
	{
	  rect[0].x = 0;
	  rect[0].y = 0;
	  rect[0].width = ewin->titlebar_height + ewin->titlebar_border * 2;
	  rect[0].height = get_titlebar_width(ewin) + ewin->titlebar_border;
	}
      else
	{
	  rect[0].x = 0;
	  rect[0].y = attrib.height - get_titlebar_width(ewin);
	  rect[0].width = ewin->titlebar_height + ewin->titlebar_border * 2;
	  rect[0].height = get_titlebar_width(ewin) + ewin->titlebar_border;
	}
      XGetWindowAttributes(gl_dpy, ewin->win, &attrib);
      rect[1].x = ewin->titlebar_height + ewin->titlebar_border;
      rect[1].y = 0;
      rect[1].width = attrib.width + ewin->bf_border * 2
	+ attrib.border_width * 2 + ewin->border_size*2;
      rect[1].height = attrib.height + ewin->bf_border * 2
	+ attrib.border_width * 2 + ewin->border_size*2;
      XShapeCombineShape(gl_dpy, ewin->parent, ShapeBounding,
			 ewin->border_size + attrib.border_width 
			 + ewin->titlebar_height + ewin->titlebar_border * 2, 
			 ewin->bf_border +
			 ewin->border_size + attrib.border_width,
			 ewin->win, ShapeBounding, ShapeSet);
    }
  else if ((gl_epiwin.titlebar.position == RIGHT) &&
	   ((ewin->is_shade == 0) || (ewin->is_shade == 1)))
    {
      if (gl_epiwin.titlebar.align_shape == ALIGN_LEFT)
	{
	  rect[0].x = attrib.width - ewin->titlebar_height
	    - ewin->titlebar_border*2;
	  rect[0].y = 0;
	  rect[0].width = ewin->titlebar_height + ewin->titlebar_border * 2;
	  rect[0].height = get_titlebar_width(ewin) + ewin->titlebar_border;
	}
      else
	{
	  rect[0].x = attrib.width - ewin->titlebar_height
	    - ewin->titlebar_border*2;
	  rect[0].y = attrib.height - get_titlebar_width(ewin);
	  rect[0].width = ewin->titlebar_height + ewin->titlebar_border * 2;
	  rect[0].height = get_titlebar_width(ewin) + ewin->titlebar_border;
	}
      XGetWindowAttributes(gl_dpy, ewin->win, &attrib);
      rect[1].x = 0;
      rect[1].y = 0;
      rect[1].width = attrib.width + ewin->bf_border * 2
	+ attrib.border_width * 2 + ewin->border_size*2;
      rect[1].height = attrib.height + ewin->bf_border * 2
	+ attrib.border_width * 2 + ewin->border_size*2;
      XShapeCombineShape(gl_dpy, ewin->parent, ShapeBounding,
			 ewin->border_size + attrib.border_width
			 + ewin->bf_border, ewin->bf_border + ewin->border_size 
			 + attrib.border_width,
			 ewin->win, ShapeBounding, ShapeSet);
    }
  else if ((gl_epiwin.titlebar.position == BOTTOM) &&
	   ((ewin->is_shade == 0) || (ewin->is_shade == 1)))
    {
      if (gl_epiwin.titlebar.align_shape == ALIGN_LEFT)
	{
	  rect[0].x = 0;
	  rect[0].y = attrib.height - ewin->titlebar_height
	    - ewin->titlebar_border*2;
	  rect[0].width = get_titlebar_width(ewin) + ewin->titlebar_border;
	  rect[0].height = ewin->titlebar_height + ewin->titlebar_border * 2;
	}
      else
	{
	  rect[0].x = attrib.width - get_titlebar_width(ewin)
	    - ewin->titlebar_border;;
	  rect[0].y = attrib.height - ewin->titlebar_height
	    - ewin->titlebar_border*2;
	  rect[0].width = get_titlebar_width(ewin) + ewin->titlebar_border;
	  rect[0].height = ewin->titlebar_height + ewin->titlebar_border * 2;
	}
      XGetWindowAttributes(gl_dpy, ewin->win, &attrib);
      rect[1].x = 0;
      rect[1].y = 0;
      rect[1].width = attrib.width + ewin->bf_border * 2
	+ attrib.border_width * 2 + ewin->border_size*2;
      rect[1].height = attrib.height + ewin->bf_border * 2
	+ attrib.border_width * 2 + ewin->border_size*2;
      XShapeCombineShape(gl_dpy, ewin->parent, ShapeBounding,
			 ewin->border_size + attrib.border_width
			 + ewin->bf_border, ewin->bf_border + ewin->border_size 
			 + attrib.border_width,
			 ewin->win, ShapeBounding, ShapeSet);
    }
  if (ewin->titlebar_height != 0)
    {      
      if ((ewin->is_shade == 0) || (ewin->is_shade == 1))
	XShapeCombineRectangles(gl_dpy, ewin->parent, ShapeBounding,
				0, 0, rect, nb_rect,
				ShapeUnion, Unsorted);
      else
	XShapeCombineRectangles(gl_dpy, ewin->parent, ShapeBounding,
				0, 0, rect, 1,
				ShapeUnion, Unsorted);
    }
}

void	set_background(button, state)
     Window	button;
     char	*state;
{
  if (!strcmp("active", state))
    XSetWindowBackground(gl_dpy, button,
			 gl_epiwin.titlebar.color_active.pixel);
  else
    XSetWindowBackground(gl_dpy, button,
			 gl_epiwin.titlebar.color_inactive.pixel);
  XClearWindow(gl_dpy, button);
}


void	titlebar_button_state(ewin, state)
     t_win	*ewin;
     char	*state;
{
  if (gl_epiwin.align_btn_iconify)
    {
      set_background(ewin->button_iconify, state);
      draw_button_iconify(ewin->button_iconify, state);
    }
  if (gl_epiwin.align_btn_max)
    {
      set_background(ewin->button_max, state);
      draw_button_max(ewin->button_max, state);
    }
  if (gl_epiwin.align_btn_max_height)
    {
      set_background(ewin->button_max_height, state);
      draw_button_max_height(ewin->button_max_height, state);
    }
  if (gl_epiwin.align_btn_max_width)
    {
      set_background(ewin->button_max_width, state);
      draw_button_max_width(ewin->button_max_width, state);
    }
  if (gl_epiwin.align_btn_close)
    {
      set_background(ewin->button_close, state);
      draw_button_close(ewin->button_close, state);
    }
  if (gl_epiwin.align_btn_destroy)
    {
      set_background(ewin->button_destroy, state);
      draw_button_destroy(ewin->button_destroy, state);
    }
  if (gl_epiwin.align_btn_shade)
    {
      set_background(ewin->button_shade, state);
      draw_button_shade(ewin->button_shade, state);
    }
}

void			window_state(ewin, state)
     t_win			*ewin;
     char			*state;
{
  XWindowAttributes	attrib;
  int			expose;
  
  expose = 1;
  if (!strcmp(state, "active"))
    {
      if ((gl_epiwin.current) && (gl_epiwin.current != ewin))
	window_state(gl_epiwin.current, "inactive");
      if ((ewin->prop & NO_TITLE) == 0)
	{
#ifdef	  IMLIB
	  if (gl_epiwin.titlebar.image_active)
	    draw_titlebar_image(ewin, state);
	  else
	    {
#endif
	      if (gl_epiwin.titlebar.grad_active_type)
		draw_gradient(ewin, ewin->titlebar_win, "titlebar", state);
	      else
		{
		  XSetWindowBackground(gl_dpy, ewin->titlebar_win, 
				       gl_epiwin.titlebar.color_active.pixel);
		  XClearWindow(gl_dpy, ewin->titlebar_win);
		}
#ifdef	  IMLIB
	    }
#endif
	}
      XSetWindowBackground(gl_dpy, ewin->border_frame, 
			   gl_epiwin.color_active.pixel);
      XClearWindow(gl_dpy, ewin->border_frame);
      XSetInputFocus(gl_dpy, ewin->win, RevertToParent, CurrentTime);
      gl_epiwin.current = ewin;
      ewin->active = 1;
    }
  else
    {
      if ((ewin->prop & NO_TITLE) == 0)
	{
#ifdef	  IMLIB
	  if (gl_epiwin.titlebar.image_inactive)
	    draw_titlebar_image(ewin, state);
	  else
	    {
#endif
	      if (gl_epiwin.titlebar.grad_inactive_type)
		draw_gradient(ewin, ewin->titlebar_win, "titlebar", state);
	      else
		{
		  XSetWindowBackground(gl_dpy, ewin->titlebar_win, 
				       gl_epiwin.titlebar.color_inactive.pixel);
		  XClearWindow(gl_dpy, ewin->titlebar_win);
		}
#ifdef	  IMLIB
	    }
#endif
	}
      XSetWindowBackground(gl_dpy, ewin->border_frame, 
			   gl_epiwin.color_inactive.pixel);
      XClearWindow(gl_dpy, ewin->border_frame);
      if (gl_epiwin.focus_mode == FOCUS_FOLLOW_MOUSE)
	XSetInputFocus(gl_dpy, gl_scr_info.root_win, 
		       RevertToParent, CurrentTime);
      ewin->active = 0;
      if ((gl_epiwin.current) && (gl_epiwin.current == ewin))
	gl_epiwin.current = NULL;
    }
  if ((ewin->prop & NO_TITLE) == 0)
    {
      if (gl_epiwin.titlebar.width_mode)
	{
	  if ((gl_epiwin.titlebar.position == TOP)
	      || (gl_epiwin.titlebar.position == BOTTOM))
	    {
	      if (gl_epiwin.titlebar.align_shape == ALIGN_LEFT)
		XDrawLine(gl_dpy, ewin->titlebar_win, gc_black,
			  get_titlebar_width(ewin) - ewin->titlebar_border, 0,
			  get_titlebar_width(ewin) - ewin->titlebar_border,
			  gl_epiwin.titlebar.height);
	      else
		{
		  XGetWindowAttributes(gl_dpy, ewin->parent, &attrib);
		  XDrawLine(gl_dpy, ewin->titlebar_win, gc_black,
			    attrib.width - get_titlebar_width(ewin)
			    - ewin->titlebar_border * 2, 0,
			    attrib.width - get_titlebar_width(ewin)
			    - ewin->titlebar_border * 2,
			    gl_epiwin.titlebar.height);
		}
	    }
	  else
	    {
	      XDrawLine(gl_dpy, ewin->titlebar_win, gc_black, 0,
			get_titlebar_width(ewin) - ewin->titlebar_border,
			gl_epiwin.titlebar.height,
			get_titlebar_width(ewin) - ewin->titlebar_border);
	    }
	}
      titlebar_button_state(ewin, state);
      draw_titlebar(ewin, ewin->titlebar_win, state);
    }
  /*  if (gl_epiwin.current == ewin)
      XSync(gl_dpy, 1);
      else
      while (expose)
      {
      XPeekEvent(gl_dpy, &gl_event);
      if ((gl_event.type == Expose))
      XNextEvent(gl_dpy, &gl_event);
      else 
      expose = 0;
      }
  */
  if (gl_menu.here_task)
    draw_menu_task();
}


/* write application's name on titlebar */
void			draw_titlebar(t_win	*ewin,
				      Window	titlebar,
				      char	*state)
{
  char			*name;
  int			font_direction;
  int			top_size;
  int			bottom_size;
  int			start;
  int			x;
  int			y;
  XCharStruct		dim;
  XWindowAttributes	attrib;
  
  if (ewin->prop & NO_TITLE)
    return ;
  XGetWindowAttributes(gl_dpy, titlebar, &attrib);
  XFetchName(gl_dpy, titlebar, &name);
  if (gl_epiwin.titlebar.position == LEFT)
    rev_str(name);
  if (name)
    XTextExtents(gl_epiwin.titlebar.font, name, strlen(name), &font_direction,
		 &top_size, &bottom_size, &dim);
  if (gl_epiwin.titlebar.position == TOP
      || gl_epiwin.titlebar.position == BOTTOM)
    {
      if (gl_epiwin.titlebar.align == ALIGN_RIGHT)
	{
	  if (gl_epiwin.titlebar.width_mode)
	    {
	      if (gl_epiwin.titlebar.align_shape == ALIGN_LEFT)
		start = get_titlebar_width(ewin) - dim.width
		  - gl_epiwin.titlebar.space;
	      else
		start = attrib.width - dim.width
		  - gl_epiwin.titlebar.space;
	    }
	  else
	    start = attrib.width - gl_epiwin.titlebar.space - dim.width;
	}
      else if (gl_epiwin.titlebar.align == ALIGN_CENTER)
	{
	  if (gl_epiwin.titlebar.width_mode)
	    {
	      if (gl_epiwin.titlebar.align_shape == ALIGN_RIGHT)
		start = attrib.width - get_titlebar_width(ewin) +
		  ((get_titlebar_width(ewin) - dim.width) / 2);
	      else
		start = (get_titlebar_width(ewin) - dim.width) / 2;
	    }
	  else
	    start = (attrib.width - dim.width) / 2;
	}
      else
	{
	  if (gl_epiwin.titlebar.width_mode)
	    {
	      if (gl_epiwin.titlebar.align_shape == ALIGN_LEFT)
		start = gl_epiwin.titlebar.space;
	      else
		start = attrib.width - get_titlebar_width(ewin) 
		  + gl_epiwin.titlebar.space + ewin->titlebar_border;
	    }
	  else
	    start = gl_epiwin.titlebar.space;
	}
      x = start;
      y = ewin->titlebar_height / 2 + top_size / 2;
      if (!strcmp(state, "active"))
	{
	  if (name)
	    XDrawString(gl_dpy, titlebar, gl_epiwin.titlebar.font_active,
			x, y, name, strlen(name));
	}
      else
	if (name)
	  XDrawString(gl_dpy, titlebar, gl_epiwin.titlebar.font_inactive,
		      x, y, name, strlen(name)); 
    }
  else
    {
      /* Added by a stranger */
      if (gl_epiwin.titlebar.width_mode)
	{
	  if (gl_epiwin.titlebar.align_shape == ALIGN_LEFT)
	    {
	      if (gl_epiwin.titlebar.align == ALIGN_RIGHT)
		start = get_titlebar_width(ewin) - dim.width
		  - gl_epiwin.titlebar.space;
	      else if (gl_epiwin.titlebar.align == ALIGN_CENTER)
		start = ((get_titlebar_width(ewin) - dim.width) / 2);
	      else
		start = gl_epiwin.titlebar.space;
	    }
	  else
	    {
	      if (gl_epiwin.titlebar.align == ALIGN_RIGHT)
		start = attrib.height - dim.width - gl_epiwin.titlebar.space;
	      else if (gl_epiwin.titlebar.align == ALIGN_CENTER)
		start =  (attrib.height - get_titlebar_width(ewin))
		  + ((get_titlebar_width(ewin) - dim.width) / 2);
	      else
		start = (attrib.height - get_titlebar_width(ewin))
		  + gl_epiwin.titlebar.space;
	    }
	}
      else
	{
	  if (gl_epiwin.titlebar.align == ALIGN_RIGHT)
	    start = attrib.height - dim.width - gl_epiwin.titlebar.space;
	  else if (gl_epiwin.titlebar.align == ALIGN_CENTER)
	    start = ((attrib.height - dim.width) / 2);
	  else
	    start = gl_epiwin.titlebar.space;
	}
      /* This might want to be adjusted for appearance.*/
      x = (gl_epiwin.titlebar.height
	   - (gl_epiwin.titlebar.font->ascent
	      + gl_epiwin.titlebar.font->descent)) / 2;
      y = start;
      if (!strcmp(state, "active"))
	{
	  if (name)
	    {
	      int i, j;
	      int chr;
	      
	      for (i = 0; i < strlen(name); i++)
		{
		  chr = name[i] - 32;
		  if (chr >= 0 && chr <= 96)
		    {
		      if (gl_fn[chr].width)
			{
			  for (j = 0; j < gl_fn[chr].count; j++)
			    {
			      gl_fn[chr].points[j].x += x;
			      gl_fn[chr].points[j].y += y;
			    }
			  XDrawPoints(gl_dpy, titlebar,
				      gl_epiwin.titlebar.font_active,
				      gl_fn[chr].points, gl_fn[chr].count,
				      CoordModeOrigin);
			  for(j = 0;j < gl_fn[chr].count; j++)
			    {
			      gl_fn[chr].points[j].x -= x;
			      gl_fn[chr].points[j].y -= y;
			    }
			  y += gl_fn[chr].width;
			}
		    }
		}
	    }
	}
      else if (name)
	{
	  int i, j;
	  int chr;
	  
	  for (i = 0;i < strlen(name); i++)
	    {
	      chr = name[i] - 32;
	      if(chr >= 0 && chr <= 96)
		{
		  if (gl_fn[chr].width)
		    {
		      for(j = 0; j < gl_fn[chr].count; j++)
			{
			  gl_fn[chr].points[j].x += x;
			  gl_fn[chr].points[j].y += y;
			}
		      XDrawPoints(gl_dpy, titlebar,
				  gl_epiwin.titlebar.font_inactive,
				  gl_fn[chr].points, gl_fn[chr].count,
				  CoordModeOrigin);
		      for(j = 0; j < gl_fn[chr].count; j++)
			{
			  gl_fn[chr].points[j].x -= x;
			  gl_fn[chr].points[j].y -= y;
			}
		      y += gl_fn[chr].width;
		    }
		}
	    }
	}
    }
  if (name)
    free(name);
}

#ifdef	  IMLIB
void		draw_titlebar_image(ewin, state)
     t_win		*ewin;
     char		*state;
{
  int			width;
  int			height;
  Pixmap		pixmap;
  Pixmap		mask;
  XWindowAttributes	attrib;
  
  if (ewin->prop & NO_TITLE)
    return ;
  if (!strcmp(state, "active"))
    {
      if (!(gl_epiwin.titlebar.image =
	    Imlib_load_image(gl_imlib_data, gl_epiwin.titlebar.image_active)))
	{
	  fprintf(stderr, "cannot load titlebar active image\n");
	  exit(ERROR);
	}
    }
  else
    {
      if (!(gl_epiwin.titlebar.image =
	    Imlib_load_image(gl_imlib_data,
			     gl_epiwin.titlebar.image_inactive)))
	{
	  fprintf(stderr, "cannot load titlebar inactive image\n");
	  exit(ERROR);
	}
    }
  if (gl_epiwin.titlebar.image_max)
    {
      if (gl_epiwin.titlebar.width_mode)
	width = get_titlebar_width(ewin);
      else
	{
	  XGetWindowAttributes(gl_dpy, ewin->titlebar_win, &attrib);
	  width = attrib.width;
	}
      height = attrib.height;
    }
  else
    {
      width = gl_epiwin.titlebar.image->rgb_width;
      height = gl_epiwin.titlebar.image->rgb_height;
    }
  Imlib_render(gl_imlib_data, gl_epiwin.titlebar.image, 
	       width, height);  
  pixmap = Imlib_move_image(gl_imlib_data, 
			    gl_epiwin.titlebar.image);
  mask = Imlib_move_mask(gl_imlib_data, 
			 gl_epiwin.titlebar.image);
  XSetWindowBackgroundPixmap(gl_dpy, ewin->titlebar_win, pixmap);
  XClearWindow(gl_dpy, ewin->titlebar_win);
}
#endif

Pixel		*alloc_linear_gradient(s_from, s_to, npixels)
     char		*s_from;
     char		*s_to;
     int		npixels;
{
  Pixel		*pixels;
  XColor	from;
  XColor	to;
  XColor	c;
  int		r;
  int		dr;
  int		g;
  int		dg;
  int		b;
  int		db;
  int		i; 
  int		got_all;
  
  got_all = 1;
  if (npixels < 1) 
    return (NULL);
  if (!s_from || !XParseColor(gl_dpy, 
			      XDefaultColormap(gl_dpy, gl_scr_info.screen),
			      s_from, &from))
    return (NULL);
  if (!s_to || !XParseColor(gl_dpy,
			    XDefaultColormap(gl_dpy, gl_scr_info.screen), 
			    s_to, &to))
    return (NULL);
  c = from;
  r = from.red; 
  dr = (to.red - from.red) / npixels;
  g = from.green; 
  dg = (to.green - from.green) / npixels;
  b = from.blue; 
  db = (to.blue - from.blue) / npixels;
  pixels = xmalloc(sizeof (Pixel) * npixels);
  c.flags = DoRed | DoGreen | DoBlue;
  for (i = 0; i < npixels; ++i)
    {
      if (!XAllocColor(gl_dpy, XDefaultColormap(gl_dpy, gl_scr_info.screen),
		       &c))
	got_all = 0;
      pixels[i] = c.pixel;
      c.red = (unsigned short) (r += dr);
      c.green = (unsigned short) (g += dg);
      c.blue = (unsigned short) (b += db);
    }
  if (!got_all) 
    {
      char s[256];
      sprintf(s, "color gradient %s to %s\n", s_from, s_to);
    }
  return (pixels);
}

Pixel		*alloc_nonlinear_gradient(s_colors, clen, nsegs, npixels)
     char		*s_colors[];
     int		clen[];
     int		nsegs;
     int		npixels;
{
  Pixel		*pixels;
  Pixel		*p;
  int		i;
  int		j;
  int		n;
  int		curpixel;
  int		perc;  
  
  curpixel = 0;
  perc = 0;
  pixels = xmalloc(sizeof (Pixel) * npixels);
  if (nsegs < 1) 
    {
      free(pixels);
      return (NULL);
    }
  for (i = 0; i < npixels; i++)
    pixels[i] = 0;
  for (i = 0; (i < nsegs) && (curpixel < npixels) && (perc <= 100); i++) 
    {
      p = 0;
      n = clen[i] * npixels / 100;
      p = alloc_linear_gradient(s_colors[i], s_colors[i + 1], n);
      if (!p) 
	{
	  free(pixels);
	  return (NULL);
	}
      for (j = 0; j < n; j++)
	pixels[curpixel + j] = p[j];
      free(p);
      perc += clen[i];
      curpixel += n;
    }
  for (i = curpixel; i < npixels; i++)
    pixels[i] = pixels[i - 1];
  return (pixels);
}

int		gradient(color1, color2, type, target, status)
     char		*color1;
     char		*color2;
     char		*type;
     char		*target;
     char		*status;
{
  char		**s_colors;
  int		nsegs;
  int		i;
  int		sum;
  int		*perc;
  Pixel		*pixels;
  
  sum = 0;
  s_colors = (char **) xmalloc(sizeof (char *) * 2);
  perc = (int *) xmalloc(sizeof (int));
  nsegs = 1;
  s_colors[0] = (char *) xmalloc(sizeof (char) * (strlen(color1) + 1));
  strcpy(s_colors[0], color1);
  s_colors[1] = (char *) xmalloc(sizeof (char) * (strlen(color2) + 1));
  strcpy(s_colors[1], color2);
  perc[0] = 100;
  for (i = 0; i < nsegs; ++i)
    sum += perc[i];
  if (sum != 100) 
    {
      free(s_colors[0]);
      free(s_colors[1]);
      free(s_colors);
      free(perc);
      return (0);
    }
  pixels = alloc_nonlinear_gradient(s_colors, perc, nsegs, GRAD_NB_PIXEL);
  free(s_colors[0]);
  free(s_colors[1]);
  free(s_colors);
  free(perc);
  if (!pixels) 
    return (0);
  if (!strcmp(target, "titlebar"))
    {
      if (!strcmp("active", status))
	{
	  gl_epiwin.titlebar.pixels_active = pixels;
	  gl_epiwin.titlebar.grad_active_type = *type;
	}
      else
	{
	  gl_epiwin.titlebar.pixels_inactive = pixels;
	  gl_epiwin.titlebar.grad_inactive_type = *type;
	}
    }
  else
    {
      gl_menu.pixels = pixels;
      gl_menu.grad_type = *type;
    }
  return (1);
}

void			draw_gradient(ewin, win, target, status)
     t_win			*ewin;
     Window			win;
     char			*target;
     char			*status;
{
  XRectangle		bounds;
  XWindowAttributes	attrib;
  char			type;
  Pixel			*pixels;
  
  XGetWindowAttributes(gl_dpy, win, &attrib);
  if (!strcmp(target, "titlebar"))
    {
      if (ewin->prop & NO_TITLE)
	return ;
      if (!strcmp("active", status))
	{
	  type = gl_epiwin.titlebar.grad_active_type;
	  pixels = gl_epiwin.titlebar.pixels_active;
	}
      else
	{
	  type = gl_epiwin.titlebar.grad_inactive_type;
	  pixels = gl_epiwin.titlebar.pixels_inactive;
	}
      if ((gl_epiwin.titlebar.position == TOP)
	  || (gl_epiwin.titlebar.position == BOTTOM))
	{
	  if (gl_epiwin.titlebar.width_mode)
	    {
	      if (gl_epiwin.titlebar.align_shape == ALIGN_LEFT)
		bounds.x = 0;
	      else
		bounds.x = attrib.width - get_titlebar_width(ewin)
		  + ewin->titlebar_border;
	      bounds.y = 0;
	      bounds.width = get_titlebar_width(ewin) 
		- ewin->titlebar_border;
	    }
	  else
	    {
	      bounds.width = attrib.width;
	      bounds.x = bounds.y = 0;
	    }
	  bounds.height = attrib.height;
	}
      else
	{
	  if (gl_epiwin.titlebar.width_mode)
	    {
	      if (gl_epiwin.titlebar.align_shape == ALIGN_LEFT)
		bounds.y = 0;
	      else
		bounds.y = attrib.height - get_titlebar_width(ewin)
		  + ewin->titlebar_border;
	      bounds.height = get_titlebar_width(ewin) 
		- ewin->titlebar_border;
	    }
	  else
	    {
	      bounds.y = 0;
	      bounds.height = attrib.height;
	    }
	  bounds.x = 0;
	  bounds.width = attrib.width;
	}
    }
  else
    {
      pixels = gl_menu.pixels;
      type = gl_menu.grad_type;
      bounds.x = bounds.y = 0;
      bounds.width = attrib.width;
      bounds.height = attrib.height;
    }
  /*	flush_expose(win); */
  if (type == 'H') 
    {
      register int i = 0, dw = bounds.width / GRAD_NB_PIXEL + 1;
      while (i < GRAD_NB_PIXEL)
	{
	  unsigned short x = i * bounds.width / GRAD_NB_PIXEL;
	  XSetForeground(gl_dpy, gl_epiwin.transmask_gc, pixels[i++]);
	  XFillRectangle(gl_dpy, win, gl_epiwin.transmask_gc,
			 bounds.x + x, bounds.y, dw, bounds.height);
	}
    } 
  else 
    {
      register int i = 0, dh = bounds.height / GRAD_NB_PIXEL + 1;
      while (i < GRAD_NB_PIXEL)
	{
	  unsigned short y = i * bounds.height / GRAD_NB_PIXEL;
	  XSetForeground(gl_dpy, gl_epiwin.transmask_gc, pixels[i++]);
	  XFillRectangle(gl_dpy, win, gl_epiwin.transmask_gc,
			 bounds.x, bounds.y + y, bounds.width, dh);
	}
    }
}
