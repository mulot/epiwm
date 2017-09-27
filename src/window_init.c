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
** window_init.c
**
** Made by (mulot)
** Login   <mulot_j@epita.fr>
**
** Started on  Wed Mar 17 13:33:36 1999 mulot
 ** Last update Mon Oct 23 23:18:10 2000 Julien Mulot
*/


#include "epiwm.h"


char	set_align(char *align)
{
  if (!strcmp("left", align))
    return (ALIGN_LEFT);
  else if (!strcmp("center", align))
    return (ALIGN_CENTER);
  else
    return (ALIGN_RIGHT);
}


int			get_titlebar_width(t_win *ewin)
{
  XWindowAttributes	attrib;
  
  if (gl_epiwin.titlebar.width_mode)
    {
      if ((gl_epiwin.titlebar.width_mode == IN_PIXEL))
	return (gl_epiwin.titlebar.width);
      if ((gl_epiwin.titlebar.position == TOP)
	  || (gl_epiwin.titlebar.position == BOTTOM))
	{
	  XGetWindowAttributes(gl_dpy, ewin->border_frame, &attrib);
	  return ((gl_epiwin.titlebar.width 
		   * (attrib.width + ewin->bf_border * 2)) / 100);
	}
      else
	{
	  XGetWindowAttributes(gl_dpy, ewin->border_frame, &attrib);
	  return ((gl_epiwin.titlebar.width 
		   * (attrib.height + ewin->bf_border * 2)) / 100);
	}
    }
  else
    {
      XGetWindowAttributes(gl_dpy, ewin->parent, &attrib);
      return (attrib.width);
    }
}


void	init_titlebar_width(char *size)
{
  if (size[strlen(size) - 1] == '%')
    {
      gl_epiwin.titlebar.width_mode = IN_PERCENT;
      size[strlen(size) - 1] = '\0';
    }
  else
    gl_epiwin.titlebar.width_mode = IN_PIXEL;
  gl_epiwin.titlebar.width = atoi(size);
}


void	set_focus_mode(char *mode)
{
  if (!strcmp("SloppyFocus", mode))
    gl_epiwin.focus_mode = FOCUS_SLOPPY;
  else if (!strcmp("FocusFollowMouse", mode))
    gl_epiwin.focus_mode = FOCUS_FOLLOW_MOUSE;
  else if (!strcmp("ClickToFocus", mode))
    gl_epiwin.focus_mode = FOCUS_CLICK;
}


void	set_titlebar_position(char *position)
{
  if (!strcmp("top", position))
    gl_epiwin.titlebar.position = TOP;
  else if (!strcmp("left", position))
    gl_epiwin.titlebar.position = LEFT;
  else if (!strcmp("right", position))
    gl_epiwin.titlebar.position = RIGHT;
  else if (!strcmp("bottom", position))
    gl_epiwin.titlebar.position = BOTTOM;
}


#ifdef	IMLIB
void	set_btn_size(char *image)
{
  if (!(gl_epiwin.titlebar.image =
	Imlib_load_image(gl_imlib_data, image)))
    {
      fprintf(stderr, "cannot load button image %s\n", image);
      exit(ERROR);
    }
  gl_epiwin.btn_width = gl_epiwin.titlebar.image->rgb_width;
  gl_epiwin.btn_height = gl_epiwin.titlebar.image->rgb_height;
}


void	set_titlebar_btn_size()
{
  if (gl_epiwin.titlebar.img_btn_close_active)
    set_btn_size(gl_epiwin.titlebar.img_btn_close_active);
  else if (gl_epiwin.titlebar.img_btn_destroy_active)
    set_btn_size(gl_epiwin.titlebar.img_btn_destroy_active);
  else if (gl_epiwin.titlebar.img_btn_max_active)
    set_btn_size(gl_epiwin.titlebar.img_btn_max_active);
  else if (gl_epiwin.titlebar.img_btn_iconify_active)
    set_btn_size(gl_epiwin.titlebar.img_btn_iconify_active);
  else if (gl_epiwin.titlebar.img_btn_shade_active)
    set_btn_size(gl_epiwin.titlebar.img_btn_shade_active);
  else if (gl_epiwin.titlebar.img_btn_max_height_active)
    set_btn_size(gl_epiwin.titlebar.img_btn_max_height_active);
  else if (gl_epiwin.titlebar.img_btn_max_width_active)
    set_btn_size(gl_epiwin.titlebar.img_btn_max_width_active);
}
#endif


int	set_title_position()
{
#define SPACE 4
  int	space;
  
  space = 0;
  if (gl_epiwin.titlebar.align == ALIGN_LEFT)
    {
      if (gl_epiwin.align_btn_close == ALIGN_LEFT)
	if (space < gl_epiwin.pos_btn_close)
	  space = gl_epiwin.pos_btn_close;
      if (gl_epiwin.align_btn_destroy == ALIGN_LEFT)
	if (space < gl_epiwin.pos_btn_destroy)
	  space = gl_epiwin.pos_btn_destroy;
      if (gl_epiwin.align_btn_iconify == ALIGN_LEFT)
	if (space < gl_epiwin.pos_btn_iconify)
	  space = gl_epiwin.pos_btn_iconify;
      if (gl_epiwin.align_btn_max == ALIGN_LEFT)
	if (space < gl_epiwin.pos_btn_max)
	  space = gl_epiwin.pos_btn_max;
      if (gl_epiwin.align_btn_max_height == ALIGN_LEFT)
	if (space < gl_epiwin.pos_btn_max_height)
	  space = gl_epiwin.pos_btn_max_height;
      if (gl_epiwin.align_btn_max_width == ALIGN_LEFT)
	if (space < gl_epiwin.pos_btn_max_width)
	  space = gl_epiwin.pos_btn_max_width;
      if (gl_epiwin.align_btn_shade == ALIGN_LEFT)
	if (space < gl_epiwin.pos_btn_shade)
	  space = gl_epiwin.pos_btn_shade;
      return (space + gl_epiwin.btn_width + SPACE);
    }
  if (gl_epiwin.titlebar.align == ALIGN_RIGHT)
    {
      if (gl_epiwin.align_btn_close == ALIGN_RIGHT)
	if (space < gl_epiwin.pos_btn_close)
	  space = gl_epiwin.pos_btn_close;
      if (gl_epiwin.align_btn_destroy == ALIGN_RIGHT)
	if (space < gl_epiwin.pos_btn_destroy)
	  space = gl_epiwin.pos_btn_destroy;
      if (gl_epiwin.align_btn_iconify == ALIGN_RIGHT)
	if (space < gl_epiwin.pos_btn_iconify)
	  space = gl_epiwin.pos_btn_iconify;
      if (gl_epiwin.align_btn_max == ALIGN_RIGHT)
	if (space < gl_epiwin.pos_btn_max)
	  space = gl_epiwin.pos_btn_max;
      if (gl_epiwin.align_btn_max_height == ALIGN_RIGHT)
	if (space < gl_epiwin.pos_btn_max_height)
	  space = gl_epiwin.pos_btn_max_height;
      if (gl_epiwin.align_btn_max_width == ALIGN_RIGHT)
	if (space < gl_epiwin.pos_btn_max_width)
	  space = gl_epiwin.pos_btn_max_width;
      if (gl_epiwin.align_btn_shade == ALIGN_RIGHT)
	if (space < gl_epiwin.pos_btn_shade)
	  space = gl_epiwin.pos_btn_shade;
      return (space + gl_epiwin.btn_width + SPACE);
    }
  return (0);
}


void	button_init(char *align, int pos, char *btn_align, int *btn_pos)
{
  *btn_pos = pos;
  if (!strcmp(align, "right"))
    *btn_align = ALIGN_RIGHT;
  else
    *btn_align = ALIGN_LEFT;
}


void		window_build(FILE *file)
{
  char		line[LINE_LEN];
  char		*tmp;
  
  while (fgets(line, LINE_LEN, file) != NULL)
    {
      if ((*line != '#') && (*line != '\t') && (*line != '\n')
	  && (*line != ' '))
	{
	  if (strlen(line) != 1)
	    line[strlen(line) - 1] = ' ';
	  tmp = index_file(line, 0);
	  if (!strcasecmp(tmp, "FocusMode"))
	    set_focus_mode(index_file(line, 1));
	  else if (!strcasecmp(tmp, "OpaqueMove"))
	    gl_epiwin.prop |= OPAQ_MOVE;
	  else if (!strcasecmp(tmp, "AutoRaise"))
	    gl_epiwin.prop |= AUTO_RAISE;
	  else if (!strcasecmp(tmp, "NewAutoFocus"))
	    gl_epiwin.prop |= NEW_AUTO_FOCUS;
	  else if (!strcasecmp(tmp, "SmartPlacement"))
	    gl_epiwin.prop |= SMART_PLACE;
	  else if (!strcasecmp(tmp, "BorderActiveColor"))
	    init_color(index_file(line, 1), &gl_epiwin.color_active);
	  else if (!strcasecmp(tmp, "BorderInactiveColor"))
	    init_color(index_file(line, 1), &gl_epiwin.color_inactive);
	  else if (!strcasecmp(tmp, "BorderStickColor"))
	    init_color(index_file(line, 1), &gl_epiwin.color_stick);
	  else if (!strcasecmp(tmp, "BorderWidth"))
	    gl_epiwin.border_size = atoi(index_file(line, 1));
	  else if (!strcasecmp(tmp, "TitleFontActiveColor"))
	    init_font_color(&gl_epiwin.titlebar.font_active,
			    index_file(line, 1));
	  else if (!strcasecmp(tmp, "TitleFontInactiveColor"))
	    init_font_color(&gl_epiwin.titlebar.font_inactive, 
			    index_file(line, 1));
	  else if (!strcasecmp(tmp, "TitleActiveColor"))
	    {
	      if (!strcmp(index_file(line, 1),"H")
		  || !strcmp(index_file(line, 1),"V"))
		gradient(index_file(line, 2), index_file(line, 3),
			 index_file(line, 1), "titlebar", "active");
	      else
		init_color(index_file(line, 1),
			   &gl_epiwin.titlebar.color_active);
	    }
	  else if (!strcasecmp(tmp, "TitleInactiveColor"))
	    {
	      if (!strcmp(index_file(line, 1),"H")
		  ||!strcmp(index_file(line, 1),"V"))
		gradient(index_file(line, 2), index_file(line, 3),
			 index_file(line, 1), "titlebar", "inactive");
	      else
		init_color(index_file(line, 1),
			   &gl_epiwin.titlebar.color_inactive);
	    }
	  else if (!strcasecmp(tmp, "TitleHeight"))
	    gl_epiwin.titlebar.height = atoi(index_file(line, 1));
	  else if (!strcasecmp(tmp, "TitleWidth"))
	    init_titlebar_width(index_file(line, 1));
	  else if (!strcasecmp(tmp, "TitlePosition"))
	    set_titlebar_position(index_file(line, 1));
	  else if (!strcasecmp(tmp, "TitleAlign"))
	    gl_epiwin.titlebar.align = set_align(index_file(line, 1));
	  else if (!strcasecmp(tmp, "TitleShapeAlign"))
	    gl_epiwin.titlebar.align_shape = set_align(index_file(line, 1));
	  else if (!strcasecmp(tmp, "TitleFont"))
	    {
	      gl_epiwin.titlebar.font = XLoadQueryFont(gl_dpy, 
						       index_file(line, 1));
	      XSetFont(gl_dpy, gl_epiwin.titlebar.font_active, 
		       gl_epiwin.titlebar.font->fid);
	      XSetFont(gl_dpy, gl_epiwin.titlebar.font_inactive, 
		       gl_epiwin.titlebar.font->fid);
	    }
	  else if (!strcasecmp(tmp, "CloseButton"))
	    button_init(index_file(line, 1), atoi(index_file(line, 2)),
			&gl_epiwin.align_btn_close, 
			&gl_epiwin.pos_btn_close);
	  else if (!strcasecmp(tmp, "DestroyButton"))
	    button_init(index_file(line, 1), atoi(index_file(line, 2)),
			&gl_epiwin.align_btn_destroy, 
			&gl_epiwin.pos_btn_destroy);
	  else if (!strcasecmp(tmp, "MaxButton"))
	    button_init(index_file(line, 1), atoi(index_file(line, 2)),
			&gl_epiwin.align_btn_max, 
			&gl_epiwin.pos_btn_max);
	  else if (!strcasecmp(tmp, "IconifyButton"))
	    button_init(index_file(line, 1), atoi(index_file(line, 2)),
			&gl_epiwin.align_btn_iconify, 
			&gl_epiwin.pos_btn_iconify);
	  else if (!strcasecmp(tmp, "ShadeButton"))
	    button_init(index_file(line, 1), atoi(index_file(line, 2)),
			&gl_epiwin.align_btn_shade, 
			&gl_epiwin.pos_btn_shade);
	  else if (!strcasecmp(tmp, "MaxHeightButton"))
	    button_init(index_file(line, 1), atoi(index_file(line, 2)),
			&gl_epiwin.align_btn_max_height, 
			&gl_epiwin.pos_btn_max_height);
	  else if (!strcasecmp(tmp, "MaxWidthButton"))
	    button_init(index_file(line, 1), atoi(index_file(line, 2)),
			&gl_epiwin.align_btn_max_width, 
			&gl_epiwin.pos_btn_max_width);
#ifdef	  IMLIB
	  else if (!strcasecmp(tmp, "TitleActiveImage"))
	    init_image(index_file(line, 1), &gl_epiwin.titlebar.image_active);
	  else if (!strcasecmp(tmp, "TitleInactiveImage"))
	    init_image(index_file(line, 1), &gl_epiwin.titlebar.image_inactive);
	  else if (!strcasecmp(tmp, "CloseButtonImageActive"))
	    init_image(index_file(line, 1),
		       &gl_epiwin.titlebar.img_btn_close_active);
	  else if (!strcasecmp(tmp, "DestroyButtonImageActive"))
	    init_image(index_file(line, 1),
		       &gl_epiwin.titlebar.img_btn_destroy_active);
	  else if (!strcasecmp(tmp, "MaxButtonImageActive"))
	    init_image(index_file(line, 1),
		       &gl_epiwin.titlebar.img_btn_max_active);
	  else if (!strcasecmp(tmp, "IconifyButtonImageActive"))
	    init_image(index_file(line, 1),
		       &gl_epiwin.titlebar.img_btn_iconify_active);
	  else if (!strcasecmp(tmp, "ShadeButtonImageActive"))
	    init_image(index_file(line, 1),
		       &gl_epiwin.titlebar.img_btn_shade_active);
	  else if (!strcasecmp(tmp, "CloseButtonImageInactive"))
	    init_image(index_file(line, 1),
		       &gl_epiwin.titlebar.img_btn_close_inactive);
	  else if (!strcasecmp(tmp, "DestroyButtonImageInactive"))
	    init_image(index_file(line, 1),
		       &gl_epiwin.titlebar.img_btn_destroy_inactive);
	  else if (!strcasecmp(tmp, "MaxButtonImageInactive"))
	    init_image(index_file(line, 1),
		       &gl_epiwin.titlebar.img_btn_max_inactive);
	  else if (!strcasecmp(tmp, "IconifyButtonImageInactive"))
	    init_image(index_file(line, 1),
		       &gl_epiwin.titlebar.img_btn_iconify_inactive);
	  else if (!strcasecmp(tmp, "ShadeButtonImageInactive"))
	    init_image(index_file(line, 1),
		       &gl_epiwin.titlebar.img_btn_shade_inactive);
	  else if (!strcasecmp(tmp, "TitleImageMax"))
	    gl_epiwin.titlebar.image_max = 1;
#endif
	}
    }
}

