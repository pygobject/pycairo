set -e

export MSYS2_FC_CACHE_SKIP=1
pacman --noconfirm -Suy --force
pacman --noconfirm -S --needed mingw-w64-$MSYS2_ARCH-cairo \
    mingw-w64-$MSYS2_ARCH-$PYTHON mingw-w64-$MSYS2_ARCH-$PYTHON-pip \
    mingw-w64-$MSYS2_ARCH-toolchain git
$PYTHON -m pip install pytest coverage codecov hypothesis
$PYTHON -m pip install mypy || true

export CFLAGS="-std=c90 -Wall -Wno-long-long -Werror -coverage"
$PYTHON -m coverage run --branch setup.py test
$PYTHON -m codecov --required --branch "$CODECOV_BRANCH"
$PYTHON setup.py sdist
$PYTHON setup.py install --root="$(pwd)"/_root_abs
$PYTHON -m pip install dist/*

# Also test with older cairo
rm -Rf build
curl -O "http://repo.msys2.org/mingw/$MSYS2_ARCH/mingw-w64-$MSYS2_ARCH-cairo-1.14.4-1-any.pkg.tar.xz"
pacman --noconfirm -U "mingw-w64-$MSYS2_ARCH-cairo-1.14.4-1-any.pkg.tar.xz"
$PYTHON setup.py test
