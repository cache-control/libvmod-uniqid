AM_CFLAGS = $(VARNISHAPI_CFLAGS) -Wall -Werror
AM_LDFLAGS = $(VARNISHAPI_LIBS) $(VMOD_LDFLAGS)

vmod_LTLIBRARIES = libvmod_uniqid.la

libvmod_uniqid_la_SOURCES = \
	city.h \
	city.c \
	vmod_uniqid.c

nodist_libvmod_uniqid_la_SOURCES = \
	vcc_uniqid_if.c \
	vcc_uniqid_if.h

dist_man_MANS = vmod_uniqid.3

@BUILD_VMOD_UNIQID@

AM_TESTS_ENVIRONMENT = \
	PATH="$(VMOD_TEST_PATH)" \
	LD_LIBRARY_PATH="$(VARNISH_LIBRARY_PATH)"
TEST_EXTENSIONS = .vtc
VTC_LOG_COMPILER = varnishtest -v
AM_VTC_LOG_FLAGS = -Dvmod_uniqid="$(VMOD_UNIQID)"

TESTS = @VMOD_TESTS@

EXTRA_DIST = \
	vmod_uniqid.vcc \
	$(VMOD_TESTS)
