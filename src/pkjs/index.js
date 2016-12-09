// OctoWatch2
// A Pebble watch app for monitoring and basic controlling of 3D printers via Octoprint
//
// Licence: CC BY-SA 3.0, http://creativecommons.org/licenses/by-sa/3.0/
// Author: Dominik Scholz <pebble@schlotzz.com>, go4u.de Webdesign <info@go4u.de>

var Clay = require('pebble-clay'),
    clayConfig = require('./config'),
    clay = new Clay(clayConfig, null, {autoHandleEvents: false}),
    Octowatch = require('./octowatch.js'),
    Octoprint = require('./octoprint.js');


// on app loaded
Pebble.addEventListener('ready', function (e) {
  console.log('connect!');
  Octoprint.fetchStatus();
});


// on received message
Pebble.addEventListener('appmessage', function (e) {
  console.log('received data from pebble: ' + JSON.stringify(e.payload));
  Octowatch.handleCommand(e.payload);  
});


// configuration opened
Pebble.addEventListener('showConfiguration', function(e) {
  console.log('show configuration!');
  Pebble.openURL(clay.generateUrl());
});


// on app closed
Pebble.addEventListener('webviewclosed', function (e) {
  console.log('webview closed!');
  if (e && !e.response) return;
  
  var dict = clay.getSettings(e.response, false),
      value;
  
  for (var key in dict) {
    value = dict[key].value;
    if (value === true) value = 1;
    if (value === false) value = 0;
    console.log("save setting " + key + ": " + value);
    localStorage.setItem('octoprint' + key, value);
  }

});
