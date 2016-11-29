#Octowatch2 ![Octowatch2 logo](https://github.com/schlotzz/octowatch2/blob/master/resources/images/icon_menu.png "Octowatch2 logo")
Use your Pebble watch to monitor and control 3D printers that use the excellent [Octoprint](http://octoprint.org/) host software.

![Octowatch2 screenshot](https://github.com/schlotzz/octowatch2/blob/master/screenshots/octowatch2-main-window.png "Screenshot Octowatch2 main window") ![Octowatch2 screenshot](https://github.com/schlotzz/octowatch2/blob/master/screenshots/octowatch2-appmenu.png "Screenshot Pebble app menu")

##Features
Octowatch2 is heavily inspired by [Octowatch](https://github.com/jjg/octowatch), but completely rewritten from scratch to consider current Pebble SDK4 developments.

* built with current SDK4
* runs on Pebble, Pebble 2, Pebble 2 SE, Pebble Time (currently working on Pebble Time 2)
* supports HTTPS (if used)
* supports HTTP Basic Auth (if used)
* uses AppGlance to display current state in menu
* uses the [Pebble Clay](https://github.com/pebble/clay) config framework
* shows progress bar in main view
* uses confirmation dialog for aborting printjobs
* split C/JS source into several files for better maintainability


##Setup

###Installation
Due to it's early state of development, Octowatch2 has not been published in the [Pebble App Store](https://apps.getpebble.com), yet. Therefore you can download its source and compile it by yourself using the great [Cloud Pebble](https://cloudpebble.net/) online IDE. In order to install this and other self-built apps you have to enable [developer connection](https://developer.pebble.com/guides/tools-and-resources/developer-connection/) in the Pebble app on your smartphone your watch is connected to.

###Configuration
As soon as your Octowatch2 app has been installed on your device, you have to tell it a little about your Octoprint configuration. You need the following:

* Name or IP address of your Octoprint server
* Port that Octoprint is listening on (usually 80 or 5050)
* The [API key](http://docs.octoprint.org/en/master/api/general.html#authorization) for your Octoprint server
* If HTTP Basic Auth is enabled, your username and password

Once you gathered all of the above information, open the Pebble app on your phone. Select Octowatch2 and tap the "SETTINGS" button with the little gear next to it.

![Pebble App Config](https://github.com/schlotzz/octowatch2/blob/master/screenshots/octowatch2-setup.jpg "Pebble App Config")

This will bring up the configuration screen for Octowatch2. Enter your settings into these fields and tap "SAVE SETTINGS".

Next, restart your Octowatch2 app. If everything is setup correctly, your current printer state will be displayed.

##Troubleshooting
Octowatch2 is considered Alpha software at the moment. Some (a lot?) things may change and bugs can be expected. If you experience any issues while connecting to your Octoprint server, double-check the settings. Currently there's no validation, it just won't work.

##Feedback
Please feel free to leave me questions, feedback or bug reports in the issues section of this repository (https://github.com/schlotzz/octowatch2/issues).

##License
This software is released under CreativeCommons [CC-BY-SA 3.0](http://creativecommons.org/licenses/by-sa/3.0/).

![CreativeCommons CC-BY-SA 3.0](https://i.creativecommons.org/l/by-sa/3.0/88x31.png "CreativeCommons CC-BY-SA 3.0")
