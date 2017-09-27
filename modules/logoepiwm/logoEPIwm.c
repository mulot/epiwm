/* This is logoEPIwm */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <X11/extensions/shape.h>
#include <X11/cursorfont.h>
#ifdef HAVE_CONFIG_H
#include "../../config.h"
#endif
#ifndef NOXPM
#include <X11/xpm.h>
#endif
#include "logoEPIwm.xbm"
#include "logoEPIwm_mask.xbm"
#include "logoEPIwm_icon.xbm"
#ifndef NOXPM
#include "logoEPIwm.xpm"
#endif

Display		*gl_display;
int		gl_screen_num;

int	main(argc, argv)
int	argc;
char	**argv;
{
  Window		win;
  GC			gc;
  XSetWindowAttributes	setwinattr;
  XGCValues		gcvalues;
  unsigned long		valuemask, gcvaluemask, inputmask;
  unsigned int		width, height;
  int			x, y, geomflags;
  unsigned int		border_width = 0;
  unsigned int		display_width, display_height, display_depth;
  char			*window_name = "logoEPIwm";
  char			*icon_name = "logoEPIwm";
  Pixmap		icon_pixmap, background_pixmap, shape_pixmap;
  XSizeHints		size_hints;
  XIconSize		*size_list;
  XWMHints		wm_hints;
  XClassHint		class_hints;
  XTextProperty		windowName, iconName;
  int			count, argnum;
  int			use_wm, float_up, use_mono;
  XEvent		report;
  char			*display_name = NULL;
  char			buffer[20];
  int			bufsize = 20;
  KeySym		keysym;
  XComposeStatus	compose;
  int			charcount;
  Cursor		cursor;
  time_t		time1;
  time_t		time2;
  int			eternal;  
#ifndef NOXPM
  Visual		*default_visual;
  Colormap		default_cmap;
  XpmAttributes		xpmattributes;
  XVisualInfo		*visual_info, vinfo_template;
  int			nmatches;
#endif
  XWindowChanges	winchanges;
  Window		root, child, basewin;
  int			offs_x, offs_y, new_x, new_y, tmp_x, tmp_y;
  unsigned int		tmp_mask;
  
  use_wm = False;
  float_up = False;
  use_mono = False;
  x = y = 0;
  geomflags = 0;
  eternal = 0;
  for(argnum=1; argnum<argc; argnum++)
    {
      if (!strcmp(argv[argnum],"-w"))
	use_wm = True;
      if (!strcmp(argv[argnum],"-f"))
	float_up = True;
      if (!strcmp(argv[argnum],"-mono"))
	use_mono = True;
      if (!strcmp(argv[argnum],"-e"))
	eternal = 1;
      if (!strcmp(argv[argnum],"-geometry"))
	geomflags = XParseGeometry(argv[++argnum], &x, &y, &width, &height);
    }
  if ( (gl_display = XOpenDisplay(display_name)) == NULL )
    {
      (void) fprintf(stderr, "%s: Cannot connect to X server %s\n",
		     argv[0], XDisplayName(display_name));
      exit(-1);
    }
  gl_screen_num = DefaultScreen(gl_display);
  display_width = DisplayWidth(gl_display, gl_screen_num);
  display_height = DisplayHeight(gl_display, gl_screen_num);
  display_depth = DefaultDepth(gl_display, gl_screen_num);
  width = logoEPIwm_width;
  height = logoEPIwm_height;
  if (!geomflags)
    {
      x = (display_width - width) / 2; 
      y = (display_height - height) / 2;
    }
  if (geomflags & XNegative)
    x = display_width - width + x;
  if (geomflags & YNegative)
    y = display_height - height + y;
  if (x < 0) 
    x = 0;
  if (x > display_width - width) 
    x = display_width - width;
  if (y < 0) 
    y = 0;
  if (y > display_height - height) 
    y = display_height - height;
  win = XCreateSimpleWindow(gl_display, RootWindow(gl_display, gl_screen_num),
			    x, y, width, height, border_width,
			    BlackPixel(gl_display, gl_screen_num),
			    WhitePixel(gl_display, gl_screen_num));
  basewin = win;
  gcvalues.foreground = BlackPixel(gl_display, gl_screen_num);
  gcvalues.background = WhitePixel(gl_display, gl_screen_num);
  gcvaluemask = GCForeground | GCBackground;
  gc = XCreateGC(gl_display, win, gcvaluemask, &gcvalues);
#ifdef NOXPM
  background_pixmap =
    XCreatePixmapFromBitmapData(gl_display, win, logoEPIwm_bits,
				logoEPIwm_width, logoEPIwm_height,
				BlackPixel(gl_display, gl_screen_num),
				WhitePixel(gl_display, gl_screen_num),
				display_depth);
#else
  default_visual = DefaultVisual(gl_display, gl_screen_num);
  default_cmap = DefaultColormap(gl_display, gl_screen_num);
  vinfo_template.visualid = XVisualIDFromVisual(default_visual);
  visual_info = XGetVisualInfo(gl_display, VisualIDMask,
			       &vinfo_template, &nmatches);
  if ((visual_info->class == StaticGray) || (use_mono))
    {
      background_pixmap =
	XCreatePixmapFromBitmapData(gl_display, win, logoEPIwm_bits,
				    logoEPIwm_width, logoEPIwm_height,
				    BlackPixel(gl_display, gl_screen_num),
				    WhitePixel(gl_display, gl_screen_num),
				    display_depth);
    }
  else
    {
      xpmattributes.visual = default_visual;
      xpmattributes.colormap = default_cmap;
      xpmattributes.depth = display_depth;
      xpmattributes.valuemask = XpmVisual | XpmColormap | XpmDepth;
      if (XpmCreatePixmapFromData(gl_display, win, logoEPIwm,
				  &background_pixmap, &shape_pixmap,
				  &xpmattributes) < XpmSuccess)
	{
	  printf("%s: Failed to allocate colormap. Using black-and-white.\n",
		 argv[0]);
	  background_pixmap =
	    XCreatePixmapFromBitmapData(gl_display, win, logoEPIwm_bits,
					logoEPIwm_width, logoEPIwm_height,
					BlackPixel(gl_display, gl_screen_num),
					WhitePixel(gl_display, gl_screen_num),
					display_depth);
	}
    }
  XFree(visual_info);
#endif
  setwinattr.background_pixmap = background_pixmap;
  if (use_wm)
    setwinattr.override_redirect = False;
  else
    setwinattr.override_redirect = True;
  cursor = XCreateFontCursor(gl_display, XC_fleur);
  setwinattr.cursor = cursor;
  valuemask = CWBackPixmap | CWOverrideRedirect | CWCursor;
  XChangeWindowAttributes(gl_display, win, valuemask, &setwinattr);
  shape_pixmap = XCreateBitmapFromData(gl_display,win, logoEPIwm_mask_bits,
				       logoEPIwm_mask_width, logoEPIwm_mask_height);
  XShapeCombineMask(gl_display, win, ShapeBounding, 0, 0, shape_pixmap, ShapeSet);
  if (XGetIconSizes(gl_display, RootWindow(gl_display,gl_screen_num),
		    &size_list, &count) == 0)
    {
      icon_pixmap = XCreateBitmapFromData(gl_display, win, logoEPIwm_icon_bits, logoEPIwm_icon_width, logoEPIwm_icon_height);
      /*      icon_pixmap = XCreatePixmapFromBitmapData(gl_display, win, logoEPIwm_icon_bits, logoEPIwm_icon_width, logoEPIwm_icon_height, BlackPixel(gl_display, gl_screen_num), WhitePixel(gl_display, gl_screen_num), display_depth);*/
    }
  else
    {
      icon_pixmap = XCreateBitmapFromData(gl_display, win, logoEPIwm_icon_bits, logoEPIwm_icon_width, logoEPIwm_icon_height);
      /*      icon_pixmap = XCreatePixmapFromBitmapData(gl_display, win, logoEPIwm_icon_bits, logoEPIwm_icon_width, logoEPIwm_icon_height, BlackPixel(gl_display, gl_screen_num), WhitePixel(gl_display, gl_screen_num), display_depth); */
    }
  size_hints.min_width = width;   
  size_hints.min_height = height;
  size_hints.max_width = width;
  size_hints.max_height = height;
  size_hints.flags = PPosition | PSize | PMinSize | PMaxSize;
  if (XStringListToTextProperty(&window_name, 1, &windowName) == 0)
    {
      (void) fprintf(stderr,
		     "%s: structure allocation for windowName failed.\n",
		     argv[0]);
      exit(-1);
    }
  if (XStringListToTextProperty(&icon_name, 1, &iconName) == 0)
    {
      (void) fprintf(stderr,
		     "%s: structure allocation for iconName failed.\n",
		     argv[0]);
      exit(-1);
    }
  wm_hints.initial_state = NormalState;
  wm_hints.input = True;
  wm_hints.icon_pixmap = icon_pixmap;
  wm_hints.flags = StateHint | IconPixmapHint | InputHint;
  class_hints.res_name = argv[0];
  class_hints.res_class = "logoEPIwm";
  XSetWMProperties(gl_display, win, &windowName, &iconName,
		   argv, argc, &size_hints, &wm_hints, &class_hints);
  inputmask = ExposureMask | KeyPressMask | ButtonPressMask | 
    ButtonReleaseMask | StructureNotifyMask | ButtonMotionMask | 
    PointerMotionHintMask | EnterWindowMask | LeaveWindowMask;
  if (float_up) inputmask |= VisibilityChangeMask;
  XSelectInput(gl_display, win, inputmask);
  XMapWindow(gl_display,win);
  while (1)
    {
      XNextEvent(gl_display, &report);
      switch(report.type)
	{
	case Expose:
	  if (report.xexpose.count != 0)
	    break;
	  break;
	case ConfigureNotify:
	  width = report.xconfigure.width;
	  height = report.xconfigure.height;
	  break;
	case ReparentNotify:
	  if (!use_wm)
	    fprintf(stderr,"%s: Window manager wouldn't leave window alone!\n",
		    argv[0]);
	  basewin = report.xreparent.parent;
	  break;
	case EnterNotify:
	  XGrabKeyboard(gl_display, win, False, GrabModeAsync, GrabModeAsync,
			CurrentTime);
	  break;
	case LeaveNotify:
	  XUngrabKeyboard(gl_display, CurrentTime);
	  break;
	case ButtonPress:
	  XRaiseWindow(gl_display, win);
	  XQueryPointer(gl_display, basewin, &root, &child, &tmp_x, &tmp_y,
			&offs_x, &offs_y, &tmp_mask);
	  break;
	case ButtonRelease:
	  XQueryPointer(gl_display, basewin, &root, &child, &new_x, &new_y,
		        &tmp_x, &tmp_y, &tmp_mask);
	  winchanges.x = new_x - offs_x;
	  winchanges.y = new_y - offs_y;
	  XReconfigureWMWindow(gl_display, basewin, gl_screen_num,
			       CWX | CWY, &winchanges);
	  break;
	case MotionNotify:
	  while (XCheckMaskEvent(gl_display, ButtonMotionMask, &report));
	  if (!XQueryPointer(gl_display, report.xmotion.window, &root, &child,
			    &new_x, &new_y, &tmp_x, &tmp_y, &tmp_mask))
	    break;
	  winchanges.x = new_x - offs_x;
	  winchanges.y = new_y - offs_y;
	  XReconfigureWMWindow(gl_display, win, gl_screen_num, CWX | CWY, 
			       &winchanges);
	  break;
	case VisibilityNotify:
	  if (float_up)
	    if ((report.xvisibility.state == VisibilityFullyObscured)
		|| (report.xvisibility.state == VisibilityPartiallyObscured))
	      XRaiseWindow(gl_display,win);
	  break;
	case KeyPress:
	  charcount = XLookupString(&report.xkey, buffer, bufsize,
				    &keysym, &compose);
	  if((keysym == XK_Q) || (keysym == XK_q))
	    {
	      XCloseDisplay(gl_display);
	      exit(1);
	    }
	  break;
	default:
	  break;
	}
      if (!eternal)
	{
	  time1 = time(NULL);
	  time2 = time(NULL);
	  while (difftime(time2, time1) < 2)
	    time2 = time(NULL);
	  exit(1);
	}
    }
}

