# Master Of Cyber Security
#  COS60011-Technology Design Porject
#  Created by: Ethan PP Cutting - 100942775 - 17/05/2023
#  modified last: Ethan PP Cutting - 25/05/2023

import serial
import asyncio
import websockets

async def send_to_websocket(input_port):
    try:
        ser_input = serial.Serial(input_port, 9600)
        print(f'Serial connection established on {input_port}')

        async with websockets.connect('ws://localhost:8080') as websocket:
            print('WebSocket connection established')

            while True:
                try:
                    # Read data from WebSocket and send to Serial port
                    data_from_websocket = await websocket.recv()
                    ser_input.write(data_from_websocket)
                    print(f'Data sent to {input_port} from WebSocket:', data_from_websocket)

                    # Read data from Serial port and send to WebSocket
                    data_from_serial = ser_input.readline().strip().decode('utf-8')
                    await websocket.send(data_from_serial)
                    print(f'Data sent to WebSocket from {input_port}:', data_from_serial)
                except KeyboardInterrupt:
                    print('Keyboard Interrupt: Exiting...')
                    break
                except Exception as e:
                    print(f'Error:', e)
    except Exception as e:
        print(f'Serial connection error:', e)

async def main():
    await send_to_websocket('COM4')

if __name__ == "__main__":
    asyncio.run(main())
