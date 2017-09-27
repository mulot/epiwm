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
** iconify.c
**
** Made by (Poindessous Thomas)
** Login   <poinde_t@epita.fr>
**
** Started on  Wed Mar  3 15:46:08 1999 Poindessous Thomas
 ** Last update Mon Oct 23 23:15:52 2000 Julien Mulot
*/


#include "epiwm.h"


void		iconify_add_elt(t_win *ewin, int width, int height)
{
  t_icon_list	*new_icon;
  
  new_icon = xmalloc(sizeof (t_icon_list));
  new_icon->next = NULL;
  new_icon->win = ewin;
  new_icon->width = width;
  new_icon->height = height;
  if (!(gl_epiwin.current_workspace->first_icon))
    {
      new_icon->prec = NULL;
      gl_epiwin.current_workspace->first_icon = new_icon;
      gl_epiwin.current_workspace->last_icon = new_icon;
      new_icon->nb = 0;
    }
  else
    {
      gl_epiwin.current_workspace->last_icon->next = new_icon;
      new_icon->prec = gl_epiwin.current_workspace->last_icon;
      new_icon->nb = gl_epiwin.current_workspace->last_icon->nb + 1;
      gl_epiwin.current_workspace->last_icon = new_icon;
    }
}


void		iconify_rm_elt(t_win *ewin)
{
  t_icon_list	*elt;
  
  elt = iconify_search_elt(ewin);
  if (elt)
    {
      if (gl_epiwin.current_workspace->first_icon == elt)
	{
	  if (gl_epiwin.current_workspace->last_icon == elt)
	    {
	      gl_epiwin.current_workspace->last_icon = NULL;
	      gl_epiwin.current_workspace->first_icon = NULL;
	    }
	  else
	    {
	      gl_epiwin.current_workspace->first_icon = elt->next;
	      iconify_re_number(gl_epiwin.current_workspace->first_icon);
	      iconify_re_draw(gl_epiwin.current_workspace->first_icon);
	    }
	}
      else
	{
	  if (gl_epiwin.current_workspace->last_icon == elt)
	    {
	      gl_epiwin.current_workspace->last_icon = elt->prec;
	      elt->prec->next = elt->next;
	    }
	  else
	    {
	      elt->prec->next = elt->next;
	      elt->next->prec = elt->prec;
	    }
	  iconify_re_number(elt->prec->next);
	  iconify_re_draw(elt->prec->next);
	}
      free(elt);
      ewin->iconify_title_win = 0;
    }
}


t_icon_list	*iconify_search_elt(t_win *ewin)
{
  t_icon_list	*tmp;
  
  tmp = gl_epiwin.current_workspace->first_icon;
  if (tmp)
    {
      while ((tmp != gl_epiwin.current_workspace->last_icon)
	     && (tmp->win != ewin))
	tmp = tmp->next;
      if (tmp->win == ewin)
	return (tmp);
    }
  return (NULL);
}


#ifdef	  IMLIB
void		draw_icon_image(Window icon, char *state)
{
  int		width;
  int		height;
  Pixmap	pixmap;
  
  if (!gl_icon.showed)
    return ;
  if (!strcmp("active", state))
    {
      if (!(gl_icon.image = Imlib_load_image(gl_imlib_data,
					     gl_icon.image_active)))
	{
	  fprintf(stderr, "cannot load icon active image\n");
	  exit(ERROR);
	}
    }
  else
    {
      if (!(gl_icon.image = Imlib_load_image(gl_imlib_data, 
					     gl_icon.image_inactive)))
	{
	  fprintf(stderr, "cannot load icon inactive image\n");
	  exit(ERROR);
	}
    }
  width = gl_icon.image->rgb_width;
  height = gl_icon.image->rgb_height;
  Imlib_render(gl_imlib_data, gl_icon.image, width, height);  
  pixmap = Imlib_move_image(gl_imlib_data, gl_icon.image);
  XSetWindowBackgroundPixmap(gl_dpy, icon, pixmap);  
  XClearWindow(gl_dpy, icon);
}
#endif


void		draw_icon(Window icon, int width, char *state)
{
  char		*name;
  GC		font_color;
  int		font_direction;
  int		top_size;
  int		bottom_size;
  XCharStruct	dim;
  
  if (!gl_icon.showed)
    return ;
  if (!strcmp(state, "active"))
    font_color = gl_icon.font_active;
  else
    font_color = gl_icon.font_inactive;
  XGetIconName(gl_dpy, icon, &name);
  if (name)
    {
      XTextExtents(gl_icon.font, name, strlen(name), &font_direction,
		   &top_size, &bottom_size, &dim);
      
      /*  XDrawString(gl_dpy, icon, font_color,
	  (width - text_width) / 2, gl_icon.height / 2,
	  name, strlen(name));*/
      
      XDrawString(gl_dpy, icon, font_color,
		  (width - dim.width) / 2, (gl_icon.height + top_size - 2) / 2, 
		  name, strlen(name));
      free(name);
    }
}


Window		add_iconify_win(t_win *ewin)
{
  Window	temp;
  int		width;
  int		height;
  int		icon_x;
  int		icon_y;
  int		nb_icon;
  t_icon_list	*icon;
  
  icon_calculate_width_height(ewin, &width, &height);
  iconify_add_elt(ewin, width, height);
  icon_x = 0;
  icon_y = 0;
  icon = gl_epiwin.current_workspace->first_icon;
  for (nb_icon = gl_epiwin.current_workspace->last_icon->nb;
       nb_icon > 0; nb_icon--)
    {      
      icon_y = icon_y + icon->height;
      icon = icon->next;
    }
  temp = icon_create_win(icon_x, icon_y, gl_icon.width, gl_icon.height);
  XMoveWindow(gl_dpy, temp, icon_x, icon_y + height - gl_icon.height);
  if (ewin->iconify_win)
    XMoveWindow(gl_dpy, ewin->iconify_win, icon_x, icon_y);
  return (temp);
}


void		iconify_re_number(t_icon_list *list)
{
  t_icon_list	*tmp;
  
  tmp = list;
  if (tmp)
    {
      while (tmp != gl_epiwin.current_workspace->last_icon)
	{
	  tmp->nb--;
	  tmp = tmp->next;
	}
      tmp->nb--;
    }
}


void			iconify_re_draw(t_icon_list *list)
{
  t_icon_list		*tmp;
  int			icon_x;
  int			icon_y;
  int			nb_icon;
  t_icon_list		*icon;
  XWindowAttributes	attrib;
  
  tmp = list;
  if (tmp)
    {
      while (tmp != NULL)
	{
	  icon_x = 0;
	  icon_y = 0;
	  icon = gl_epiwin.current_workspace->first_icon;
	  for (nb_icon = tmp->nb; nb_icon > 0; nb_icon--)
	    {      
	      icon_y = icon_y + icon->height;
	      icon = icon->next;
	    }
	  if (tmp->win->iconify_win)
	    XGetWindowAttributes(gl_dpy, tmp->win->iconify_win, &attrib);
	  else
	    XGetWindowAttributes(gl_dpy, tmp->win->iconify_title_win, &attrib);
	  if (attrib.x == 0)
	    {
	      if (tmp->win->iconify_win)
		{
		  XMoveWindow(gl_dpy, tmp->win->iconify_win,
			      0, icon_y);
		  XLowerWindow(gl_dpy, tmp->win->iconify_win);
		  XMoveWindow(gl_dpy, tmp->win->iconify_title_win,
			      0, icon_y + attrib.height);
		  XLowerWindow(gl_dpy, tmp->win->iconify_title_win);
		}
	      else
		{
		  XMoveWindow(gl_dpy, tmp->win->iconify_title_win,
			      0, icon_y);
		  XLowerWindow(gl_dpy, tmp->win->iconify_title_win);
		}
	    }
	  tmp = tmp->next;
	}
    }  
}


Window			icon_create_win(int x, int y, int width, int height)
{
  Window		temp;
  XSetWindowAttributes	attr;
  
  temp = XCreateSimpleWindow(gl_dpy, gl_scr_info.root_win, x, y, width, height,
			     0, 0,
			     gl_icon.color_inactive.pixel);
  attr.event_mask = ButtonPressMask | ButtonReleaseMask
    | EnterWindowMask | LeaveWindowMask | ExposureMask | PointerMotionMask;
  XChangeWindowAttributes(gl_dpy, temp, CWEventMask, &attr);
  return (temp);
}


void			icon_calculate_width_height(t_win	*ewin,
						    int		*width,
						    int		*height)
{
  XWindowAttributes	attrib;
  
  if (ewin->iconify_win)
    {
      XGetWindowAttributes(gl_dpy, ewin->iconify_win, &attrib);
      if (gl_icon.width > attrib.width)
	*width = gl_icon.width;
      else
	*width = attrib.width;
      *height = gl_icon.height + attrib.height;
    }
  else
    {
      *width = gl_icon.width;
      *height = gl_icon.height;
    }
}
