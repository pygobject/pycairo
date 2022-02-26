all:
	python3 -m sphinx -b html -n . _build

clean:
	rm -rf _build

.PHONY: clean show linkcheck watch

show: all
	xdg-open _build/index.html

linkcheck:
	sphinx-build -b linkcheck -n . _build

watch:
	sphinx-autobuild -b html -n . _build
