set -e

export MSYS2_FC_CACHE_SKIP=1
# Avoid import mismatch, for example: D:/a/1/s\tests\test_api.py vs D:/a/1/s/tests/test_api.py
export PY_IGNORE_IMPORTMISMATCH=1
export PYTHONPYCACHEPREFIX="$HOME/.cache/$MSYS2_ARCH-$PYTHON/"
mkdir -p $PYTHONPYCACHEPREFIX

pacman --noconfirm -Suy
pacman --noconfirm -S --needed mingw-w64-$MSYS2_ARCH-cairo \
    mingw-w64-$MSYS2_ARCH-$PYTHON mingw-w64-$MSYS2_ARCH-$PYTHON-pip \
    mingw-w64-$MSYS2_ARCH-toolchain git
$PYTHON -m pip install pytest coverage codecov hypothesis
$PYTHON -m pip install mypy || true

export CFLAGS="-std=c90 -Wall -Wno-long-long -Werror -coverage"
$PYTHON -m coverage run --branch setup.py test
$PYTHON -m codecov --required --branch "$CODECOV_BRANCH" || true
$PYTHON setup.py sdist
$PYTHON setup.py install --root="$(pwd)"/_root_abs
$PYTHON -m pip install dist/*

# Also test with older cairo
rm -Rf build
curl -O "http://repo.msys2.org/mingw/$MSYS2_ARCH/mingw-w64-$MSYS2_ARCH-cairo-1.14.4-1-any.pkg.tar.xz"
pacman --noconfirm -U "mingw-w64-$MSYS2_ARCH-cairo-1.14.4-1-any.pkg.tar.xz"
$PYTHON setup.py test
