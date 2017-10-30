"user strict";

var socketio = require('socket.io');
var io;

var dgram = require('dgram');

exports.listen = function(server) {
	io = socketio.listen(server);
	io.set('log level 1');
	
	io.sockets.on('connection', function(socket) {
		handleCommand(socket);
	});
};


function handleCommand(socket) {
	/*
	var errorTimer = setTimeout(function() {
		socket.emit("daError", "Oops: User too slow at sending first command.");
	}, 5000);
	*/	
	
	
	// Server methods
	socket.on('mode', function(data) {

		console.log('command: ' + data);

		var HOST = '127.0.0.1';
		var PORT = 12345;
		var buffer = new Buffer(data);

		var client = dgram.createSocket('udp4');
		client.send(buffer, 0, buffer.length, PORT, HOST, function(err, bytes) {
		    if (err) 
		    	throw err;
		    console.log('UDP message sent to ' + HOST +':'+ PORT);
		});

		// Handle an incoming message over the UDP from the local application.
		client.on('message', function (message, remote) {
		    console.log("UDP Client: message Rx" + remote.address + ':' + remote.port +' - ' + message);

			var reply = message.toString('utf8')
		    socket.emit('modeReply', reply);
		});

		//socket.emit('daMsg', data.text);
	});
};