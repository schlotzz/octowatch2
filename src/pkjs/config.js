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
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];