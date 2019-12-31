set CAIRO_VER=1.15.10
wget https://github.com/preshing/cairo-windows/releases/download/%CAIRO_VER%/cairo-windows-%CAIRO_VER%.zip
7z x cairo-windows-%CAIRO_VER%.zip

::~ set MSVC_PLATFORM=x86
::~ set PYTHON=C:\Python27\python.exe
::~ set PATH=
::~ set LIBPATH=

set CAIRO_ROOT=%CD%\cairo-windows-%CAIRO_VER%
set INCLUDE=%CAIRO_ROOT%\include
set LIB=%CAIRO_ROOT%\lib\%MSVC_PLATFORM%
SET PATH=%CD%\cairo-windows-%CAIRO_VER%\lib\%MSVC_PLATFORM%;%PATH%

::~ call "%LOCALAPPDATA%\Programs\Common\Microsoft\Visual C++ for Python\9.0\vcvarsall.bat" %MSVC_PLATFORM%
::~ SET DISTUTILS_USE_SDK=1
::~ SET MSSdk=1

%PYTHON% -m pip install --upgrade setuptools || goto :error
%PYTHON% -m pip install --upgrade pytest hypothesis coverage codecov || goto :error
set CL=/WX
%PYTHON% -m coverage run --branch setup.py test || goto :error
%PYTHON% -m pytest tests\\test_cmod.py || goto :error
%PYTHON% -m coverage xml || goto :error
%PYTHON% -m codecov -f coverage.xml

goto :EOF
:error
exit /b 1
