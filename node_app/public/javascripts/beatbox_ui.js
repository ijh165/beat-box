"use strict";
// Client-side interactions with the browser.

// Web sockets: automatically establish a socket with the server
var socket = io.connect();

// Make connection to server when web page is fully loaded.
$(document).ready(function() {
	
	/*
	socket.on('daMsg', function(result) {
		$('#messages').append(divMessage(result));
	});
	*/

	window.setInterval(function() {updateDisplay()}, 1000);

	// listen for server reply
	socket.on('modeReply', function(result) {
		$('#modeid').html(result);
	});
});


function updateDisplay() {
	// execute server methods
	socket.emit('mode', "get beat_mode");
}



