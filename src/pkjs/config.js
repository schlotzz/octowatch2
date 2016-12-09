// OctoWatch2
// A Pebble watch app for monitoring and basic controlling of 3D printers via Octoprint
//
// Licence: CC BY-SA 3.0, http://creativecommons.org/licenses/by-sa/3.0/
// Author: Dominik Scholz <pebble@schlotzz.com>, go4u.de Webdesign <info@go4u.de>

module.exports = [
  {
    "type": "heading",
    "defaultValue": "OctoWatch2 Configuration"
  },
  {
    "type": "text",
    "defaultValue": "Please enter your Octoprint configuration."
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Basic Settings"
      },
      {
        "type": "toggle",
        "messageKey": "usessl",
        "label": "Use HTTPS",
        "defaultValue": localStorage.getItem('octoprintusessl') == '1' || false,
        "description": "Activate this function, if you have enabled https on your Octoprint setup and want to encrypt the network traffic."
      },
      {
        "type": "input",
        "messageKey": "host",
        "label": "Host",
        "defaultValue": localStorage.getItem('octoprinthost') || "octopi.local",
        "description": "Enter the IP or network name of your Octoprint server. Default is <i>octopi.local</i>"
      },
      {
        "type": "input",
        "messageKey": "port",
        "label": "Port",
        "defaultValue": localStorage.getItem('octoprintport') || "80",
        "description": "Enter the port number of your Octoprint server. Default is <i>80</i>"
      },
      {
        "type": "input",
        "messageKey": "apikey",
        "label": "APIKEY",
        "defaultValue": localStorage.getItem('octoprintapikey') || "",
        "description": "Enter the APIKEY of your Octoprint server. Must be generated in Octoprint settings before."
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Advanced Settings"
      },
      {
        "type": "toggle",
        "messageKey": "usebasicauth",
        "label": "Use HTTP Basic Auth",
        "defaultValue": localStorage.getItem('octoprintusebasicauth') == '1' || false,
        "description": "Activate this, if your Octoprint server is tunneled through HAProxy and you need to use HTTP Basic Auth."
      },
      {
        "type": "input",
        "messageKey": "user",
        "label": "User",
        "defaultValue": localStorage.getItem('octoprintuser') || "",
        "description": "Enter the username of your HTTP Basic Auth."
      },
      {
        "type": "input",
        "messageKey": "password",
        "label": "Password",
        "defaultValue": localStorage.getItem('octoprintpassword') || "",
        "description": "Enter the password of your HTTP Basic Auth."
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Preheat Settings"
      },
      {
        "type": "input",
        "messageKey": "nozzle",
        "label": "Nozzle",
        "defaultValue": localStorage.getItem('octoprintnozzle') || 185,
        "description": "Set the temperature for preheating your nozzle."
      },
      {
        "type": "input",
        "messageKey": "bed",
        "label": "Heatbed",
        "defaultValue": localStorage.getItem('octoprintbed') || 65,
        "description": "Set the temperature for preheating your heated bed."
      }
    ]
  },
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];