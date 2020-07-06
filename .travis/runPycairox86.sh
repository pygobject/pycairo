cp cairocomplied/lib/x86/cairo.dll cairo/cairo.dll
curl -L https://aka.ms/nugetclidl -o nuget.exe
./nuget install pythonx86 -Version $PYVER -OutputDirectory pythonx86
./pythonx86/pythonx86.$PYVER/tools/python.exe -m pip install --upgrade pip
./pythonx86/pythonx86.$PYVER/tools/python.exe -m pip install --upgrade wheel
./pythonx86/pythonx86.$PYVER/tools/python.exe -m pip install --upgrade setuptools
./pythonx86/pythonx86.$PYVER/tools/python.exe -m pip install pytest
./pythonx86/pythonx86.$PYVER/tools/python.exe -m pip install --upgrade mypy || true
export INCLUDE="$PWD/cairocomplied/include/"
export LIB="$PWD/cairocomplied/lib/x86/"
cmd.exe //c "RefreshEnv.cmd"
./pythonx86/pythonx86.$PYVER/tools/python.exe setup.py bdist_wheel
