import paho.mqtt.client as paho
import os

device_id = '1'
device_secret = '1234'
random_client_id = '1234'

def on_connect(client, data, flags, rc):
        print('Connected, rc: ' +str(rc))

#subscription event
def on_subscribe(client, userdata, mid, gqos):
        print('Subscribed: ' + str(mid))
        client.publish(out_topic, 'Ready')

#recieved message event
def on_message(client, obj, msg):
        say=msg.payload
        os.system('flite ' + say )
        print('Finished')
        client.publish(out_topic, 'Finished')

#create the MQTT client
client = paho.Client(client_id=random_client_id, protocol=paho.MQTTv31)

#assign event callbacks
client.on_message = on_message
client.on_connect = on_connect
client.on_subscribe = on_subscribe

#topics
in_topic = 'Say' #receive messages
out_topic = 'Said' #publishing messages

#client connect
client.connect("localhost")
client.subscribe(in_topic, 0)

#Continue the network loop, exit when an error occurs
os.system('flite -t "Ready"')

rc = 0
while rc == 0:
        rc = client.loop()
print('rc: ' + str(rc))

