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
** init.c
**
** Made by ()
** Login   <mulot_j@epita.fr>
**
** Started on  Thu Dec 31 03:18:14 1998
 ** Last update Sat Oct 28 21:03:56 2000 Julien Mulot
*/


#include "epiwm.h"


void		init_misc()
{
  XIconSize	icon_size;
  
  init_tab_function();  
  gl_scr_info.screen = DefaultScreen(gl_dpy);
  gl_scr_info.root_win = RootWindow(gl_dpy, gl_scr_info.screen);
  gl_scr_info.dpy_width = DisplayWidth(gl_dpy, gl_scr_info.screen);
  gl_scr_info.dpy_height = DisplayHeight(gl_dpy, gl_scr_info.screen);
  gl_scr_info.depth = DefaultDepth(gl_dpy, gl_scr_info.screen);
  gl_menu.grad_type = 0;
  gl_menu.here_prop = 0;
  gl_menu.here_task = 0;
  gl_menu.workspace_type = NONE;
  gl_menu.font_inactive = XCreateGC(gl_dpy, gl_scr_info.root_win, 0, NULL);
  gl_menu.font_active = XCreateGC(gl_dpy, gl_scr_info.root_win, 0, NULL);
  gl_menu.width = MENU_MINI_WIDTH;
  gl_menu.workspace_width = MENU_MINI_WIDTH;
  gl_menu_apps = NULL;
  gl_menu.lngst_str = NULL;
  gl_menu.current_elt = NULL;
  gl_menu.current_menu = NULL;
  gl_menu.current_task = NULL;
  gl_menu.current_prop = NULL;
  gl_menu.current_workspace = NULL;
  gl_menu.auto_menu = 0;
  gl_menu_task = NULL;
  gl_win_move.win = 0;
  gl_win_move.bef = 0;
  gl_win_move.last = 0;
  gl_icon.showed = 1;
  gl_icon.font_active =
    XCreateGC(gl_dpy, gl_scr_info.root_win, 0, NULL);
  gl_icon.font_inactive =
    XCreateGC(gl_dpy, gl_scr_info.root_win, 0, NULL);
  gl_epiwin.titlebar.font_active =
    XCreateGC(gl_dpy, gl_scr_info.root_win, 0, NULL);
  gl_epiwin.titlebar.font_inactive =
    XCreateGC(gl_dpy, gl_scr_info.root_win, 0, NULL);
  gl_epiwin.titlebar.grad_active_type = 0;
  gl_epiwin.titlebar.grad_inactive_type = 0;
  gl_epiwin.titlebar.position = TOP;
  gl_epiwin.titlebar.align = LEFT;
  gl_epiwin.titlebar.align_shape = LEFT;
  gl_epiwin.titlebar.width_mode = NONE;
  gl_epiwin.titlebar.width = 0;
#ifdef	IMLIB
  gl_menu.image_name = NULL;
  gl_icon.image_active = NULL;
  gl_icon.image_inactive = NULL;
  gl_epiwin.titlebar.image_active = NULL;
  gl_epiwin.titlebar.image_inactive = NULL;
  gl_epiwin.titlebar.img_btn_close_active = NULL;
  gl_epiwin.titlebar.img_btn_destroy_active = NULL;
  gl_epiwin.titlebar.img_btn_max_active = NULL;
  gl_epiwin.titlebar.img_btn_shade_active = NULL;
  gl_epiwin.titlebar.img_btn_iconify_active = NULL;
  gl_epiwin.titlebar.img_btn_close_inactive = NULL;
  gl_epiwin.titlebar.img_btn_destroy_inactive = NULL;
  gl_epiwin.titlebar.img_btn_max_inactive = NULL;
  gl_epiwin.titlebar.img_btn_shade_inactive = NULL;
  gl_epiwin.titlebar.img_btn_iconify_inactive = NULL;
  gl_epiwin.titlebar.image_max = 0;
#endif
  gl_epiwin.focus_mode = FOCUS_SLOPPY;
  gl_epiwin.nb_elt = 0;
  gl_epiwin.btn_width = 12;
  gl_epiwin.btn_height = 12;
  gl_epiwin.prop = 0;
  gl_epiwin.first_win_group_list = NULL;
  gl_epiwin.current = NULL;
  gl_epiwin.left_win = NULL;
  gl_epiwin.align_btn_close = NONE;
  gl_epiwin.align_btn_destroy = NONE;
  gl_epiwin.align_btn_iconify = NONE;
  gl_epiwin.align_btn_max = NONE;
  gl_epiwin.align_btn_max_height = NONE;
  gl_epiwin.align_btn_max_width = NONE;
  gl_epiwin.align_btn_shade = NONE;  
  gl_epiwin.virtual_width = 0;
  gl_epiwin.virtual_x = 0;
  gl_epiwin.cursor_position_x = -1;
  gl_epiwin.cursor_position_y = -1;
  gl_workspace_change_percent = 0;
  gl_workspace_cycle = 0;
  menu_pos = NULL;
  gl_apps_prop = NULL;
  gc_black = XCreateGC(gl_dpy, gl_scr_info.root_win, 0, NULL);
  XSetForeground(gl_dpy, gc_black, BlackPixel(gl_dpy,
					      gl_scr_info.screen));
  gl_epiwin.opaque_mvt = 0;
  gl_current_func = NULL;
  icon_size.min_width = 1;
  icon_size.min_height = 1;
  icon_size.max_width = 1152;
  icon_size.max_height = 864;
  icon_size.width_inc = 1;
  icon_size.height_inc = 1;
  XSetIconSizes(gl_dpy, gl_scr_info.root_win, &icon_size, 6);
  gl_icon.position = BOTTOM_LEFT;
  gl_key_list = NULL;
}


/* Capture X-Window errors */
XErrorHandler	handler(Display *gl_dpy, XErrorEvent *err_event)
{
  if ((err_event->request_code == X_ChangeWindowAttributes) &&
      (err_event->error_code == BadAccess))
    {
      fprintf(stderr, "Another Window Manager is running...\n");
      exit(ERROR);
    }
  else
    {
      /*      fprintf(stderr, "Yeesss, an ERROR : %d %d \n",
	      err_event->error_code,
	      err_event->request_code);*/
      return (0);
    }
}


/* just open the display ;) */
void	init_display()
{
  if ((gl_dpy = XOpenDisplay(gl_display_name)) == NULL)
    {
      fprintf(stderr, "\nEPIwm can not open display %s\n\n",
	      XDisplayName(gl_display_name));
      help();
      exit(ERROR);
    }
#ifdef	  IMLIB
  gl_imlib_data = Imlib_init(gl_dpy);
#endif
  XSetErrorHandler((XErrorHandler) handler);
}


/*
  void		set_font(font, val_font, color)
  XFontStruct	**font;
  char		*val_font;
  GC		*color;
  {
  *font = XLoadQueryFont(gl_dpy, val_font);
  XSetFont(gl_dpy, *color, (*font)->fid);
  }
*/


#ifdef	  IMLIB
void	init_image(char *image, char **image_dest)
{
  if (!strncmp(image, "~/", 2))
    {
      *image_dest = 
	(char *) xmalloc(sizeof (char) * (strlen(image)
					  + strlen(getenv("HOME"))));
      strcpy(*image_dest, getenv("HOME"));
      strcat(*image_dest, cut_str(image, "~"));
    }
  else
    {
      *image_dest = (char *) xmalloc(sizeof (char) * (strlen(image) + 1));
      strcpy(*image_dest, image);
    }
}
#endif


/* Initialization of the colors of fonts */
void		init_font_color(GC *font, char *color)
{
  XColor	exact_color;
  XColor	tmp_color;
  
  XAllocNamedColor(gl_dpy,
		   XDefaultColormap(gl_dpy, gl_scr_info.screen),
		   color, &tmp_color, &exact_color);
  XSetForeground(gl_dpy, *font, tmp_color.pixel);
}


/* Initialization of the colors of windows */
void		init_color(char *color, XColor *color_dest)
{
  XColor	exact_color;
  
  XAllocNamedColor(gl_dpy, XDefaultColormap(gl_dpy, gl_scr_info.screen),
		   color, color_dest, &exact_color);
}


/* Select cursors for misc events */
void	init_cursor()
{
  gl_epiwin.normal_cursor = XCreateFontCursor(gl_dpy, XC_left_ptr);
  gl_epiwin.resize_W_cursor = XCreateFontCursor(gl_dpy, XC_left_side);
  gl_epiwin.resize_E_cursor = XCreateFontCursor(gl_dpy, XC_right_side);
  gl_epiwin.resize_N_cursor = XCreateFontCursor(gl_dpy, XC_top_side);
  gl_epiwin.resize_S_cursor = XCreateFontCursor(gl_dpy, XC_bottom_side);
  gl_epiwin.resize_NW_cursor = XCreateFontCursor(gl_dpy, XC_top_left_corner);
  gl_epiwin.resize_NE_cursor = XCreateFontCursor(gl_dpy, XC_top_right_corner);
  gl_epiwin.resize_SW_cursor = XCreateFontCursor(gl_dpy,
						 XC_bottom_left_corner);
  gl_epiwin.resize_SE_cursor = XCreateFontCursor(gl_dpy,
						 XC_bottom_right_corner);
  gl_epiwin.target_cursor = XCreateFontCursor(gl_dpy, XC_target);
  gl_epiwin.move_cursor = XCreateFontCursor(gl_dpy, XC_fleur);
  XDefineCursor(gl_dpy, gl_scr_info.root_win, gl_epiwin.normal_cursor);
  gl_epiwin.current_cursor = gl_epiwin.normal_cursor;
}


/* Select types of evenements to exam */
void	init_event()
{
  XSelectInput(gl_dpy, gl_scr_info.root_win,
	       ButtonPressMask | PointerMotionMask |
	       SubstructureNotifyMask | ExposureMask | KeyPressMask
	       | SubstructureRedirectMask);
}


/* capture all windows that already exist */
void		capture_windows()
{
  Window	root;
  Window	parent;
  Window	*children;
  unsigned int	nb_children;
  int		i;
  
  XQueryTree(gl_dpy, RootWindow(gl_dpy, gl_scr_info.screen),
	     &root, &parent, &children, &nb_children);
  for (i = 0; i < nb_children; i++)
    handle_window(children[i]);
  free(children);
}


void		init_gc()
{
  XGCValues	gcv;
  unsigned long	gcm;
  
  gcm = GCFunction | GCPlaneMask | GCGraphicsExposures | GCLineWidth;
  gcv.line_width = 0;
  gcv.function = GXcopy;
  gcv.plane_mask = AllPlanes;
  gcv.graphics_exposures = False;
  gl_epiwin.transmask_gc = XCreateGC(gl_dpy, gl_scr_info.root_win, gcm, &gcv);
  XSetClipMask(gl_dpy, gl_epiwin.transmask_gc, None);
  gc_xor = XCreateGC(gl_dpy, gl_scr_info.root_win, 0, NULL);
  XSetSubwindowMode(gl_dpy, gc_xor, IncludeInferiors);
  XSetForeground(gl_dpy, gc_xor, BlackPixel(gl_dpy, gl_scr_info.screen)
                 ^ WhitePixel(gl_dpy, gl_scr_info.screen));
  XSetFunction(gl_dpy, gc_xor, GXxor);
}


/* initialisation of X */
void	initx()
{
  init_gc();
  init_cursor();
  init_event();
  init_atoms();
#ifdef	__GNOME__
  init_gnome();
#endif
#ifdef	IMLIB
  set_titlebar_btn_size();
#endif
  gl_epiwin.titlebar.space = set_title_position();
  gl_epiwin.prop |= FIRST_START;
  capture_windows();
  gl_epiwin.prop &= ~FIRST_START;
  gl_parent_pid = getpid();
}


char           *cut_str(char *str, char *word)
{
  while (*str == *word)
    {
      word++;
      str++;
    }
  return (str);
}


char		*index_file(char *line, int word_number)
{
  char		*tokens[50];
  static char	tmpstring[LINE_LEN];
  int		i;
  
  i = 0;
  strncpy(tmpstring, line, LINE_LEN);
  tokens[i++] = strtok(tmpstring, " ");
  while ((tokens[i - 1] != NULL) && i < 50)
	  tokens[i++] = strtok(NULL, " ");
  if (tokens[i - 1] != NULL)
	  tokens[i - 1] = NULL;
  return (tokens[word_number]);
}


void	init_config(FILE *file, char *part)
{
  char	line[LINE_LEN];
  
  if (!strcmp(part, "window"))
    window_build(file);
  else if (!strcmp(part, "workspace"))
    workspace_build(file);
  else if (!strcmp(part, "style"))
    style_build(file);
  else if (!strcmp(part, "start"))
    {
      while (fgets(line, LINE_LEN, file) != NULL)
	{
	  if (strlen(line) != 1)
	    line[strlen(line) - 1] = ' ';
	  if ((!strcasecmp(index_file(line, 0), "Init")) && 1)
	    system(cut_str(line, "Init "));
	  if ((!strcasecmp(index_file(line, 0), "Restart")) && 0)
	    system(cut_str(line, "Restart "));
	}
    }
  else if (!strcmp(part, "menu"))
    menu_build(file);
  else if (!strcmp(part, "icons"))
    icon_build(file);
  else if (!strcmp(part, "key"))
    key_build(file);
  fclose(file);
}


void	read_config(char *file)
{
  char	path[100];
  char	home[100];
  FILE	*config_file;
  
  strncpy(home, getenv("HOME"), 100);
  if (gl_theme_path)
    sprintf(path, "%s/%s", gl_theme_path, file);
  else
    sprintf(path, "%s/.epiwm/%s", home, file);
  if ((config_file = fopen(path, "r")) == NULL)
    {
      sprintf(path, CONFIG_PATH "/%s", file);
      if ((config_file = fopen(path, "r")) == NULL)
	{
	  fprintf(stderr,
		  "Can not open %s/.epiwm/%s or " CONFIG_PATH "/%s \n"
		  ,home, file, file);
	  exit(ERROR);
	}
    }
  init_config(config_file, file);
}


void	init_atoms()
{
  gl_epiwin.context = XUniqueContext();
  EPIWM_WM_STATE = XInternAtom(gl_dpy, "WM_STATE", False);
  EPIWM_WM_CHANGE_STATE = XInternAtom(gl_dpy, "WM_CHANGE_STATE", False);
  EPIWM_WM_PROTOCOLS = XInternAtom(gl_dpy, "WM_PROTOCOLS", False);
  EPIWM_WM_TAKE_FOCUS = XInternAtom(gl_dpy, "WM_TAKE_FOCUS", False);
  EPIWM_WM_DELETE_WINDOW = XInternAtom(gl_dpy, "WM_DELETE_WINDOW", False);
  EPIWM_WM_SAVE_YOURSELF = XInternAtom(gl_dpy, "WM_SAVE_YOURSELF", False);    
  EPIWM_WM_NAME = XInternAtom(gl_dpy, "WM_NAME", False);
  EPIWM_WM_ICON_NAME = XInternAtom(gl_dpy, "WM_ICON_NAME", False);
  EPIWM_MOTIF_WM_HINTS = XInternAtom(gl_dpy, "_MOTIF_WM_HINTS", False);
}


void		prepare_font()
{
  int		i;
  int		x;
  int		y;
  int		tall;
  GC		temp_gc;
  Pixmap	temp_pixmap;
  XImage	*temp_image;
  char		ff[1];
  
  ff[0] = 0;
  temp_pixmap = XCreatePixmap(gl_dpy, DefaultRootWindow(gl_dpy), 200, 200, 1);
  temp_gc = XCreateGC(gl_dpy, temp_pixmap, 0, NULL);
  XSetFont(gl_dpy, temp_gc, gl_epiwin.titlebar.font->fid);
  for (i = 0; i < 96; i++)
    {
      ff[0] = i + 32;
      gl_fn[i].count = 0;
      gl_fn[i].width = XTextWidth(gl_epiwin.titlebar.font, ff, 1);
      tall = gl_epiwin.titlebar.font->ascent
	+ gl_epiwin.titlebar.font->descent;
      XSetForeground(gl_dpy, temp_gc, 0);
      XFillRectangle(gl_dpy, temp_pixmap, temp_gc, 0, 0, 200, 200);
      XSetForeground(gl_dpy, temp_gc, 1);
      XDrawString(gl_dpy, temp_pixmap, temp_gc, 0,
		  tall - gl_epiwin.titlebar.font->descent, ff, 1);
      temp_image = XGetImage(gl_dpy, temp_pixmap, 0, 0, gl_fn[i].width,
			     tall, 1, XYPixmap);
      for (x = 0; x < gl_fn[i].width; x++)
	{
	  for (y = 0; y < tall; y++)
	    {
	      if (XGetPixel(temp_image, x, y) && gl_fn[i].count < 200)
		{
		  if (gl_epiwin.titlebar.position == RIGHT)
		    {
		      gl_fn[i].points[gl_fn[i].count].x = tall - y;
		      gl_fn[i].points[gl_fn[i].count].y = x;
		    }
		  else
		    {
		      gl_fn[i].points[gl_fn[i].count].x = y;
		      gl_fn[i].points[gl_fn[i].count].y = gl_fn[i].width - x;
		    }
		  gl_fn[i].count++;
		}			
	    }
	}
      XFree(temp_image);
    }
  XFree(temp_gc);
  XFreePixmap(gl_dpy, temp_pixmap);
}
