const http = require('http');
const WebSocketServer = require('websocket').server
let connections = []

const httpserver = http.createServer()
const websocket = new WebSocketServer({ "httpServer": httpserver })
httpserver.listen(8080, () => console.log('Listening on port 8080'))

websocket.on('request', request => {
  const connection = request.accept(null, request.origin)
  connection.on('message', message => {
    connections.forEach(c => c.send(`User ${connection.socket.remotePort} says: ${message.utf8Data}`))
  })
  connections.push(connection)
  connections.forEach(c => c.send(`User ${connection.socket.remotePort} has joined the chat`))
})

/*
  You can paste this in the browser console to test the websocket server
  let ws = new WebSocket('ws://localhost:8080')
  ws.onmessage = message => console.log(message.data)
  ws.send('Hello World')

*/
