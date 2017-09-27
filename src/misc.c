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
** misc.c
**
** Made by (Julien Mulot)
** Login   <mulot_j@epita.fr>
**
** Started on  Fri Jan  8 21:16:10 1999 Julien Mulot
 ** Last update Mon Oct 23 23:41:13 2000 Julien Mulot
*/


#include "epiwm.h"


void		add_elt(t_win *ewin)
{
  t_win_list	*tmp_elt;
  
  if (gl_epiwin.align_btn_max == 0)
    ewin->button_max = 0;
  if (gl_epiwin.align_btn_iconify == 0)
    ewin->button_iconify = 0;
  if (gl_epiwin.align_btn_max_height == 0)
    ewin->button_max_height = 0;
  if (gl_epiwin.align_btn_max_width == 0)
    ewin->button_max_width = 0;
  if (gl_epiwin.align_btn_close == 0)
    ewin->button_close = 0;
  if (gl_epiwin.align_btn_shade == 0)
    ewin->button_shade = 0;
  ewin->iconify_title_win = 0;
  ewin->is_shade = 0;
  ewin->size = 0;
  if (test_shape(ewin->win))
    ewin->size |= SHAPE;
  ewin->win_width = 0;
  ewin->win_height = 0;
  ewin->shade_width = 0;
  ewin->shade_height = 0;  
  tmp_elt = (t_win_list *) xmalloc(sizeof (t_win_list));
  tmp_elt->p_t_win = ewin;
  tmp_elt->next_elt = NULL;
  if (gl_epiwin.first_win_group_list == NULL)
    {
      gl_epiwin.first_win_group_list = tmp_elt;
      gl_epiwin.nb_elt = 1;
    }
  else
    {
      tmp_elt->next_elt = gl_epiwin.first_win_group_list;
      gl_epiwin.first_win_group_list = tmp_elt;
      gl_epiwin.nb_elt++;
    }
}


t_win		*search_elt(Window w)
{
  t_win_list	*temp;
  
  if (w == 0)
    return (NULL);
  if (gl_epiwin.first_win_group_list)
    {
      temp = gl_epiwin.first_win_group_list;
      while (temp->next_elt && !(search_win(temp->p_t_win, w)))
	temp = temp->next_elt;
      if (search_win(temp->p_t_win, w))
	return (temp->p_t_win);
    }
  return (NULL);
}


void		remove_elt(Window w)
{
  t_win_list	*temp;
  t_win_list	*before;
  
  if (gl_epiwin.first_win_group_list)
    {
      temp = gl_epiwin.first_win_group_list;
      before = NULL;
      while (temp->next_elt && !(search_win(temp->p_t_win, w)))
	{
	  before = temp;
	  temp = temp->next_elt;
	}
      if (search_win(temp->p_t_win, w))
	{
	  if (before)
	    before->next_elt = temp->next_elt;
	  else
	    gl_epiwin.first_win_group_list = temp->next_elt;
	  free(temp->p_t_win);
	  free(temp);
	  gl_epiwin.nb_elt--;
	}
    }
}


int	search_win(t_win *ewin, Window win)
{
  return ((ewin->win == win) ||
	  (ewin->parent == win) ||
	  (ewin->border_frame == win) ||
	  (ewin->titlebar_win == win) ||
	  (ewin->button_close == win) ||
	  (ewin->button_max_height == win) ||
	  (ewin->button_max_width == win) ||
	  (ewin->button_max == win) ||
	  (ewin->button_shade == win) ||
	  (ewin->button_iconify == win) ||
	  (ewin->iconify_title_win == win) ||	  
	  (ewin->iconify_win == win));
}
