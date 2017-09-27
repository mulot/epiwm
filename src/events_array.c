/*
** tab_events.c
**
** Made by (Poindessous Thomas)
** Login   <poinde_t@epita.fr>
**
** Started on  Sat Jan 30 18:58:34 1999 Poindessous Thomas
 ** Last update Mon Oct 23 22:53:03 2000 Julien Mulot
*/


#include "epiwm.h"
#include "events.h"


void	tab_events()
{
  int	i;
  
  for (i = 0; i < 36; i++)
    tab[i] = NULL;
  tab[DestroyNotify] = dest_win;
  tab[UnmapNotify] = unmap_win;
  tab[MapNotify] = map_win;
  tab[MapRequest] = map_req;
  tab[ResizeRequest] = resize_win;
  tab[ButtonPress] = mouse_press;
  tab[KeyPress] = handle_keyboard;
  tab[MotionNotify] = mouse_move;
  tab[ButtonRelease] = mouse_release;
  tab[ConfigureRequest] = configure_win;
  tab[EnterNotify] = mouse_enter_win;
  tab[LeaveNotify] = mouse_leave_win;
  tab[Expose] = refresh_win;
  tab[PropertyNotify] = prop_not;
  tab[ClientMessage] = client_message;
}
