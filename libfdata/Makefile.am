AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCTHREADS_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBFCACHE_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBFDATA_DLL_EXPORT@

lib_LTLIBRARIES = libfdata.la

libfdata_la_SOURCES = \
	libfdata.c \
	libfdata_area.c libfdata_area.h \
	libfdata_cache.c libfdata_cache.h \
	libfdata_definitions.h \
	libfdata_error.c libfdata_error.h \
	libfdata_extern.h \
	libfdata_libcdata.h \
	libfdata_libcerror.h \
	libfdata_libcnotify.h \
	libfdata_libfcache.h \
	libfdata_list.c libfdata_list.h \
	libfdata_list_element.c libfdata_list_element.h \
	libfdata_mapped_range.c libfdata_mapped_range.h \
	libfdata_notify.c libfdata_notify.h \
	libfdata_range.c libfdata_range.h \
	libfdata_range_list.c libfdata_range_list.h \
	libfdata_segments_array.c libfdata_segments_array.h \
	libfdata_stream.c libfdata_stream.h \
	libfdata_support.c libfdata_support.h \
	libfdata_types.h \
	libfdata_unused.h \
	libfdata_vector.c libfdata_vector.h

libfdata_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	@LIBCTHREADS_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBFCACHE_LIBADD@ \
	@PTHREAD_LIBADD@

libfdata_la_LDFLAGS = -no-undefined -version-info 1:0:0

EXTRA_DIST = \
	libfdata_definitions.h.in \
	libfdata.rc \
	libfdata.rc.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f libfdata_definitions.h
	/bin/rm -f libfdata.rc
	/bin/rm -f Makefile

splint:
	@echo "Running splint on libfdata ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libfdata_la_SOURCES)

