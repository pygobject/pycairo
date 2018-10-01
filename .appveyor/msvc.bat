set CAIRO_VER=1.15.10
appveyor DownloadFile https://github.com/preshing/cairo-windows/releases/download/%CAIRO_VER%/cairo-windows-%CAIRO_VER%.zip
7z x cairo-windows-%CAIRO_VER%.zip

::~ set MSVC_PLATFORM=x86
::~ set PYTHON_ROOT=Python27
::~ set PATH=
::~ set LIBPATH=

set CAIRO_ROOT=%CD%\cairo-windows-%CAIRO_VER%
set INCLUDE=%CAIRO_ROOT%\include
set LIB=%CAIRO_ROOT%\lib\%MSVC_PLATFORM%
set PYTHON=C:\%PYTHON_ROOT%\python.exe

::~ call "%LOCALAPPDATA%\Programs\Common\Microsoft\Visual C++ for Python\9.0\vcvarsall.bat" %MSVC_PLATFORM%
::~ SET DISTUTILS_USE_SDK=1
::~ SET MSSdk=1

%PYTHON% -m pip install --upgrade setuptools || goto :error
%PYTHON% -m pip install --upgrade pytest hypothesis coverage codecov || goto :error
copy cairo-windows-%CAIRO_VER%\lib\%MSVC_PLATFORM%\cairo.dll cairo || goto :error
set CL=/WX
%PYTHON% -m coverage run --branch setup.py test || goto :error
%PYTHON% -m coverage xml || goto :error
%PYTHON% -m codecov --required -f coverage.xml || goto :error

goto :EOF
:error
exit /b 1
