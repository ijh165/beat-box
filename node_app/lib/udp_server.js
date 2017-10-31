"user strict";

var fs = require('fs');
var socket_io = require('socket.io');
var dgram = require('dgram');

exports.listen = function(server) {
	var io = socket_io.listen(server);
	io.set('log level 1');
	
	io.sockets.on('connection', function(socket) {
		handleCommand(socket);
	});
};

function handleCommand(socket) {	
	// Server methods
	
	socket.on('uptime', function() {
		var absPath = '/proc/uptime';
		//console.log('accessing ' + absPath);
		
		fs.exists(absPath, function(exists) {
			if (exists) {
				fs.readFile(absPath, 'utf8', function(err, fileData) {
					if (err) {
						socket.emit('server_error', 'ERROR: Unable to read file ' + absPath);
					} else {
						var uptimeRaw = fileData.toString('utf8');
						var uptimeSec = Math.floor( parseFloat(uptimeRaw.split(' ')[0]) );
						socket.emit('uptime_update', uptimeSec);
					}
				});
			} else {
				socket.emit('server_error', 'ERROR: File ' + absPath + ' not found.');
			}
		});
	});
	
	socket.on('audio_info', function() {
		communicateWithLocalCApp(socket, 'get mode', function (message) {
		    var mode = message.toString('utf8');
		    socket.emit('mode_update', mode);		    
		});
		
		communicateWithLocalCApp(socket, 'get volume', function (message) {
		    var volume = parseInt(message.toString('utf8'));
		    socket.emit('volume_update', volume);		    
		});
		
		communicateWithLocalCApp(socket, 'get tempo', function (message) {
		    var tempo = parseInt(message.toString('utf8'));
		    socket.emit('tempo_update', tempo);		    
		});
	});
	
	socket.on('set_mode', function(mode) {
		communicateWithLocalCApp(socket, 'set mode ' + mode, function (message) {
			socket.emit('mode_update', mode);
		});
	});
	
	socket.on('set_volume', function(volume) {
		communicateWithLocalCApp(socket, 'set volume ' + volume, function (message) {
			socket.emit('volume_update', volume);
		});
	});
	
	socket.on('set_tempo', function(tempo) {
		communicateWithLocalCApp(socket, 'set tempo ' + tempo, function (message) {
			socket.emit('tempo_update', tempo);	
		});
	});
};

function communicateWithLocalCApp(socket, data, responseCallback) {
	var TIMEOUT = 10000;
	
	var errorTimer = setTimeout(function() {
		socket.emit('beatbox_app_timeout');
	}, TIMEOUT);
	
	// Info for connecting to the local process via UDP
	var PORT = 12345;
	var HOST = '127.0.0.1';
	var buffer = new Buffer(data);

	var client = dgram.createSocket('udp4');
	client.send(buffer, 0, buffer.length, PORT, HOST, function(err, bytes) {
	    if (err) 
	    	throw err;
	    //console.log('UDP message sent to ' + HOST +':'+ PORT);
	});
	
	client.on('listening', function () {
	    var address = client.address();
	    //console.log('UDP Client: listening on ' + address.address + ":" + address.port);
	});
	
	// Handle an incoming message over the UDP from the local application.
	client.on('message', function(message, remote) {
		//console.log('UDP Client: message Rx ' + remote.address + ':' + remote.port +' - ' + message);
		responseCallback(message);
		client.close();
	});
	
	client.on('close', function() {
		//console.log('closed');
		clearTimeout(errorTimer);
		socket.emit('response_recieved');
	});
	
	client.on('error', function(err) {
		socket.emit('udp_error', err);
	});
}
