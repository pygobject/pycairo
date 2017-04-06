#!/usr/bin/env python

import os
import sys
import doctest
import unittest

def _test():
    # check we are running from the test dir
    assert os.getcwd().endswith('/test'), "test.py must be run from the 'test' directory"

    runner = unittest.TextTestRunner()
    suite  = doctest.DocFileSuite (__file__.replace ('.py', '.test'))
    runner.run(suite)


if __name__ == "__main__":
    _test()
