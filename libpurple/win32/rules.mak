# Rules on how to make object files from various sources

%.o: %.c
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDE_PATHS) -o $@ -c $<

%.c: %.xs
	$(PERL) -MExtUtils::ParseXS -e 'ExtUtils::ParseXS::process_file(filename => "$<", output => "$@", typemap => "$(PURPLE_PERL_TOP)/common/typemap");'

%.o: %.rc
	$(WINDRES) -I$(PURPLE_TOP) -i $< -o $@

%.pc: %.pc.in
	sed -e 's|@prefix@|/usr/local|g' \
	    -e 's|@exec_prefix@|$${prefix}|g' \
	    -e 's|@libdir@|$${exec_prefix}/lib|g' \
	    -e 's|@includedir@|$${prefix}/include|g' \
	    -e 's|@datarootdir@|$${prefix}/share|g' \
	    -e 's|@datadir@|$${datarootdir}|g' \
	    -e 's|@sysconfdir@|$${prefix}/etc|g' \
	    -e 's|@VERSION@|$(PIDGIN_VERSION)|g' \
	    -e 's|@PURPLE_MAJOR_VERSION@|$(PURPLE_MAJOR_VERSION)|g' \
	    -e 's|@GSTREAMER_VER@|0.10|g' \
	    -e 's|@abs_top_srcdir@|$(ABS_TOP_SRCDIR)|g' \
	    -e 's|@abs_top_builddir@|$(ABS_TOP_BUILDDIR)|g' \
	    $< > $@

%.desktop: %.desktop.in $(wildcard $(PIDGIN_TREE_TOP)/po/*.po)
	LC_ALL=C $(PERL) $(INTLTOOL_MERGE) -d -u -c $(PIDGIN_TREE_TOP)/po/.intltool-merge-cache $(PIDGIN_TREE_TOP)/po $< $@
