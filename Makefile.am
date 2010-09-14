SUBDIRS = src examples test doc

EXTRA_DIST = \
  COPYING \
  COPYING-LGPL-2.1 \
  COPYING-MPL-1.1 \
  pycairo.pc.in \
  setup.py \
  waf \
  wscript

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = pycairo.pc

# release targets from cairo/Makefile.am
# Some custom targets to make it easier to release things.
# Use either:
#		make release-check
# or		make release-publish

RELEASE_UPLOAD_HOST =   cairographics.org
RELEASE_UPLOAD_BASE =	/srv/cairo.freedesktop.org/www

RELEASE_UPLOAD_DIR =	$(RELEASE_UPLOAD_BASE)/releases
RELEASE_URL_BASE = 	http://cairographics.org/releases
#RELEASE_UPLOAD_DIR =	$(RELEASE_UPLOAD_BASE)/snapshots
#RELEASE_URL_BASE = 	http://cairographics.org/snapshots

RELEASE_ANNOUNCE_LIST = cairo-announce@cairographics.org (and CC python-announce-list@python.org)

tar_file = $(PACKAGE)-$(VERSION).tar.gz
sha1_file = $(tar_file).sha1

$(sha1_file): $(tar_file)
	sha1sum $^ > $@

release-verify-even-micro:
	@echo -n "Checking that $(VERSION) has an even micro component..."
	@test "$(PYCAIRO_VERSION_MICRO)" = "`echo $(PYCAIRO_VERSION_MICRO)/2*2 | bc`" \
		|| (echo "Ouch." && echo "The version micro component '$(PYCAIRO_VERSION_MICRO)' is not an even number." \
		&& echo "The version in configure.in must be incremented before a new release." \
		&& false)
	@echo "Good."

release-verify-newer:
	@echo -n "Checking that no $(VERSION) release already exists..."
	@ssh $(RELEASE_UPLOAD_HOST) test ! -e $(RELEASE_UPLOAD_DIR)/$(tar_file) \
		|| (echo "Ouch." && echo "Found: $(RELEASE_UPLOAD_HOST):$(RELEASE_UPLOAD_DIR)/$(tar_file)" \
		&& echo "Are you sure you have an updated CVS checkout?" \
		&& echo "This should never happen." \
		&& false)
	@echo "Good."

release-remove-old:
	rm -f $(tar_file) $(sha1_file)

release-check: release-verify-even-micro release-verify-newer release-remove-old distcheck $(sha1_file)

release-upload: release-check $(tar_file) $(sha1_file)
	mkdir -p releases
	scp $(tar_file) $(sha1_file) $(RELEASE_UPLOAD_HOST):$(RELEASE_UPLOAD_DIR)
	mv $(tar_file) $(sha1_file) releases
	ssh $(RELEASE_UPLOAD_HOST) "rm -f $(RELEASE_UPLOAD_DIR)/LATEST-$(PACKAGE)-[0-9]* && ln -s $(tar_file) $(RELEASE_UPLOAD_DIR)/LATEST-$(PACKAGE)-$(VERSION)"

release-publish: release-upload releases/$(sha1_file)
	@echo ""
	@echo "Please send an announcement to $(RELEASE_ANNOUNCE_LIST)"
	@echo "including the following:"
	@echo ""
	@echo "Subject: ANN: $(PACKAGE) release $(VERSION) now available"
	@echo ""
	@echo "============================== CUT HERE =============================="
	@echo "Pycairo is a set of Python bindings for the multi-platform 2D graphics library cairo."
	@echo " http://cairographics.org"
	@echo " http://cairographics.org/pycairo"
	@echo ""
	@echo "A new $(PACKAGE) release $(VERSION) is now available from:"
	@echo ""
	@echo "	$(RELEASE_URL_BASE)/$(tar_file)"
	@echo "	$(RELEASE_URL_BASE)/$(sha1_file)"
	@echo ""
	@echo -n "	"
	@cat releases/$(sha1_file)
	@echo ""
	@echo "============================== CUT HERE =============================="
	@echo "Also, please include the new entries from the NEWS file."
	@echo ""
	@echo "Last but not least, do not forget to bump up the micro"
	@echo "version component to the next (odd) number and commit."

.PHONY: release-verify-even-micro release-verify-newer release-remove-old release-check release-upload release-publish
