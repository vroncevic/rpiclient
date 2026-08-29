# -*- coding: UTF-8 -*-

'''
Module
    rpiserver.py
Copyright
    Copyright (C) 2016 - 2026 Vladimir Roncevic <elektron.ronca@gmail.com>
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
Info
    Defines class RPIServer and RPITCPHandler for JSON-RPC 2.0 TCP server.
    Accepts rpi_cmd method calls (SET_CHANNEL, REQ) and returns status ACKs.
'''

from __future__ import annotations

import argparse
import json
import logging
import socketserver
import sys
from typing import Any

__author__: str = 'Vladimir Roncevic'
__copyright__: str = '(C) 2026, https://github.com/vroncevic/rpi_client_gtk'
__credits__: list[str] = ['Vladimir Roncevic', 'Python Software Foundation']
__license__: str = 'https://github.com/vroncevic/rpi_client_gtk/blob/dev/LICENSE'
__version__: str = '1.0.0'
__maintainer__: str = 'Vladimir Roncevic'
__email__: str = 'elektron.ronca@gmail.com'
__status__: str = 'Updated'

logging.basicConfig(
    level=logging.INFO,
    format='[%(asctime)s] [%(levelname)s] (%(threadName)s) %(message)s'
)
logger: logging.Logger = logging.getLogger('rpiserver')


class RPITCPHandler(socketserver.StreamRequestHandler):
    '''
    Request handler for incoming JSON-RPC 2.0 client connections.
    '''

    def handle(self) -> None:
        '''
        Handle incoming TCP stream requests line by line.

        :return: None
        :rtype: None
        '''
        client_address: tuple[str, int] = self.client_address
        logger.info('Client connected from %s:%d', client_address[0], client_address[1])

        while True:
            line_bytes: bytes = self.rfile.readline()
            if not line_bytes:
                logger.info('Client disconnected: %s:%d', client_address[0], client_address[1])
                break

            line_str: str = line_bytes.decode('utf-8', errors='replace').strip()
            if not line_str:
                continue

            logger.debug('Received raw payload: %s', line_str)
            response: dict[str, Any] | None = self._process_message(line_str)
            if response is not None:
                response_str: str = json.dumps(response) + '\n'
                self.wfile.write(response_str.encode('utf-8'))
                self.wfile.flush()
                logger.debug('Sent response: %s', response_str.strip())

    def _process_message(self, raw_message: str) -> dict[str, Any]:
        '''
        Parse and process a single JSON-RPC 2.0 message string.

        :param raw_message: Raw JSON string received from client
        :type raw_message: str
        :return: JSON-RPC 2.0 response dictionary
        :rtype: dict[str, Any]
        '''
        try:
            payload: dict[str, Any] = json.loads(raw_message)
        except json.JSONDecodeError as exc:
            logger.warning('Failed to parse JSON: %s', exc)
            return {
                'jsonrpc': '2.0',
                'error': {'code': -32700, 'message': 'Parse error'},
                'id': None
            }

        req_id: Any = payload.get('id')
        method: Any = payload.get('method')
        params: dict[str, Any] = payload.get('params', {})

        if payload.get('jsonrpc') != '2.0' or method != 'rpi_cmd':
            logger.warning('Invalid JSON-RPC 2.0 method or version: %s', payload)
            return {
                'jsonrpc': '2.0',
                'error': {'code': -32601, 'message': 'Method not found'},
                'id': req_id
            }

        action: str = str(params.get('action', '')).upper()

        if action == 'REQ':
            logger.info('Keepalive REQ received from client (id=%s)', req_id)
            return {
                'jsonrpc': '2.0',
                'result': {'status': 'ACK'},
                'id': req_id
            }

        if action == 'SET_CHANNEL':
            channel: int = int(params.get('channel', 0))
            value: int = int(params.get('value', 0))
            if not (1 <= channel <= 8):
                logger.warning('Channel %d out of valid range (1-8)', channel)
                return {
                    'jsonrpc': '2.0',
                    'error': {'code': -32602, 'message': f'Invalid channel: {channel}. Expected range 1-8.'},
                    'id': req_id
                }
            value = max(0, min(100, value))
            logger.info('SET_CHANNEL received: Channel=%d, Value=%d', channel, value)
            return {
                'jsonrpc': '2.0',
                'result': {
                    'status': 'ACK',
                    'channel': channel,
                    'value': value
                },
                'id': req_id
            }

        logger.warning('Unknown action requested: %s', action)
        return {
            'jsonrpc': '2.0',
            'error': {'code': -32602, 'message': f'Invalid action: {action}'},
            'id': req_id
        }


class RPIServer(socketserver.ThreadingTCPServer):
    '''
    Multi-threaded TCP server for RPI Client connections.
    '''

    allow_reuse_address: bool = True
    daemon_threads: bool = True


def parse_arguments() -> argparse.Namespace:
    '''
    Parse command line arguments for the TCP server.

    :return: Parsed arguments namespace
    :rtype: argparse.Namespace
    '''
    parser: argparse.ArgumentParser = argparse.ArgumentParser(
        description='RPIServer - JSON-RPC 2.0 TCP server for RPI Client'
    )
    parser.add_argument(
        '--host',
        type=str,
        default='127.0.0.1',
        help='Host IP address to bind to (default: 127.0.0.1)'
    )
    parser.add_argument(
        '--port',
        type=int,
        default=8000,
        help='Port number to listen on (default: 8000)'
    )
    parser.add_argument(
        '-v', '--verbose',
        action='store_true',
        help='Enable verbose debug logging'
    )
    return parser.parse_args()


def main() -> None:
    '''
    Main server entry point.

    :return: None
    :rtype: None
    '''
    args: argparse.Namespace = parse_arguments()
    if args.verbose:
        logger.setLevel(logging.DEBUG)

    server_address: tuple[str, int] = (args.host, args.port)
    logger.info('Starting RPIServer on %s:%d...', args.host, args.port)

    try:
        with RPIServer(server_address, RPITCPHandler) as server:
            logger.info('RPIServer is running. Press Ctrl+C to terminate.')
            server.serve_forever()
    except KeyboardInterrupt:
        logger.info('Shutting down RPIServer...')
    except Exception as exc:
        logger.error('Server error: %s', exc)
        sys.exit(1)


if __name__ == '__main__':
    main()
