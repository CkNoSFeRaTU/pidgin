if FUZZ
check_PROGRAMS=\
	fuzz_base16_decode \
	fuzz_html_to_xhtml \
	fuzz_jabber_caps \
	fuzz_jabber_id_new \
	fuzz_markup_is_rtl \
	fuzz_markup_linkify \
	fuzz_markup_strip_html \
	fuzz_mime_decode_field \
	fuzz_quotedp_decode \
	fuzz_str_to_time \
	fuzz_xmlnode

common_CFLAGS=\
	$(GLIB_CFLAGS) \
	$(DEBUG_CFLAGS) \
	$(LIBXML_CFLAGS) \
	-I.. \
	-I$(top_srcdir)/libpurple \
	-DBUILDDIR=\"$(top_builddir)\"

common_LDADD=\
	$(top_builddir)/libpurple/protocols/jabber/libjabber.la \
	$(top_builddir)/libpurple/libpurple.la \
	$(GLIB_LIBS)

fuzz_base16_decode_SOURCES=fuzz_base16_decode.c
fuzz_base16_decode_LDADD=$(common_LDADD)
fuzz_base16_decode_CFLAGS=-fsanitize=fuzzer,address $(common_CFLAGS)

fuzz_html_to_xhtml_SOURCES=fuzz_html_to_xhtml.c
fuzz_html_to_xhtml_LDADD=$(common_LDADD)
fuzz_html_to_xhtml_CFLAGS=-fsanitize=fuzzer,address $(common_CFLAGS)

fuzz_jabber_caps_SOURCES=fuzz_jabber_caps.c
fuzz_jabber_caps_LDADD=$(common_LDADD)
fuzz_jabber_caps_CFLAGS=-fsanitize=fuzzer,address $(common_CFLAGS)

fuzz_jabber_id_new_SOURCES=fuzz_jabber_id_new.c
fuzz_jabber_id_new_LDADD=$(common_LDADD)
fuzz_jabber_id_new_CFLAGS=-fsanitize=fuzzer,address $(common_CFLAGS)

fuzz_markup_is_rtl_SOURCES=fuzz_markup_is_rtl.c
fuzz_markup_is_rtl_LDADD=$(common_LDADD)
fuzz_markup_is_rtl_CFLAGS=-fsanitize=fuzzer,address $(common_CFLAGS)

fuzz_markup_linkify_SOURCES=fuzz_markup_linkify.c
fuzz_markup_linkify_LDADD=$(common_LDADD)
fuzz_markup_linkify_CFLAGS=-fsanitize=fuzzer,address $(common_CFLAGS)

fuzz_markup_strip_html_SOURCES=fuzz_markup_strip_html.c
fuzz_markup_strip_html_LDADD=$(common_LDADD)
fuzz_markup_strip_html_CFLAGS=-fsanitize=fuzzer,address $(common_CFLAGS)

fuzz_mime_decode_field_SOURCES=fuzz_mime_decode_field.c
fuzz_mime_decode_field_LDADD=$(common_LDADD)
fuzz_mime_decode_field_CFLAGS=-fsanitize=fuzzer,address $(common_CFLAGS)

fuzz_quotedp_decode_SOURCES=fuzz_quotedp_decode.c
fuzz_quotedp_decode_LDADD=$(common_LDADD)
fuzz_quotedp_decode_CFLAGS=-fsanitize=fuzzer,address $(common_CFLAGS)

fuzz_str_to_time_SOURCES=fuzz_str_to_time.c
fuzz_str_to_time_LDADD=$(common_LDADD)
fuzz_str_to_time_CFLAGS=-fsanitize=fuzzer,address $(common_CFLAGS)

fuzz_xmlnode_SOURCES=fuzz_xmlnode.c
fuzz_xmlnode_LDADD=$(common_LDADD)
fuzz_xmlnode_CFLAGS=-fsanitize=fuzzer,address $(common_CFLAGS)

endif
