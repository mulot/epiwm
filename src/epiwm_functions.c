/*
** epiwm_functions.c
** 
** Made by (Julien Mulot)
** Login   <mulot_j@epita.fr>
** 
** Started on  Sun May  9 02:13:29 1999 Julien Mulot
 ** Last update Mon Oct 23 22:52:20 2000 Julien Mulot
*/


#include "epiwm.h"


void	init_tab_function()
{  
  func_tab[0].name = xmalloc(sizeof (char) * (strlen("Close") + 1));
  strcpy(func_tab[0].name, "Close");
  func_tab[0].func = func_close;
  func_tab[0].type = FUNC_WIN;
  func_tab[1].name = xmalloc(sizeof (char) * (strlen("Iconify") + 1));
  strcpy(func_tab[1].name, "Iconify");
  func_tab[1].func = func_iconify;
  func_tab[1].type = FUNC_WIN;
  func_tab[2].name = xmalloc(sizeof (char) * (strlen("Max") + 1));
  strcpy(func_tab[2].name, "Max");
  func_tab[2].func = func_max;
  func_tab[2].type = FUNC_WIN;
  func_tab[3].name = xmalloc(sizeof (char) * (strlen("MaxFit") + 1));
  strcpy(func_tab[3].name, "MaxFit");
  func_tab[3].func = func_max_fit;
  func_tab[3].type = FUNC_WIN;
  func_tab[4].name = xmalloc(sizeof (char) * (strlen("MaxHeight")
						      + 1));
  strcpy(func_tab[4].name, "MaxHeight");
  func_tab[4].func = func_max_height;
  func_tab[4].type = FUNC_WIN;
  func_tab[5].name = xmalloc(sizeof (char) * (strlen("MaxWidth") + 1));
  strcpy(func_tab[5].name, "MaxWidth");
  func_tab[5].func = func_max_width;
  func_tab[5].type = FUNC_WIN;
  func_tab[6].name = xmalloc(sizeof (char) * (strlen("Destroy") + 1));
  strcpy(func_tab[6].name, "Destroy");
  func_tab[6].func = func_destroy;
  func_tab[6].type = FUNC_WIN;
  func_tab[7].name = xmalloc(sizeof (char) * (strlen("Move") + 1));
  strcpy(func_tab[7].name, "Move");
  func_tab[7].func = func_move;
  func_tab[7].type = FUNC_WIN;
  func_tab[8].name = xmalloc(sizeof (char) * (strlen("Shade") + 1));
  strcpy(func_tab[8].name, "Shade");
  func_tab[8].func = func_shade;
  func_tab[8].type = FUNC_WIN;
  func_tab[9].name = xmalloc(sizeof (char) * (strlen("SendTo") + 1));
  strcpy(func_tab[9].name, "SendTo");
  func_tab[9].func = NULL;
  func_tab[9].type = FUNC_WIN;
  func_tab[10].name = xmalloc(sizeof (char)*(strlen("WindowSwitch")+1));
  strcpy(func_tab[10].name, "WindowSwitch");
  func_tab[10].func = win_switch;
  func_tab[10].type = FUNC_ROOT;
  func_tab[11].name = xmalloc(sizeof (char)*(strlen("GoPagerRight")
						     + 1));
  strcpy(func_tab[11].name, "GoPagerRight");
  func_tab[11].func = go_to_pager_right_keyb;
  func_tab[11].type = FUNC_ROOT;
  func_tab[12].name = xmalloc(sizeof (char)*(strlen("GoPagerLeft")+1));
  strcpy(func_tab[12].name, "GoPagerLeft");
  func_tab[12].func = go_to_pager_left_keyb;
  func_tab[12].type = FUNC_ROOT;
  func_tab[13].name = xmalloc(sizeof (char)
				      * (strlen("GoPagerStepRight") + 1));
  strcpy(func_tab[13].name, "GoPagerStepRight");
  func_tab[13].func = go_to_pager_step_right_keyb;
  func_tab[13].type = FUNC_ROOT;
  func_tab[14].name = xmalloc(sizeof (char)
				      * (strlen("GoPagerStepLeft") + 1));
  strcpy(func_tab[14].name, "GoPagerStepLeft");
  func_tab[14].func = go_to_pager_step_left_keyb;
  func_tab[14].type = FUNC_ROOT;
  func_tab[15].name = xmalloc(sizeof (char) * (strlen("GoTo") + 1));
  strcpy(func_tab[15].name, "GoTo");
  func_tab[15].func = NULL;
  func_tab[15].type = FUNC_ROOT;
  func_tab[16].name = xmalloc(sizeof (char) * (strlen("Restart") + 1));
  strcpy(func_tab[16].name, "Restart");
  func_tab[16].func = restart;
  func_tab[16].type = FUNC_ROOT;
  func_tab[17].name = xmalloc(sizeof (char) * (strlen("Exit") + 1));
  strcpy(func_tab[17].name, "Exit");
  func_tab[17].func = quit;
  func_tab[17].type = FUNC_ROOT;
}


void			replace_win()
{
  t_win_list		**tmp;
  XWindowAttributes	*attrib;
  
  tmp = &gl_epiwin.first_win_group_list;
  while (*tmp)
    {
      attrib = xmalloc(sizeof (XWindowAttributes));
      XGetWindowAttributes(gl_dpy, (*tmp)->p_t_win->parent, attrib);
      XReparentWindow(gl_dpy, (*tmp)->p_t_win->win, gl_scr_info.root_win,
		      attrib->x, attrib->y);
      XMoveWindow(gl_dpy, (*tmp)->p_t_win->win, attrib->x, attrib->y);
      XGetWindowAttributes(gl_dpy, (*tmp)->p_t_win->win, attrib);
      XRemoveFromSaveSet(gl_dpy, (*tmp)->p_t_win->win);
      internal_destroy((*tmp)->p_t_win);
      XFree(attrib);
    }
}


void	win_func_terminate()
{
  XDefineCursor(gl_dpy, gl_scr_info.root_win, gl_epiwin.normal_cursor);
  gl_epiwin.current_cursor = gl_epiwin.normal_cursor;
  gl_current_func = NULL;
}


void		win_switch()
{
  t_win_list	*win_list;
  
  win_list = gl_epiwin.first_win_group_list;
  if (win_list)
    {
      if (gl_epiwin.current)
	{
	  while (win_list->p_t_win != gl_epiwin.current)
	    win_list = win_list->next_elt;
	  win_list = win_list->next_elt;
	  if (!win_list)
	    win_list = gl_epiwin.first_win_group_list;
	}
      go_to_nth_workspace(window_workspace(win_list->p_t_win));
      XRaiseWindow(gl_dpy, win_list->p_t_win->parent);
      window_state(win_list->p_t_win, "active");
      XWarpPointer(gl_dpy, None, win_list->p_t_win->parent, 0, 0, 0, 0, 
		   gl_epiwin.border_size + 1, gl_epiwin.titlebar.height + 1);
    }
}


void	quit()
{
  go_to_nth_workspace(1);
  replace_win();
  XSetInputFocus(gl_dpy, PointerRoot, RevertToPointerRoot, CurrentTime);
  XCloseDisplay(gl_dpy);
  exit(OK);
}


void	restart()
{
  go_to_nth_workspace(1);
  replace_win();
  XSelectInput(gl_dpy, gl_scr_info.root_win, 0);
  XSync(gl_dpy, 0);
  XCloseDisplay(gl_dpy);
  execvp(gl_argv[0], gl_argv);
  exit(OK);
}


void	func_sendto()
{
  if (gl_menu.prop_type == FUNC_ROOT)
    {
      XDefineCursor(gl_dpy, gl_scr_info.root_win, gl_epiwin.target_cursor);
      gl_epiwin.current_cursor = gl_epiwin.target_cursor;
      gl_current_func = func_sendto;
    }
  else
    send_window_to_workspace(gl_epiwin.current, gl_workspace_to_send); 
}


void	func_close()
{
  if (gl_menu.prop_type == FUNC_ROOT)
    {
      XDefineCursor(gl_dpy, gl_scr_info.root_win, gl_epiwin.target_cursor);
      gl_epiwin.current_cursor = gl_epiwin.target_cursor;
      gl_current_func = func_close;
    }
  else
    win_close(gl_epiwin.current, 0, 0);
}


void	func_destroy()
{
  if (gl_menu.prop_type == FUNC_ROOT)
    {
      XDefineCursor(gl_dpy, gl_scr_info.root_win, gl_epiwin.target_cursor);
      gl_epiwin.current_cursor = gl_epiwin.target_cursor;
      gl_current_func = func_destroy;
    }
  else
    win_destroy(gl_epiwin.current, 0, 0);
}


void	func_iconify()
{
  if (gl_menu.prop_type == FUNC_ROOT)
    {
      XDefineCursor(gl_dpy, gl_scr_info.root_win, gl_epiwin.target_cursor);
      gl_epiwin.current_cursor = gl_epiwin.target_cursor;
      gl_current_func = func_iconify;
    }
  else
    win_iconify(gl_epiwin.current, 0, 0);
}


void	func_max_fit()
{
  if (gl_menu.prop_type == FUNC_ROOT)
    {
      XDefineCursor(gl_dpy, gl_scr_info.root_win, gl_epiwin.target_cursor);
      gl_epiwin.current_cursor = gl_epiwin.target_cursor;
      gl_current_func = func_max_fit;
    }
  else
    max_fit(gl_epiwin.current);
}


void			func_max()
{
  t_mouse_move_win	list;
  
  if (gl_menu.prop_type == FUNC_ROOT)
    {
      XDefineCursor(gl_dpy, gl_scr_info.root_win, gl_epiwin.target_cursor);
      gl_epiwin.current_cursor = gl_epiwin.target_cursor;
      gl_current_func = func_max;
    }
  else
    {
      gl_event.xbutton.y = 0;
      gl_event.xbutton.x = 0;
      list.button = &gl_event.xbutton;
      list.ewin = gl_epiwin.current;
      mouse_resize_max(&list);
    }
}


void	func_max_height()
{
  if (gl_menu.prop_type == FUNC_ROOT)
    {
      XDefineCursor(gl_dpy, gl_scr_info.root_win, gl_epiwin.target_cursor);
      gl_epiwin.current_cursor = gl_epiwin.target_cursor;
      gl_current_func = func_max_height;
    }
  else
    win_max_height(gl_epiwin.current);
}


void	func_max_width()
{
  if (gl_menu.prop_type == FUNC_ROOT)
    {
      XDefineCursor(gl_dpy, gl_scr_info.root_win, gl_epiwin.target_cursor);
      gl_epiwin.current_cursor = gl_epiwin.target_cursor;
      gl_current_func = func_max_width;
    }
  else
    win_max_width(gl_epiwin.current);
}


void	func_resize()
{
  if (gl_menu.prop_type == FUNC_ROOT)
    {
      XDefineCursor(gl_dpy, gl_scr_info.root_win, gl_epiwin.target_cursor);
      gl_epiwin.current_cursor = gl_epiwin.target_cursor;
      gl_current_func = func_resize;
    }
  else
    {
      /* utiliser win_resize dans window_functions.c */
    }
}


void	func_move()
{
  if (gl_menu.prop_type == FUNC_ROOT)
    {
      XDefineCursor(gl_dpy, gl_scr_info.root_win, gl_epiwin.target_cursor);
      gl_epiwin.current_cursor = gl_epiwin.target_cursor;
      gl_current_func = func_move;
    }
  else
    {
      /* utiliser win_move dans window_functions.c */
    }
}


void	func_shade()
{
  if (gl_menu.prop_type == FUNC_ROOT)
    {
      XDefineCursor(gl_dpy, gl_scr_info.root_win, gl_epiwin.target_cursor);
      gl_epiwin.current_cursor = gl_epiwin.target_cursor;
      gl_current_func = func_shade;
    }
  else
    win_shade(gl_epiwin.current, 0, 0);  
}
