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
** epiwm.c
**
** Made by ()
** Login   <mulot_j@epita.fr>
**
** Started on  Mon Dec 28 03:16:49 1998
 ** Last update Mon Oct 23 22:49:57 2000 Julien Mulot
*/


#include "epiwm.h"


void	add_signals()
{
  signal(SIGINT, quit);
  signal(SIGQUIT, quit);
  signal(SIGKILL, quit);
  signal(SIGTERM, quit);
  /*   signal(SIGSEGV, quit);   */
}


/* Help of command line */
void	help()
{
  printf("EPIwm Version %s Usage:\n", VERSION);
  printf("\t-h or --help\tprint this help\n");
  printf("\t-v\t\tshow version\n");
  printf("\t-d display\tspecify the display\n");
  printf("\t-theme <theme path>\tspecify the theme path\n");
  exit(OK);
}


/* options of command line */
void	options(int nb_arg, char **opt)
{
  while (--nb_arg != 0)
    {
      if (((strncasecmp(opt[nb_arg], "-h", 2)) == 0)
	  || ((strncasecmp(opt[nb_arg], "--help", 6)) == 0))
	help();
      else if ((strncasecmp(opt[nb_arg], "-d", 2)) == 0)
	gl_display_name = opt[nb_arg + 1];
      else if ((strncmp(opt[nb_arg], "-theme", 6)) == 0)
	{
	  if (opt[nb_arg+1])
	    {
	      gl_theme_path = (char *) xmalloc(sizeof (char)
					       * (strlen(opt[nb_arg+1]) + 1));
	      strcpy(gl_theme_path, opt[nb_arg+1]);
	    }
	}
      else if ((strncasecmp(opt[nb_arg], "-v", 2)) == 0)
	{
	  printf("EPIwm Version %s\n", VERSION);
	  exit(OK);
	} 
      else if (((strncasecmp(opt[nb_arg - 1], "-d", 2)) != 0)
	       && ((strncasecmp(opt[nb_arg - 1], "-theme", 6)) != 0))
	{
	  printf("\nunrecognized option\n\n");
	  help();
	}
    }
}


int	main(int argc, char **argv)
{
  gl_argv = argv;
  gl_display_name = NULL;
  gl_theme_path = NULL;
  add_signals();
  options(argc, argv);
  init_display();
  init_misc();
  read_config("window");
  read_config("menu");
  read_config("icons");
  read_config("style");
  read_config("workspace");
  read_config("key");
  read_config("start");
  prepare_font();
  initx();
  handle_events();
  return (0);
}
