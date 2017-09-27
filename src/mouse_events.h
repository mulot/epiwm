/*
** mouse_events.h
** 
** Made by (Poindessous Thomas)
** Login   <poinde_t@epita.fr>
** 
** Started on  Wed Feb 24 16:09:16 1999 Poindessous Thomas
 ** Last update Wed Apr  5 02:25:31 2000 Mulot Julien
*/

#ifndef   __MOUSE_EVENTS_H__
#define   __MOUSE_EVENTS_H__

#define	TIME_RELEASE	400
#define	TIME_TWO_CLICKS	500
#define TIME_CHANGE_WORKSPACE 0
#define FOCUS_SLOPPY 0
#define FOCUS_FOLLOW_MOUSE 1
#define FOCUS_CLICK 2


typedef struct	s_mouse_move_win
{
  XButtonEvent	*button;   
  t_win		*ewin;
}		t_mouse_move_win;

typedef struct	s_win_move
{
  Window	win;
  Window	bef;
  int		mouse_x;
  int		mouse_y;
  int		win_x;
  int		win_y;
  int		width;
  int		height;
  Time		last;
  Time		time_press;
}		t_win_move;

typedef struct	s_coord
{
  int		x;
  int		y;
}		t_coord;

typedef struct	s_bicoord
{
  int		x;
  int		y;
  int		win_x;
  int		win_y;
}		t_bicoord;

void		catch_win_mouse();
void		mouse_enter_win();
void		mouse_leave_win();
void		mouse_press();
void		mouse_release();
void		mouse_move_win(t_mouse_move_win *);
void		mouse_on_icon(Window);
void		mouse_out_icon(Window);
void		mouse_uniconify(t_win *);
void		mouse_iconify(t_win *);
void		mouse_shade(t_mouse_move_win *);
void		mouse_resize_max(t_mouse_move_win *);
void		mouse_resize_l_r_d(t_mouse_move_win *);
void		mouse_on_menu(t_menu_pos *);
void		mouse_on_border_notify();
t_coord		*window_on_border(t_win *, int, int);
t_bicoord	*set_cursor_position();
int		verify_mouse_vs_button(int, int);
void		internal_resize_max(t_win *);
void		internal_resize_unmax(t_win *);
void		internal_shade(t_win *);
void		internal_unshade(t_win *);
void		internal_max_width(t_win *);
void		internal_max_height(t_win *);
void		internal_unmax_width(t_win *);
void		internal_unmax_height(t_win *);

t_win_move	gl_win_move;
#endif /* __MOUSE_EVENTS_H__ */


