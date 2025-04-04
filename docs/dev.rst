Development
===========

First install the external dependencies, see :doc:`./getting_started`.

To run the tests:

.. code-block:: console

    $ poetry install
    $ poetry self add poetry-plugin-shell  # once
    $ poetry shell
    $ pip install -e .
    # change things
    $ pytest

To work on the documentation:

.. code-block:: console

    $ poetry install --with docs
    $ poetry self add poetry-plugin-shell  # once
    $ poetry shell
    $ cd docs
    $ make watch
    # See http://127.0.0.1:8000
