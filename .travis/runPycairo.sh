#download cairo
curl -L https://github.com/preshing/cairo-windows/releases/download/$CAIRO_VERSION/cairo-windows-$CAIRO_VERSION.zip -o cairocomplied.zip
7z x cairocomplied.zip
mv cairo-windows-$CAIRO_VERSION cairocomplied
curl -L https://aka.ms/nugetclidl -o nuget.exe
#x64 python begins here
export INCLUDE="$PWD/cairocomplied/include/"
export LIB="$PWD/cairocomplied/lib/x64/"
cp cairocomplied/lib/x64/cairo.dll cairo/cairo.dll
./nuget install python -Version $PYVER -OutputDirectory python
./python/python.$PYVER/tools/python.exe -m pip install --upgrade pip
./python/python.$PYVER/tools/python.exe -m pip install --upgrade wheel
./python/python.$PYVER/tools/python.exe -m pip install --upgrade setuptools
./python/python.$PYVER/tools/python.exe -m pip install pytest
./python/python.$PYVER/tools/python.exe -m pip install --upgrade mypy || true
./python/python.$PYVER/tools/python.exe setup.py bdist_wheel
rm cairo/cairo.dll
#x86 python begins here
./nuget install pythonx86 -Version $PYVER -OutputDirectory pythonx86
cp cairocomplied/lib/x86/cairo.dll cairo/cairo.dll
./pythonx86/pythonx86.$PYVER/tools/python.exe -m pip install --upgrade pip
./pythonx86/pythonx86.$PYVER/tools/python.exe -m pip install --upgrade wheel
./pythonx86/pythonx86.$PYVER/tools/python.exe -m pip install --upgrade setuptools
./pythonx86/pythonx86.$PYVER/tools/python.exe -m pip install pytest
./pythonx86/pythonx86.$PYVER/tools/python.exe -m pip install --upgrade mypy || true
export INCLUDE="$PWD/cairocomplied/include/"
export LIB="$PWD/cairocomplied/lib/x86/"
./pythonx86/pythonx86.$PYVER/tools/python.exe setup.py bdist_wheel
