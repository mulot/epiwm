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
** motif.c
** 
** Made by (Poindessous Thomas)
** Login   <poinde_t@epita.fr>
** 
** Started on  Sun May 16 15:54:22 1999 Poindessous Thomas
 ** Last update Mon Oct 23 23:42:22 2000 Julien Mulot
*/


#include "epiwm.h"

#define TITLEBAR_BORDER 1
#define BF_BORDER 1


void			motif_get_wm_hints(Window		win,
					   t_motif_wm_hints	*mwmhints)
{   
  unsigned long		*data;
  unsigned long		count;
  Atom			type_ret;
  int			fmt_ret;
  unsigned long		bytes_after_ret;
  
  XGetWindowProperty(gl_dpy, win, EPIWM_MOTIF_WM_HINTS, 0, 32, False,
		     EPIWM_MOTIF_WM_HINTS, &type_ret, &fmt_ret,
		     &count, &bytes_after_ret,
		     (unsigned char **)  &data);
  if (!data)
    return ;
  if (count >= 4)
    {
      mwmhints->flags = data[0];
      mwmhints->decorations = data[2];
    }
  XFree(data);
}


void	motif_check(t_win *ewin)
{
  t_motif_wm_hints	mwmhints;
  
  mwmhints.flags = 0;
  motif_get_wm_hints(ewin->win, &mwmhints);
  if ((ewin->prop & NO_BORDER) == 0)
    {
      ewin->border_size = gl_epiwin.border_size;
      ewin->bf_border = BF_BORDER;
    }
  if ((ewin->prop & NO_TITLE) == 0)
    {
      ewin->titlebar_height = gl_epiwin.titlebar.height;
      ewin->titlebar_border = TITLEBAR_BORDER;
    }
  if (mwmhints.flags & MWM_HINTS_DECORATIONS)
    {
      if (mwmhints.decorations == 0)
	{
	  ewin->prop |= (NO_TITLE | NO_BORDER);
	  ewin->border_size = 0;
	  ewin->titlebar_height = 0;
	  ewin->titlebar_border = 0;
	  ewin->bf_border = 0;
	}
      else
	{
	  if ((mwmhints.decorations & MWM_DECOR_BORDER) == 0)
	    {
	      ewin->prop |= NO_BORDER;
	      ewin->border_size = 0;
	      ewin->bf_border = 0;
	    }
	  if ((mwmhints.decorations & MWM_DECOR_TITLE) == 0)
	    {
	      ewin->prop |= NO_TITLE;
	      ewin->titlebar_height = 0;
	      ewin->titlebar_border = 0;
	    }
	}	
    }  
}
