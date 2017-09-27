/*
** styles.c
**
** Made by (Julien Mulot)
** Login   <mulot_j@epita.fr>
**
** Started on  Sun Mar 14 03:42:33 1999 Julien Mulot
 ** Last update Mon Oct 23 22:53:42 2000 Julien Mulot
*/


#include "epiwm.h"


void	icon_build(FILE *file)
{
  char	tmp[LINE_LEN];
  char	*tmp2;
  
  while (fgets(tmp, LINE_LEN, file) != NULL)
    {
      if ((*tmp != '#') && (*tmp != '\t') && (*tmp != '\n') && (*tmp != ' '))
	{
	  if (strlen(tmp) != 1)
	    tmp[strlen(tmp) - 1] = ' ';
	  tmp2 = index_file(tmp, 0);
	  if (!strcasecmp(tmp2, "IconWidth"))
	    gl_icon.width = atoi(index_file(tmp, 1));
	  else if (!strcasecmp(tmp2, "IconHeight"))
	    gl_icon.height = atoi(index_file(tmp, 1));
	  else if (!strcasecmp(tmp2, "IconActiveColor"))
	    init_color(index_file(tmp, 1), &gl_icon.color_active);
	  else if (!strcasecmp(tmp2, "IconInactiveColor"))
	    init_color(index_file(tmp, 1), &gl_icon.color_inactive);
	  else if (!strcasecmp(tmp2, "IconFontInactiveColor"))
	    init_font_color(&gl_icon.font_inactive, index_file(tmp, 1));
	  else if (!strcasecmp(tmp2, "IconFontActiveColor"))
	    init_font_color(&gl_icon.font_active, index_file(tmp, 1));
	  else if (!strcasecmp(tmp2, "NoIcon"))
	    gl_icon.showed = 0;
#ifdef	IMLIB
	  else if (!strcasecmp(tmp2, "IconActiveImage"))
	    init_image(index_file(tmp, 1), &gl_icon.image_active);
	  else if (!strcasecmp(tmp2, "IconInactiveImage"))
	    init_image(index_file(tmp, 1), &gl_icon.image_inactive);
#endif
	  else if (!strcasecmp(tmp2, "IconFont"))
	    {
	      gl_icon.font = XLoadQueryFont(gl_dpy, index_file(tmp, 1));
	      XSetFont(gl_dpy, gl_icon.font_active, gl_icon.font->fid);
	      XSetFont(gl_dpy, gl_icon.font_inactive, gl_icon.font->fid);
	    }
	}
    }
}
