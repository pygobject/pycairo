all:
	python3 -m sphinx -j8 -b html -n . _build

clean:
	rm -rf _build

.PHONY: clean linkcheck watch

linkcheck:
	sphinx-build -b linkcheck -n . _build

watch:
	sphinx-autobuild --watch ../NEWS -j8 -b html -n . _build
