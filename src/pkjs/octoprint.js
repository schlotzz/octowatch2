var Octoprint = {
  
  path: {
    job: '/api/job',
    printer: '/api/printer',
    connection: '/api/connection',
    command: '/api/printer/command'
  },

  
  // fetch status
  fetchStatus: function() {
    Octoprint.fetchJobStatus();
    Octoprint.fetchPrinterStatus();
  },
  
  
  // fetch current job status
  fetchJobStatus: function() {
    Ajax.send(
      'GET',
      Octoprint.path.job,
      {},
      // got response
      function(data) {
        console.log('fetchJobStatus: received data');

        // save previous state
        Octowatch.data.previousState = Octowatch.data.state;

        // get data
        Octowatch.data.state = data.state.split(':', 1).join('');
        Octowatch.data.progress = data.progress.completion;

        if (!data.job.file.name) {
          Octowatch.data.filename = 'no file selected';
          Octowatch.data.remainingTime = 0;
        }
        else {
          Octowatch.data.filename = data.job.file.name;
          Octowatch.data.remainingTime = Math.round(data.progress.printTimeLeft ? data.progress.printTimeLeft : data.job.estimatedPrintTime);
        }
        
        Octowatch.updateJobState();
      },
      // ajax call failed
      function(data) {
        console.log('fetchJobStatus: call failed ' + data);
      }
    );
  },
  

  // fetch current printer status
  fetchPrinterStatus: function() {
    Ajax.send(
      'GET',
      Octoprint.path.printer,
      {},
      // got response
      function(data) {
        console.log('fetchPrinterStatus: received data');
        
        // get data
        if (data.temperature.tool0 && data.temperature.tool0.actual) Octowatch.data.temp0 = Math.round(data.temperature.tool0.actual);
        if (data.temperature.tool1 && data.temperature.tool1.actual) Octowatch.data.temp1 = Math.round(data.temperature.tool1.actual);
        if (data.temperature.bed && data.temperature.bed.actual) Octowatch.data.tempbed = Math.round(data.temperature.bed.actual);
        
        Octowatch.updatePrinterState();
      },
      // ajax call failed
      function(data) {
        console.log('fetchPrinterStatus: call failed ' + data);
      }
    );
  },
  
  
  // pause or resume current job
  pauseResumeJob: function() {
    Ajax.send(
      'POST',
      Octoprint.path.job,
      {command: (Octowatch.data.state != 'Printing' && Octowatch.data.state != 'Paused') ? 'start' : 'pause'},
      // got response
      function(data) {
        console.log('pauseResumeJob: received data');
        Octoprint.fetchJobStatus();
      },
      // ajax call failed
      function(data) {
        console.log('pauseResumeJob: call failed ' + data);
      }
    );
  },
  
  
  // pause or resume current job
  cancelJob: function() {
    Ajax.send(
      'POST',
      Octoprint.path.job,
      {command: 'cancel'},
      // got response
      function(data) {
        console.log('cancelJob: received data');
        Octoprint.fetchJobStatus();
      },
      // ajax call failed
      function(data) {
        console.log('cancelJob: call failed ' + data);
      }
    );
  },
  
  
  // connect to printer
  connectPrinter: function() {
    Ajax.send(
      'POST',
      Octoprint.path.connection,
      {command: 'connect'},
      // got response
      function(data) {
        console.log('connectPrinter: received data');
        Octoprint.fetchJobStatus();
      },
      // ajax call failed
      function(data) {
        console.log('connectPrinter: call failed ' + data);
      }
    );
  },
  
  
  // send gcode command
  sendCommand: function(gcode) {
    Ajax.send(
      'POST',
      Octoprint.path.command,
      {command: gcode},
      // got response
      function(data) {
        console.log('sendCommand: received data');
      },
      // ajax call failed
      function(data) {
        console.log('sendCommand: call failed ' + data);
      }
    );
  }
  
};

module.exports = Octoprint;

var Ajax = require('./ajax.js'),
    Octowatch = require('./octowatch.js');