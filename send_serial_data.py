import serial
import asyncio
import websockets

async def send_to_websocket(port):
    try:
        ser = serial.Serial(port, 9600)
        print(f'Serial connection established on {port}')

        async with websockets.connect('ws://localhost:8080') as websocket:
            print('WebSocket connection established')

            while True:
                try:
                    # Read data from WebSocket and send to Serial monitor (COM5)
                    data_from_websocket = await websocket.recv()
                    ser.write(data_from_websocket)
                    print(f'Data sent to {port} from WebSocket:', data_from_websocket)

                    # Read data from Serial monitor (COM5) and send to WebSocket
                    data_from_serial = ser.readline().strip().decode('utf-8')
                    await websocket.send(data_from_serial)
                    print(f'Data sent to WebSocket from {port}:', data_from_serial)
                except KeyboardInterrupt:
                    print('Keyboard Interrupt: Exiting...')
                    break
                except Exception as e:
                    print(f'Error:', e)
    except Exception as e:
        print(f'Serial connection error:', e)

async def main():
    await send_to_websocket('COM5')

if __name__ == "__main__":
    asyncio.run(main())
