"use strict";
// Client-side interactions with the browser.

// Web sockets: automatically establish a socket with the server
var socket = io.connect();

var AUDIO_INFO_INTERVAL = 800;
var UPTIME_INTERVAL = 1000;
var CHECK_INTERVAL = 5000;
var DECREASE = -5
var INCREASE = 5

// Make connection to server when web page is fully loaded.
$(document).ready(function() {
	$('#error-box').hide();
	
	bindModeControlEvents();
	bindVolumeControlEvents();
	bindTempoControlEvents();
	bindPlayDrumSoundEvents();

	window.setInterval(function() {
		socket.emit('audio_info');
	}, AUDIO_INFO_INTERVAL);
	
	window.setInterval(function() {
		socket.emit('uptime');
	}, UPTIME_INTERVAL);

	window.setInterval(function() {
		socket.emit('beatbox_app_check');
	}, CHECK_INTERVAL);
	
	listenSocketEvents();
});

function bindModeControlEvents() {
	$('#modes > input').click(function() {
		socket.emit('set_mode', $(this).prop('id'));
	});
}

function bindVolumeControlEvents() {
	$('#volumeUp').click(function() {
		var newVolume = parseInt($('#volume-id').prop('value')) + INCREASE;
		socket.emit('set_volume', newVolume);
	});
	
	$('#volumeDown').click(function() {
		var newVolume = parseInt($('#volume-id').prop('value')) + DECREASE;
		socket.emit('set_volume', newVolume);
	});
}

function bindTempoControlEvents() {
	$('#tempoUp').click(function() {
		var newTempo = parseInt($('#tempo-id').prop('value')) + INCREASE;
		socket.emit('set_tempo', newTempo);
	});
	
	$('#tempoDown').click(function() {
		var newTempo = parseInt($('#tempo-id').prop('value')) + DECREASE;
		socket.emit('set_tempo', newTempo);
	});
}

function bindPlayDrumSoundEvents() {
	$('#drum-sounds > input').click(function() {
		socket.emit('play_sound', $(this).prop('id'));
	});
}

function listenSocketEvents() {
	socket.on('uptime_update', function(uptime) {
		var hours = Math.floor(uptime / 3600);
		uptime = uptime % 3600;
		var minutes = Math.floor(uptime / 60);
		var seconds = uptime % 60; 
		
		var str = 'Device up for: ' + hours + ':' + minutes + ':' + seconds + ' (H:M:S)';
		$("#status").html(str);
	})
	
	socket.on('mode_update', function(mode) {
		$('#mode-id').html(mode);
	});
	
	socket.on('volume_update', function(volume) {
		$('#volume-id').prop('value', volume);
	});
	
	socket.on('tempo_update', function(tempo) {
		$('#tempo-id').prop('value', tempo);
	});
	
	socket.on('disconnect', function(reason) {
		$('#error-box').show();
		$('#error-text').html('SERVER ERROR: Disconnected from the node server. Is it running?');
	});
	
	socket.on('beatbox_app_timeout', function(error) {
		$('#error-box').show();
		$('#error-text').html('SERVER ERROR: Timed out communicating to beatbox app. Is it running?');
	});
	
	socket.on('beatbox_app_ok', function(error) {
		$('#error-box').hide();
	});
	
	socket.on('udp_error', function(error) {
		$('#error-box').show();
		$('#error-text').html(error);
	});
}