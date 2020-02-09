# Smart-Cane-for-visually-impaired-people
A smart cane/stick with obstacle detection and voice feedback on temperature and humidity coordinated with Raspberry Pi 3 via Publish/Subscribe mechanism.

Boot up a rpi3 and follow the following steps ,

1. Stay up to date

sudo apt-get update
sudo apt-get upgrade

2.You’ll want to install flite - (festival-lite) speech synthesizer. http://www.festvox.org/flite/ ,we are using this for
speech

sudo apt-get install flite

Once installed give it a test and make sure you hear the audio.

flite -t “Hello World”

NB: for volume and other sound specifics,make use of ALSA mixer 

sudo apt-get install alsa-utils

3.Install MQTT

sudo apt-get install mosquitto mosquitto-clients python-mosquitto

4.Install paho-mqtt clients

pip install paho-mqtt

5. Install the given "say.py" script using nano text editor ( credits : https://gist.github.com/kd8bxp/6520a4dcda1a43834a6cda402fa441d2 )

6.Test it

python say.py

7.Next step is to run our source code along with the assembled hardware. Please take care to change
the connection specifics (like wifi login details) in the source code. The topic is hardcoded as "Say"

8. In case of failing to connect to MQTT inspite of setting everything correctly,please reupload the code or restart the device

