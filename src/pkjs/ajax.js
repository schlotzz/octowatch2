var Ajax = {
  
  usessl: localStorage.getItem('octoprintusessl') > 0,
  host: localStorage.getItem('octoprinthost'),
  port: localStorage.getItem('octoprintport'),
  apikey: localStorage.getItem('octoprintapikey'),
  usebasicauth: localStorage.getItem('octoprintusebasicauth'),
  auth: {
    user: localStorage.getItem('octoprintuser'),
    password: localStorage.getItem('octoprintpassword')
  },
  
  
  // send ajax call
  send: function(method, path, params, onSuccess, onError) {

    var url = 'http' + (Ajax.usessl ? 's' : '') + '://' + Ajax.host + ':' + Ajax.port + path;
    console.log("calling " + url);

    var req = new XMLHttpRequest(),
        mergedParams = [],
        isMethodPost = method.toLowerCase() === 'post';
    
    // add apikey to params, if get
    if (!isMethodPost)
      params.apikey = Ajax.apikey;

    // merge params
    for (var key in params)
      mergedParams.push(key + '=' + encodeURIComponent(params[key]));

    // open request
    var mergedUrl = url + (isMethodPost ? '' : ('?' + mergedParams.join('&')));
    req.open(method.toUpperCase(), mergedUrl, true);
    
    // if credentials given, use http basic auth
    if (Ajax.usebasicauth)
      req.setRequestHeader('Authorization', 'Basic ' + Base64.encode(Ajax.auth.user + ':' + Ajax.auth.password));

    // set proper header for post
    if (isMethodPost) {
      req.setRequestHeader('Content-Type', 'application/json');
      req.setRequestHeader('X-API-KEY', Ajax.apikey);
    }

    // register callback handlers
    req.onload = function () {
      if (req.readyState === 4) {
        if (req.status === 204) {
          console.log('ajaxCall: got empty response!');
          onSuccess({});
        }
        else if (req.status === 200) {
          console.log('ajaxCall: got data!');
          try {
            onSuccess(JSON.parse(req.responseText));
          }
          catch(e) {
            console.log('ajaxCall: could not parse data or call success callback function!' + req.responseText);
          }
        }
        else if (req.status === 401) {
          console.log('ajaxCall: could not authenticate!', req.responseText);
          onError(req.responseText);
        }
        else {
          console.log('ajaxCall: error receiving data!', req.responseText);
          onError(req.responseText);
        }
      }
    };

    // send request
    console.log('ajaxCall: sent data with ' + method.toLowerCase() + ' method');
    req.send(isMethodPost ? JSON.stringify(params) : null);
  }
};

module.exports = Ajax;

var Base64 = require('./base64.js');