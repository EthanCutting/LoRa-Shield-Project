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
                    data = ser.readline().strip().decode('utf-8')
                    print(f'Received from {port}:', data)
                    await websocket.send(data)
                    print(f'Data from {port} sent over WebSocket')
                except KeyboardInterrupt:
                    print('Keyboard Interrupt: Exiting...')
                    break
                except Exception as e:
                    print(f'Error sending data from {port} over WebSocket:', e)
    except Exception as e:
        print(f'Serial connection error for {port}:', e)

async def main():
    await asyncio.gather(send_to_websocket('COM5'))

if __name__ == "__main__":
    asyncio.run(main())
