Development
===========

First install the external dependencies, see :doc:`./getting_started`.

Both ``poetry`` and ``uv`` are supported for development.

poetry
^^^^^^

To run the tests:

.. code-block:: console

    $ poetry install
    $ poetry run pip install -e .
    # change things
    $ poetry run pytest

To work on the documentation:

.. code-block:: console

    $ poetry install --with docs
    $ poetry run make -C docs watch
    # See http://127.0.0.1:8000

uv
^^

To run the tests:

.. code-block:: console

    $ uv sync
    $ uv pip install -e .
    # change things
    $ uv run pytest

To work on the documentation:

.. code-block:: console

    $ uv sync --group docs
    $ uv run make -C docs watch
    # See http://127.0.0.1:8000

