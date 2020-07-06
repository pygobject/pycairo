
cp cairocomplied/lib/x64/cairo.dll cairo/cairo.dll
choco install python --version=$PYVER
export PATH="/c/$PYDIR:/c/$PYDIR/Scripts:$PATH"
cmd.exe //c "RefreshEnv.cmd"
python -m pip install --upgrade pip
python -m pip install --upgrade wheel
python -m pip install --upgrade setuptools
python -m pip install pytest
python -m pip install --upgrade mypy || true
cmd.exe //c "RefreshEnv.cmd"
python setup.py bdist_wheel
pip install dist/$WHEELSNAMEx64
python testcairo.py
rm -f cairo/cairo.dll