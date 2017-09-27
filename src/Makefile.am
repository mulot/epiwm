EXTRA_DIST = Makefile.linux

INCLUDES = $(IMLIB_CFLAGS) $(X_CFLAGS)

bin_PROGRAMS = epiwm

epiwm_SOURCES = button_draw.c cursor.c epiwm.c epiwm_functions.c events.c events_array.c gnome.c icon_styles.c iconify.c init.c key.c menu_draw.c menu_events.c menu_init.c misc.c motif.c mouse_events.c mouse_functions.c mouse_resize.c rev_str.c style_init.c window.c window_draw.c window_events.c window_functions.c window_init.c window_placement.c window_state.c workspace.c workspace_init.c workspace_list.c xmalloc.c \
button_draw.h cursor.h epiwm.h epiwm_functions.h events.h gnome.h icon_styles.h iconify.h init.h key.h menu.h misc.h motif.h mouse_events.h mouse_functions.h screen.h style_init.h window.h window_draw.h window_events.h window_functions.h window_state.h workspace.h workspace_list.h xmalloc.h

epiwm_LDADD =  $(X_LIBS) $(X_PRE_LIBS) $(IMLIB_LIBS) $(X_EXTRA_LIBS)

