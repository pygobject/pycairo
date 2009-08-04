'''test by running example scripts
'''
from __future__ import division        # new in 2.2, redundant in 3.0
from __future__ import absolute_import # new in 2.5, redundant in 2.7/3.0
from __future__ import print_function  # new in 2.6, redundant in 3.0

import os
import os.path
import subprocess

#import py.test as test


def test_examples():
    '''run non-gui example scripts and check they exit successfully.
    '''
    os.chdir(os.path.join(os.path.dirname(__file__), '..', 'examples'))
    for f in (x for x in os.listdir('.') if x.endswith('.py')):
        retcode = subprocess.call('python %s' % f, shell=True)
        assert retcode == 0, 'Error: {0} retcode == {1}'.format(f, retcode)


def test_snippets_png():
    '''run all snippets in png mode and check they exit successfully.
    '''
    os.chdir(os.path.join(os.path.dirname(__file__), '..', 'examples',
                          'cairo_snippets'))
    retcode = subprocess.call('python snippets_png.py -s', shell=True)
    assert retcode == 0, 'Error: retcode == {0}'.format(retcode)
