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
** init.h
** 
** Made by (Julien Mulot)
** Login   <mulot_j@epita.fr>
** 
** Started on  Wed Jan 27 20:26:28 1999 Julien Mulot
 ** Last update Tue Jan 25 22:38:28 2000 Julien Mulot
*/

#ifndef   __INIT_H__
#define   __INIT_H__
#include "screen.h"
#include <stdio.h>
#include <X11/Xlib.h>

#ifndef CONFIG_PATH
#define CONFIG_PATH	"/etc/X11/epiwm"
#endif

void		capture_windows();
char		*cut_str(char *, char *);
XErrorHandler	handler(Display *, XErrorEvent *);
char		*index_file(char *, int);
void		init_color(char *, XColor *);
void		init_config(FILE *, char *);
void		init_cursor();
void		init_display();
void		init_event();
void		init_font_color(GC *, char *);
void		init_gc();
void		init_image(char *, char **);
void		init_misc();
void		initx();
void		prepare_font();
void		init_atoms();
void		read_config(char *);

char		*gl_display_name;
char		*gl_theme_path;
int		gl_parent_pid;
Display		*gl_dpy;
GC		gc_black;
GC		gc_xor;
XEvent		gl_event;
#ifdef	  IMLIB
ImlibData		*gl_imlib_data;
#endif

#endif /* __INIT_H__ */






