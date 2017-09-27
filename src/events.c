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
** events.c
** 
** Made by (Julien Mulot)
** Login   <mulot_j@epita.fr>
** 
** Started on  Fri Jan  8 19:54:25 1999 Julien Mulot
 ** Last update Mon Oct 23 22:52:51 2000 Julien Mulot
*/


#include "epiwm.h"


void	event_type()
{
  /*
    if ((gl_event.type != 6) && (gl_event.type != 2) &&
    (gl_event.type != 7) && (gl_event.type != 8) && (gl_event.type != 12))
    printf("et hop, un event %d\n",gl_event.type);
  */
  if (gl_event.type < LASTEvent)
    if (tab[gl_event.type])
      (*tab[gl_event.type])();     
}


void		mouse_move()
{
  t_menu_pos	**menu;
  t_win		*ewin;
  
  ewin = search_elt(gl_event.xmotion.window);
  if (ewin)
    mouse_motion_notify(ewin);
  else     
    if (gl_event.xmotion.window != gl_scr_info.root_win)
      {
	if (gl_event.xmotion.window == gl_menu.menu_task)
	  menu_task_move(gl_event.xmotion.y);
	else if (gl_event.xmotion.window == gl_menu.menu_prop)
	  menu_prop_move(gl_event.xmotion.y);
	else if (gl_event.xmotion.window == gl_menu.workspace)
	  menu_workspace_move(gl_event.xmotion.y);
	else
	  {	       
	    menu = &menu_pos;
	    while ((*menu) != NULL)
	      {
		if ((*menu)->win == gl_event.xmotion.window)
		  break;
		menu = &((*menu)->next);
	      }
	    if ((*menu) != NULL)
	      {
		mouse_on_menu((*menu));
		if (gl_menu.auto_menu)
		  menu_move(*menu, gl_event.xmotion.y);
	      }
	  }
      }
    else
      mouse_on_border_notify();
}


void		refresh_win()
{
  Window	child;
  Window	root;
  int		root_x;
  int		root_y;
  int		win_x;
  int		win_y;
  unsigned int	status;
  t_menu_pos	**p_menu;
  t_win		*ewin;
  
  if (gl_event.xexpose.window != gl_scr_info.root_win)
    {
      if (gl_menu.menu_task == gl_event.xexpose.window)
	draw_menu_task();
      else if (gl_menu.menu_prop == gl_event.xexpose.window)
	draw_menu_prop(gl_menu.prop_type);
      else if (gl_menu.workspace == gl_event.xexpose.window)
	draw_menu_workspace();
      else
	{	     
	  p_menu = &menu_pos;
	  while ((*p_menu) != NULL)
	    {
	      if (((*p_menu)->win) == gl_event.xexpose.window)
		break;
	      p_menu = &((*p_menu)->next);
	    }  
	  if ((*p_menu) != NULL)
	    {
	      if ((*p_menu)->here)
		draw_menu((*p_menu)->pos, p_menu);
	    }
	  else
	    {
	      if (gl_event.xexpose.count == 0)
		{
		  ewin = search_elt(gl_event.xexpose.window);
		  if (ewin)
		    {
		      if (ewin->titlebar_win == gl_event.xexpose.window)
			{		       
			  if (ewin->active)
			    window_state(ewin, "active");
			  else
			    window_state(ewin, "inactive");
			}
		      else 
			{			    
			  XQueryPointer(gl_dpy, gl_scr_info.root_win, &root,
					&child, &root_x, &root_y, &win_x,
					&win_y, &status);
			  if ((ewin->iconify_title_win
			       == gl_event.xexpose.window)
			      && (gl_event.xexpose.window != child))
			    draw_icon(gl_event.xexpose.window, gl_icon.width,
				      "inactive");			    
			}
		    }
		}
	    }
	}
    }
}


/* handle X events */
void	handle_events()
{  
  tab_events();
  while (1)
    {
      /* 
      ** XFlush(gl_dpy);
      ** if(XPending(gl_dpy))
      ** { 
      */
      XNextEvent(gl_dpy, &gl_event);	
      event_type();
      /*
      ** }
      */
    }
}

