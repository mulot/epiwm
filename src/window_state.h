/*
 ** window_state.h
 ** 
 ** Made by (Poindessous Thomas)
 ** Login   <poinde_t@epita.fr>
 ** 
 ** Started on  Sat Mar 20 18:27:35 1999 Poindessous Thomas
 ** Last update Sun Jul 25 20:43:51 1999 Thomas Poindessous
 */

#ifndef   __WINDOW_STATE_H__
#define   __WINDOW_STATE_H__

void	clientSetState(t_win *, int);
void	send_configure_notify(t_win *);
void	client_message();
void	window_group_resize(t_win *, int, int, int, int);
void	window_group_map(t_win *);
void	clientsendprotocol(t_win *, Atom);
void	internal_group_resize_top(t_win *, int, int);
void	internal_group_resize_bottom(t_win *, int, int);
void	internal_group_resize_right(t_win *, int, int);
void	internal_group_resize_left(t_win *, int, int);
void	internal_replace_button(t_win *);
void	internal_replace_button_horizontal(t_win *);
void	internal_replace_button_vertical(t_win *);
void	internal_verify_size_horizontal(t_win *, int *, int *);
void	internal_verify_size_vertical(t_win *, int *, int *);
void	internal_resize_win_horizontal(t_win *, int, int);
void	internal_resize_win_vertical(t_win *, int, int);

#endif /* __WINDOW_STATE_H__ */
