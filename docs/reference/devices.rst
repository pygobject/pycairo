.. _devices:

*******
Devices
*******

.. currentmodule:: cairo


class Device()
===============

.. class:: Device()

    A :class:`Device` represents the driver interface for drawing operations
    to a :class:`Surface`.

    .. versionadded:: 1.14

    .. method:: finish()

        This function finishes the device and drops all references to external
        resources. All surfaces, fonts and other objects created for this
        device will be finished, too. Further operations on the device will
        not affect the device but will instead trigger a
        :attr:`Status.DEVICE_FINISHED` error.

        This function may acquire devices.

        .. versionadded:: 1.14

    .. method:: flush()

        Finish any pending operations for the device and also restore any
        temporary modifications cairo has made to the device's state. This
        function must be called before switching from using the device with
        Cairo to operating on it directly with native APIs. If the device
        doesn't support direct access, then this function does nothing.

        This function may acquire devices.

        .. versionadded:: 1.14

    .. method:: acquire()

        :raises cairo.Error:
            If the device is in an error state and could not be acquired.

        Acquires the device for the current thread. This function will block
        until no other thread has acquired the device.

        If the does not raise, you successfully acquired the device. From now
        on your thread owns the device and no other thread will be able to
        acquire it until a matching call to :meth:`release`. It is allowed to
        recursively acquire the device multiple times from the same thread.

        After a successful call to :meth:`acquire`, a matching call to
        :meth:`release` is required.

        .. note::

            You must never acquire two different devices at the same time
            unless this is explicitly allowed. Otherwise the possibility of
            deadlocks exist. As various Cairo functions can acquire devices
            when called, these functions may also cause deadlocks when you
            call them with an acquired device. So you must not have a device
            acquired when calling them. These functions are marked in the
            documentation.

        .. versionadded:: 1.14

    .. method:: release()

        Releases a device previously acquired using :meth:`acquire`. See that
        function for details.

        .. versionadded:: 1.14
