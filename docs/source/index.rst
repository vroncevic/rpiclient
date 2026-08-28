rpi_client_gtk
--------------

.. image:: https://raw.githubusercontent.com/vroncevic/rpi_client_gtk/refs/heads/master/docs/rpi_client_logo.png
   :align: right
   :width: 25%

**rpi_client_gtk** is a graphical desktop application designed for real-time monitoring and remote control of Raspberry Pi nodes over TCP/IP networks.

Developed in modern **C** code with **GTK+ 3.0** and Cairo graphics.

.. toctree::
   :hidden:

   self

|Build Status| |Lines Status| |GitHub issues| |Documentation Status| |GitHub contributors|

.. |Build Status| image:: https://github.com/vroncevic/rpi_client_gtk/actions/workflows/rpi_client_gtk_c_checker.yml/badge.svg
   :target: https://github.com/vroncevic/rpi_client_gtk/actions/workflows/rpi_client_gtk_c_checker.yml

.. |Lines Status| image:: https://github.com/vroncevic/rpi_client_gtk/actions/workflows/rpi_client_gtk_lines_checker.yml/badge.svg
   :target: https://github.com/vroncevic/rpi_client_gtk/actions/workflows/rpi_client_gtk_lines_checker.yml

.. |GitHub issues| image:: https://img.shields.io/github/issues/vroncevic/rpi_client_gtk.svg
   :target: https://github.com/vroncevic/rpi_client_gtk/issues

.. |Documentation Status| image:: https://readthedocs.org/projects/rpi-client-gtk/badge/?version=master
   :target: https://rpi-client-gtk.readthedocs.io/?badge=master

.. |GitHub contributors| image:: https://img.shields.io/github/contributors/vroncevic/rpi_client_gtk.svg
   :target: https://github.com/vroncevic/rpi_client_gtk/graphs/contributors

Overview & Features
-------------------

* **8-Channel Real-time Control**:
  * Independent control for 8 channels (Channel 1 – Channel 8).
  * Interactive controls: vertical slider scales, precision spin buttons, and individual activation checkboxes.
  * Custom Cairo-rendered vertical LED bar graphs with multi-segment visual feedback.
* **8-Channel Real-time Telemetry & Status Monitoring**:
  * Live status feedback panel displaying confirmed states for all 8 channels.
  * Individual channel status gating with real-time numeric and graphical display.
* **Non-blocking Multithreaded Networking**:
  * Background worker thread (``GThread``) for asynchronous TCP socket I/O.
  * Thread-safe UI updates dispatched directly to the GTK main loop via ``g_idle_add()``.
  * Automatic keep-alive / heartbeat handshake mechanism.
* **Integrated Python Node Server**:
  * Includes ``scripts/rpiserver.py`` — a multi-threaded JSON-RPC 2.0 TCP server for local testing or deployment on the Raspberry Pi node.
* **Configuration Management & Storage**:
  * Flexible settings architecture with multiple storage providers (Plain INI and SQLite3).
* **Modern Embedded UI Styling**:
  * Dark theme aesthetics powered by GTK CSS bundled directly into the executable using GLib Resources (``GResource``).

Installation
------------

.. image:: https://raw.githubusercontent.com/vroncevic/rpi_client_gtk/master/docs/debtux.png
   :alt: Debian Linux OS

To build and install the application:

Autogeneration
^^^^^^^^^^^^^^

.. code-block:: bash

   cd rpiclient
   ./autogen.sh

Compile
^^^^^^^

.. code-block:: bash

   make

Install
^^^^^^^

.. code-block:: bash

   sudo make install

Python TCP Server
-----------------

The repository includes a JSON-RPC 2.0 multi-threaded TCP server for handling real-time RPI node communication:

.. code-block:: bash

   python3 rpiclient/scripts/rpiserver.py --host 127.0.0.1 --port 8000

Options:

* ``--host``: Server host/IP address (default: ``127.0.0.1``)
* ``--port``: Server listening port (default: ``8000``)
* ``-v``, ``--verbose``: Enable verbose debug logging

JSON-RPC 2.0 Protocol
---------------------

All communication between the client and server uses line-delimited (``\n``) JSON-RPC 2.0 messages:

* **Channel Level Control (SET_CHANNEL)**:
  
  Request (Client -> Server):
  
  .. code-block:: json

     {"jsonrpc": "2.0", "method": "rpi_cmd", "params": {"action": "SET_CHANNEL", "channel": 1, "value": 75}, "id": 101}

  Response (Server -> Client):
  
  .. code-block:: json

     {"jsonrpc": "2.0", "result": {"status": "ACK", "channel": 1, "value": 75}, "id": 101}

* **Heartbeat / Keep-Alive (REQ)**:
  
  Request (Client -> Server):
  
  .. code-block:: json

     {"jsonrpc": "2.0", "method": "rpi_cmd", "params": {"action": "REQ"}, "id": 102}

  Response (Server -> Client):
  
  .. code-block:: json

     {"jsonrpc": "2.0", "result": {"status": "ACK"}, "id": 102}

Dependencies
------------

This module requires the following dependencies and libraries:

* `gtk+ 3.0 <https://docs.gtk.org/gtk3/>`_
* `sqlite3 <https://www.sqlite.org/>`_
* `intltool <https://launchpad.net/intltool>`_
* `libtool <https://www.gnu.org/software/libtool/>`_

Contributing
------------

See `CONTRIBUTING.md <https://github.com/vroncevic/rpi_client_gtk/blob/master/CONTRIBUTING.md>`_ for contribution guidelines.

Copyright and Licence
---------------------

Copyright (C) 2016 - 2026 by `vroncevic.github.io/rpi_client_gtk <https://vroncevic.github.io/rpi_client_gtk>`_

**rpiclient** is free software; you can redistribute it and/or modify it.
