configdir = @CONFIG_PATH@

CONFIG_FILE = \
	icons \
	key \
	menu \
	start \
	style \
	window \
	workspace 

EXTRA_DIST = $(CONFIG_FILE)

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(configdir)
	$(INSTALL_DATA) $(CONFIG_FILE) $(DESTDIR)$(configdir)
