// OctoWatch2
// A Pebble watch app for monitoring and basic controlling of 3D printers via Octoprint
//
// Licence: CC BY-SA 3.0, http://creativecommons.org/licenses/by-sa/3.0/
// Author: Dominik Scholz <pebble@schlotzz.com>, go4u.de Webdesign <info@go4u.de>

var Octowatch = {
  
  // data
  data: {
    filename: 'no file selected',
    timeRemaining: '--:--',
    state: 'Loading',
    previousState: 'Loading',
    progress: 0,
    temp0: 0,
    temp1: 0,
    tempbed: 0
  },
  
  
  // handle an incoming command
  handleCommand: function(data) {
    switch(data[0]) {
      // update printer status
      case 'update':
        Octoprint.fetchStatus();
        break;
        
      // pause/resume print job
      case 'pause':
        if (Octowatch.data.state == 'Offline') Octoprint.connectPrinter();
        else Octoprint.pauseResumeJob();
        break;
        
      // cancel print job
      case 'cancel':
        Octoprint.cancelJob();
        break;
        
      // preheat nozzle and bed
      case 'preheat':
        switch(data[1]) {
          case 'bed':
            Octoprint.sendCommand("M117 Preheating Bed..."); // display message
            Octoprint.sendCommand("M140 S" + localStorage.getItem('octoprintbed')); // set bed temp
            break;
          case 'nozzle':
            Octoprint.sendCommand("M117 Preheating Nozzle..."); // display message
            Octoprint.sendCommand("M104 S" + localStorage.getItem('octoprintnozzle')); // set nozzle temp
            break;
          default:
            Octoprint.sendCommand("M117 Preheating..."); // display message
            Octoprint.sendCommand("M104 S" + localStorage.getItem('octoprintnozzle')); // set nozzle temp
            Octoprint.sendCommand("M140 S" + localStorage.getItem('octoprintbed')); // set bed temp
        }
        break;
      
      // send gcode command(s)
      case 'gcode':
        var commands = data[1].split("\n");
        for (var i in commands)
          Octoprint.sendCommand(commands[i]);
        break;
    }
  },
  
  
  // update job state
  updateJobState: function() {
    Octowatch.updatePebble({
      '1': Octowatch.data.filename,
      '2': Octowatch.data.remainingTime >= 0 ? Octowatch.secondsToReadableTime(Octowatch.data.remainingTime) : '--:--',
      '3': Octowatch.data.state,
      '4': Math.floor(Octowatch.data.progress),
      '5': Octowatch.data.remainingTime >= 0 ? Math.floor(new Date().getTime() / 1000 + parseInt(Octowatch.data.remainingTime)) : 0
    });
  },
  
  
  // update printer state
  updatePrinterState: function() {
    Octowatch.updatePebble({
      '6': Octowatch.data.temp0,
      '7': Octowatch.data.temp1,
      '8': Octowatch.data.tempbed
    });
  },
  
  
  // update data on pebble watch
  updatePebble: function(data) {
    Pebble.sendAppMessage(
      data,
      // sent data to pebble successfully
      function() {
        console.log('updatePebble: sent data successfully');
      },
      // some error occured
      function() {
        console.log('updatePebble: sending data failed');
      }
    );
    
    // show notification
    if (Octowatch.data.previousState === 'Printing' && Octowatch.data.state === 'Operational' && Octowatch.data.progress === 100.0) {
      var d = new Date();
      Pebble.showSimpleNotificationOnPebble('Print completed', Octowatch.data.filename + ' finished printing at ' + d);
    }
  },

  
  // convert seconds to readable format HH:MM
  secondsToReadableTime: function(seconds) {
    var hours = Math.floor(seconds / 3600),
        minutes = Math.ceil(seconds % 3600 / 60);
    if (hours < 10) hours = '0' + hours;
    if (minutes < 10) minutes = '0' + minutes;
    return hours + ':' + minutes;
  }
  
};

module.exports = Octowatch;

var Octoprint = require('./octoprint.js');