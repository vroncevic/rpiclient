rpiclient
--------------

.. image:: https://raw.githubusercontent.com/vroncevic/rpiclient/refs/heads/master/docs/rpi_client_logo.png
   :align: right
   :width: 25%

**rpiclient** is a graphical desktop application designed for real-time monitoring and remote control of Raspberry Pi nodes over TCP/IP networks.

Developed in modern **C** code with native support for both **GTK+ 3.0** and **GTK 4.0** and Cairo graphics.

.. toctree::
   :hidden:

   self

|Build Status| |C Checker| |Documentation Status| |GitHub issues| |GitHub contributors|

.. |Build Status| image:: https://github.com/vroncevic/rpiclient/actions/workflows/rpiclient_build_checker.yml/badge.svg
   :target: https://github.com/vroncevic/rpiclient/actions/workflows/rpiclient_build_checker.yml

.. |C Checker| image:: https://github.com/vroncevic/rpiclient/actions/workflows/rpiclient_c_checker.yml/badge.svg
   :target: https://github.com/vroncevic/rpiclient/actions/workflows/rpiclient_c_checker.yml

.. |Documentation Status| image:: https://readthedocs.org/projects/rpi-client-gtk/badge/?version=master
   :target: https://rpi-client-gtk.readthedocs.io/?badge=master

.. |GitHub issues| image:: https://img.shields.io/github/issues/vroncevic/rpiclient.svg
   :target: https://github.com/vroncevic/rpiclient/issues

.. |GitHub contributors| image:: https://img.shields.io/github/contributors/vroncevic/rpiclient.svg
   :target: https://github.com/vroncevic/rpiclient/graphs/contributors

Overview & Features
-------------------

* **Dual GTK Support (GTK 3.0 & GTK 4.0)**:
  * Fully modular backend architecture targeting either **GTK+ 3.0** or **GTK 4.0** via ``./configure --with-gtk=3.0`` or ``--with-gtk=4.0``.
  * Native widget hierarchies and signal routing for both toolkits.
* **8-Channel Real-time Control**:
  * Independent control for 8 channels (Channel 1 – Channel 8).
  * Interactive controls: vertical slider scales, precision spin buttons, and individual activation checkboxes.
  * Custom Cairo-rendered vertical LED bar graphs with multi-segment visual feedback.
* **8-Channel Real-time Telemetry & Status Monitoring**:
  * Live status feedback panel displaying confirmed states for all 8 channels.
  * Individual channel status gating with real-time numeric and graphical display.
* **Interactive Help Guide**:
  * Built-in 5-step interactive help wizard available via **Help ➔ Help** with step-by-step guidance on network configuration, channel control, and monitoring.
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

.. image:: https://raw.githubusercontent.com/vroncevic/rpiclient/master/docs/debtux.png
   :alt: Debian Linux OS

To build and install the application:

Autogeneration & Configuration
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

By default, the application is configured to build against **GTK 3.0**:

.. code-block:: bash

   cd rpiclient
   ./autogen.sh

To configure and build against **GTK 4.0**:

.. code-block:: bash

   cd rpiclient
   ./autogen.sh --with-gtk=4.0

Alternatively, if ``configure`` has already been generated:

.. code-block:: bash

   # Configure for GTK 3.0 (default)
   ./configure --with-gtk=3.0

   # Configure for GTK 4.0
   ./configure --with-gtk=4.0

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

* `gtk+ 3.0 <https://docs.gtk.org/gtk3/>`_ or `gtk4 <https://docs.gtk.org/gtk4/>`_
* `glib 2.0 <https://docs.gtk.org/glib/>`_
* `sqlite3 <https://www.sqlite.org/>`_
* `intltool <https://launchpad.net/intltool>`_
* `libtool <https://www.gnu.org/software/libtool/>`_

Docs
----

More documentation and info at:

* `rpi-client-gtk.readthedocs.io <https://rpi-client-gtk.readthedocs.io/>`_
* `gtk+ 3.0 <https://docs.gtk.org/gtk3/>`_
* `gtk4 <https://docs.gtk.org/gtk4/>`_
* `sqlite <https://www.sqlite.org/>`_

Contributing
------------

1. Fork it!
2. Create your feature branch: ``git checkout -b my-new-feature``
3. Commit your changes: ``git commit -am 'Add some feature'``
4. Push to the branch: ``git push origin my-new-feature``
5. Submit a pull request.

Copyright and Licence
---------------------

Copyright (c) 2016 - 2026 Vladimir Roncevic <elektron.ronca@gmail.com>

rpiclient is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

rpiclient is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program. If not, see <http://www.gnu.org/licenses/>.
