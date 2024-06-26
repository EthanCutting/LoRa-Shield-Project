/* 
  Master Of Cyber Security
  COS60011-Technology Design Porject
  Created by: Ethan PP Cutting - 100942775 - 17/05/2023
  modified last: Ethan PP Cutting - 25/05/2023
*/
const WebSocket = require('ws');

const wss = new WebSocket.Server({ port: 8080 });

wss.on('connection', function connection(ws) 
{
  console.log('Client connected');

  ws.on('message', function incoming(message) {
    console.log('Received: %s', message);
    
    // Broadcast the received message to all connected clients
    wss.clients.forEach(function each(client) {
      if (client !== ws && client.readyState === WebSocket.OPEN) {
        client.send(message);
      }
    });
  });

  ws.on('error', function(error) {
    console.error('WebSocket error:', error);
  });
});

wss.on('error', function(error) {
  console.error('WebSocket server error:', error);
});
