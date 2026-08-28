# GTK TCP/IP RPI Client

<img align="right" src="https://raw.githubusercontent.com/vroncevic/rpi_client_gtk/refs/heads/master/docs/rpi_client_logo.png" width="25%">

**rpi_client_gtk** is a graphical desktop application designed for real-time monitoring and remote control of Raspberry Pi nodes over TCP/IP networks.

Developed in modern **[C](https://en.wikipedia.org/wiki/C_(programming_language))** with **GTK+ 3.0** and Cairo graphics.

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**

- [Overview & Features](#overview--features)
- [Installation](#installation)
    - [Autogeneration](#autogeneration)
    - [Compile](#compile)
    - [Install](#install)
- [Python TCP Server](#python-tcp-server)
- [JSON-RPC 2.0 Protocol](#json-rpc-20-protocol)
- [Dependencies](#dependencies)
- [Docs](#docs)
- [Contributing](#contributing)
- [Copyright and Licence](#copyright-and-licence)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

[![rpi_client_gtk_c_checker](https://github.com/vroncevic/rpi_client_gtk/actions/workflows/rpi_client_gtk_c_checker.yml/badge.svg)](https://github.com/vroncevic/rpi_client_gtk/actions/workflows/rpi_client_gtk_c_checker.yml) [![rpi_client_gtk_lines_checker](https://github.com/vroncevic/rpi_client_gtk/actions/workflows/rpi_client_gtk_lines_checker.yml/badge.svg)](https://github.com/vroncevic/rpi_client_gtk/actions/workflows/rpi_client_gtk_lines_checker.yml) [![GitHub issues open](https://img.shields.io/github/issues/vroncevic/rpi_client_gtk.svg)](https://github.com/vroncevic/rpi_client_gtk/issues) [![GitHub contributors](https://img.shields.io/github/contributors/vroncevic/rpi_client_gtk.svg)](https://github.com/vroncevic/rpi_client_gtk/graphs/contributors)

---

### Overview & Features

* **8-Channel Real-time Control**:
  * Independent control for 8 channels (Channel 1 – Channel 8).
  * Interactive controls: vertical slider scales, precision spin buttons, and individual activation checkboxes.
  * Custom Cairo-rendered vertical LED bar graphs with multi-segment visual feedback.
* **8-Channel Real-time Telemetry & Status Monitoring**:
  * Live status feedback panel displaying confirmed states for all 8 channels.
  * Individual channel status gating with real-time numeric and graphical display.
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

![Debian Linux OS](https://raw.githubusercontent.com/vroncevic/rpi_client_gtk/master/docs/debtux.png)

To build and install the application:

#### Autogeneration

```bash
cd rpiclient
./autogen.sh
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

* [gtk+ 3.0 - docs.gtk.org/gtk3](https://docs.gtk.org/gtk3/)
* [sqlite3 - www.sqlite.org](https://www.sqlite.org/)
* [intltool - launchpad.net/intltool](https://launchpad.net/intltool)
* [libtool - www.gnu.org/software/libtool](https://www.gnu.org/software/libtool/)

---

### Docs

[![Documentation Status](https://readthedocs.org/projects/rpi-client-gtk/badge/?version=master)](https://rpi-client-gtk.readthedocs.io/?badge=master)

More documentation and info at:

* [rpi-client-gtk.readthedocs.io](https://rpi-client-gtk.readthedocs.io/)
* [gtk+ - docs.gtk.org/gtk3](https://docs.gtk.org/gtk3/)
* [sqlite - www.sqlite.org](https://www.sqlite.org/)

---

### Contributing

[Contributing to rpi_client_gtk](CONTRIBUTING.md)

---

### Copyright and Licence

Copyright (C) 2016 - 2026 by https://github.com/vroncevic/rpi_client_gtk

**rpiclient** is free software; you can redistribute it and/or modify it.

Lets help and support Raspberry PI && GNOME.

<p align="center">
  <a href="https://www.raspberrypi.org/donate/">
    <img src="https://raw.githubusercontent.com/vroncevic/rpi_client_gtk/refs/heads/master/docs/foundations.png">
  </a>
</p>
