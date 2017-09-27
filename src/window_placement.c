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
 ** window_placement.c
 ** 
 ** Made by (mulot)
 ** Login   <mulot_j@epita.fr>
 ** 
 ** Started on  Wed Jun 16 01:58:53 1999 mulot
 ** Last update Fri Apr  7 18:15:51 2000 Mulot Julien
 */

#include "epiwm.h"

#define SPACE_WIN	2

int			choose_place(t_win *ewin, int *place_x, int *place_y)
{
  t_win_list		*apps_list; 
  XWindowAttributes	attrib;
  XWindowAttributes	app;
  char			win_over_here;
  int			sav_x;
  int			sav_y;
  
  sav_x = 0;
  sav_y = 0;
  XGetWindowAttributes(gl_dpy, ewin->parent, &app);
  win_over_here = 0;
  apps_list = gl_epiwin.first_win_group_list;
  while (apps_list)
    {
      XGetWindowAttributes(gl_dpy, apps_list->p_t_win->parent, &attrib);
      if (attrib.map_state == IsViewable)
	{
	  if ((*place_x + app.width) <= attrib.x)
	    {
	      if ((*place_y + app.height) <= attrib.y)
		{
		}
	    }
	  else
	    {
	      if (*place_x < (attrib.x + attrib.width))
		{
		  if (attrib.y < (*place_y + app.height))
		    {
		      if (*place_y < (attrib.y + attrib.height))
			{
			  if ((attrib.y >= app.y) && (attrib.y <=
						      (app.y + app.width)))
			    {
			      if (sav_x)
				{
				  if ((attrib.x + attrib.width) < sav_x)
				    sav_x = attrib.x + attrib.width;
				}
			      else
				sav_x = attrib.x + attrib.width;
			      if (sav_y)
				{
				  if ((attrib.y + attrib.height) < sav_y)
				    sav_y = attrib.y + attrib.height;
				}
			      else
				sav_y = attrib.y + attrib.height;
			    }
			  win_over_here = 1;
			}
		    }
		}
	    }
	}
      apps_list = apps_list->next_elt;
    }
  if (win_over_here)
    {
      if (!sav_x)
	{
 	  /*printf("fuck !!!\n");*/
	  return (1);
	}
      if ((sav_x + app.width) <= gl_scr_info.dpy_width)
	{
	  *place_x = sav_x;
	  /*	  choose_place(ewin, place_x, place_y);*/
	  return (0);
	}
      else
	{
	  if ((sav_y + app.height) <= gl_scr_info.dpy_height)
	    {
	      *place_x = 0;
	      *place_y = sav_y;
	      /*	      choose_place(ewin, place_x, place_y);*/
	      return (0);
	    }
	  else
	    {
	      *place_x = 20;
	      *place_y = 20;
	    }
	}
    }
  return (1);
}


void			placement_smart(t_win *ewin)
{
  int			place_x;
  int			place_y;
  XWindowAttributes	attrib;
  
  XGetWindowAttributes(gl_dpy, ewin->parent, &attrib);
  if ((attrib.x == 0) && (attrib.y == 0))
    {
      place_y = 0;
      place_x = 0;
      while (!choose_place(ewin, &place_x, &place_y))
	;
      XMoveWindow(gl_dpy, ewin->parent,
		  place_x + SPACE_WIN, place_y + SPACE_WIN);
    }
}


void			max_fit(t_win *ewin)
{
  XWindowAttributes	attrib;
  XWindowAttributes	app;
  int			x;
  int			y;
  int			width;
  int			height;
  t_win_list		*apps_list;
  
  apps_list = gl_epiwin.first_win_group_list;
  XGetWindowAttributes(gl_dpy, ewin->parent, &attrib);
  x = 0;
  while (apps_list)
    {
      if (apps_list->p_t_win != ewin)
	{
	  XGetWindowAttributes(gl_dpy, apps_list->p_t_win->parent, &app);
	  if (app.map_state == IsViewable)
	    {
	      if ((app.x + app.width <= attrib.x) && (((attrib.y >= app.y) && (attrib.y <= (app.y + app.height))) || ((app.y >= attrib.y) && (app.y <= (attrib.y + attrib.height)))))
		if ((app.x + app.width) > x)
		  x = app.x + app.width;
	    }
	}
      apps_list = apps_list->next_elt;
    }
  XMoveWindow(gl_dpy, ewin->parent, x + SPACE_WIN, attrib.y);
  apps_list = gl_epiwin.first_win_group_list;
  XGetWindowAttributes(gl_dpy, ewin->parent, &attrib);
  y = 0;
  while (apps_list)
    {
      if (apps_list->p_t_win != ewin)
	{
	  XGetWindowAttributes(gl_dpy, apps_list->p_t_win->parent, &app);
	  if (app.map_state == IsViewable)
	    {
	      if ((app.y + app.height <= attrib.y) && (((attrib.x >= app.x) && (attrib.x <= (app.x + app.width))) || ((app.x >= attrib.x) && (app.x <= (attrib.x + attrib.width)))))
		if ((app.y + app.height) > y)
		  y = app.y + app.height;

	    }
	}
      apps_list = apps_list->next_elt;
    }
  XMoveWindow(gl_dpy, ewin->parent, attrib.x, y + SPACE_WIN); 
  apps_list = gl_epiwin.first_win_group_list;
  XGetWindowAttributes(gl_dpy, ewin->parent, &attrib);
  width = gl_scr_info.dpy_width - attrib.x;
  height = attrib.height;
  /* 
  ** expand to max width 
  */
  while (apps_list)
    {
      if (apps_list->p_t_win != ewin)
	{
	  XGetWindowAttributes(gl_dpy, apps_list->p_t_win->parent, &app);
	  if (app.map_state == IsViewable)
	    {
	      if ((app.x > (attrib.x + attrib.width)) && (((attrib.y >= app.y) && (attrib.y <= (app.y + app.height))) || ((app.y >= attrib.y) && (app.y <= (attrib.y + attrib.height)))))
		if (app.x < (attrib.x + width))
		  width = (app.x - attrib.x);
	    }
	}
      apps_list = apps_list->next_elt;
    }
  window_group_resize(ewin, attrib.x, attrib.y, width - SPACE_WIN, height);
  apps_list = gl_epiwin.first_win_group_list;
  XGetWindowAttributes(gl_dpy, ewin->parent, &attrib);
  width = attrib.width;
  height = gl_scr_info.dpy_height - attrib.y;
  /* 
  ** expand to max height
  */
  while (apps_list)
    {
      if (apps_list->p_t_win != ewin)
	{
	  XGetWindowAttributes(gl_dpy, apps_list->p_t_win->parent, &app);
	  if (app.map_state == IsViewable)
	    {
	      if ((attrib.y <= app.y) && (((attrib.x >= app.x) && (attrib.x <= (app.x + app.width))) || ((app.x >= attrib.x) && (app.x <= (attrib.x + attrib.width)))))
		if ((app.y - attrib.y) < height)
		  height = app.y - attrib.y;
	    }
	}
      apps_list = apps_list->next_elt;
    }
  window_group_resize(ewin, attrib.x, attrib.y, width, height - SPACE_WIN);
}
