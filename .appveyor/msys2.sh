set -e

export MSYS2_FC_CACHE_SKIP=1
pacman --noconfirm -S --needed mingw-w64-$MSYS2_ARCH-cairo \
    mingw-w64-$MSYS2_ARCH-$PYTHON mingw-w64-$MSYS2_ARCH-$PYTHON-pip
$PYTHON -m pip install pytest coverage codecov hypothesis

export CFLAGS="-std=c90 -Wall -Wno-long-long -Werror -coverage"
$PYTHON -m coverage run --branch setup.py test
$PYTHON -m codecov
$PYTHON setup.py sdist
$PYTHON -m pip install dist/*

# Also test with older cairo
curl -O "http://repo.msys2.org/mingw/$MSYS2_ARCH/mingw-w64-$MSYS2_ARCH-cairo-1.14.4-1-any.pkg.tar.xz"
pacman --noconfirm -U "mingw-w64-$MSYS2_ARCH-cairo-1.14.4-1-any.pkg.tar.xz"
$PYTHON setup.py test
