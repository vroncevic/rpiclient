# GTK TCP/IP RPI Client

<img align="right" src="https://raw.githubusercontent.com/vroncevic/rpiclient/refs/heads/master/docs/rpi_client_logo.png" width="25%">

**rpiclient** is a graphical desktop application designed for real-time monitoring and remote control of Raspberry Pi nodes over TCP/IP networks.

Developed in modern **[C](https://en.wikipedia.org/wiki/C_(programming_language))** with native support for both **GTK+ 3.0** and **GTK 4.0** and Cairo graphics.

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**

- [Overview & Features](#overview--features)
- [Installation](#installation)
  - [Autogeneration & Configuration](#autogeneration--configuration)
  - [Compile](#compile)
  - [Install](#install)
- [Python TCP Server](#python-tcp-server)
- [JSON-RPC 2.0 Protocol](#json-rpc-20-protocol)
- [Dependencies](#dependencies)
- [Docs](#docs)
- [Contributing](#contributing)
- [Copyright and Licence](#copyright-and-licence)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

[![rpiclient_build_checker](https://github.com/vroncevic/rpiclient/actions/workflows/rpiclient_build_checker.yml/badge.svg)](https://github.com/vroncevic/rpiclient/actions/workflows/rpiclient_build_checker.yml) [![rpiclient_c_checker](https://github.com/vroncevic/rpiclient/actions/workflows/rpiclient_c_checker.yml/badge.svg)](https://github.com/vroncevic/rpiclient/actions/workflows/rpiclient_c_checker.yml) [![Documentation Status](https://readthedocs.org/projects/rpi-client-gtk/badge/?version=master)](https://rpi-client-gtk.readthedocs.io/?badge=master) [![GitHub issues open](https://img.shields.io/github/issues/vroncevic/rpiclient.svg)](https://github.com/vroncevic/rpiclient/issues) [![GitHub contributors](https://img.shields.io/github/contributors/vroncevic/rpiclient.svg)](https://github.com/vroncevic/rpiclient/graphs/contributors)

---

### Overview & Features

* **Dual GTK Support (GTK 3.0 & GTK 4.0)**:
  * Fully modular backend architecture targeting either **GTK+ 3.0** or **GTK 4.0** via `./configure --with-gtk=3.0` or `--with-gtk=4.0`.
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
  * Background worker thread (`GThread`) for asynchronous TCP socket I/O.
  * Thread-safe UI updates dispatched directly to the GTK main loop via `g_idle_add()`.
  * Automatic keep-alive / heartbeat handshake mechanism.
* **Integrated Python Node Server**:
  * Includes `scripts/rpiserver.py` — a multi-threaded JSON-RPC 2.0 TCP server for local testing or deployment on the Raspberry Pi node.
* **Configuration Management & Storage**:
  * Flexible settings architecture with multiple storage providers (Plain INI and SQLite3).
* **Modern Embedded UI Styling**:
  * Dark theme aesthetics powered by GTK CSS bundled directly into the executable using GLib Resources (`GResource`).

---

### Installation

![Debian Linux OS](https://raw.githubusercontent.com/vroncevic/rpiclient/master/docs/debtux.png)

To build and install the application:

#### Autogeneration & Configuration

By default, the application is configured to build against **GTK 3.0**:

```bash
cd rpiclient
./autogen.sh
```

To configure and compile against **GTK 4.0**:

```bash
cd rpiclient
./autogen.sh --with-gtk=4.0
```

Alternatively, if `configure` has already been generated:

```bash
# Configure for GTK 3.0 (default)
./configure --with-gtk=3.0

# Configure for GTK 4.0
./configure --with-gtk=4.0
```

#### Compile

```bash
make
```

#### Install

```bash
sudo make install
```

---

### Python TCP Server

The repository includes a JSON-RPC 2.0 multi-threaded TCP server for handling real-time RPI node communication:

```bash
python3 rpiclient/scripts/rpiserver.py --host 127.0.0.1 --port 8000
```

Options:
* `--host`: Server host/IP address (default: `127.0.0.1`)
* `--port`: Server listening port (default: `8000`)
* `-v`, `--verbose`: Enable verbose debug logging

---

### JSON-RPC 2.0 Protocol

All communication between the client and server uses line-delimited (`\n`) JSON-RPC 2.0 messages:

* **Channel Level Control (`SET_CHANNEL`)**:
  * **Request (Client $\rightarrow$ Server)**:
    ```json
    {"jsonrpc": "2.0", "method": "rpi_cmd", "params": {"action": "SET_CHANNEL", "channel": 1, "value": 75}, "id": 101}
    ```
  * **Response (Server $\rightarrow$ Client)**:
    ```json
    {"jsonrpc": "2.0", "result": {"status": "ACK", "channel": 1, "value": 75}, "id": 101}
    ```

* **Heartbeat / Keep-Alive (`REQ`)**:
  * **Request (Client $\rightarrow$ Server)**:
    ```json
    {"jsonrpc": "2.0", "method": "rpi_cmd", "params": {"action": "REQ"}, "id": 102}
    ```
  * **Response (Server $\rightarrow$ Client)**:
    ```json
    {"jsonrpc": "2.0", "result": {"status": "ACK"}, "id": 102}
    ```

---

### Dependencies

This module requires the following dependencies and libraries:

* [gtk+ 3.0 - docs.gtk.org/gtk3](https://docs.gtk.org/gtk3/) or [gtk4 - docs.gtk.org/gtk4](https://docs.gtk.org/gtk4/)
* [glib 2.0 - docs.gtk.org/glib](https://docs.gtk.org/glib/)
* [sqlite3 - www.sqlite.org](https://www.sqlite.org/)
* [intltool - launchpad.net/intltool](https://launchpad.net/intltool)
* [libtool - www.gnu.org/software/libtool](https://www.gnu.org/software/libtool/)

---

### Docs

[![Documentation Status](https://readthedocs.org/projects/rpi-client-gtk/badge/?version=master)](https://rpi-client-gtk.readthedocs.io/?badge=master)

More documentation and info at:

* [rpi-client-gtk.readthedocs.io](https://rpi-client-gtk.readthedocs.io/)
* [gtk+ 3.0 - docs.gtk.org/gtk3](https://docs.gtk.org/gtk3/)
* [gtk4 - docs.gtk.org/gtk4](https://docs.gtk.org/gtk4/)
* [sqlite - www.sqlite.org](https://www.sqlite.org/)

---

### Contributing

1. Fork it!
2. Create your feature branch: `git checkout -b my-new-feature`
3. Commit your changes: `git commit -am 'Add some feature'`
4. Push to the branch: `git push origin my-new-feature`
5. Submit a pull request.

---

### Copyright and Licence

![Foundations](https://raw.githubusercontent.com/vroncevic/rpiclient/master/docs/foundations.png)

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
