import paho.mqtt.client as mqtt
import threading

BROKER = "localhost"
PORT = 1883
COMMAND_TOPIC = "train/command"
STATUS_TOPIC = "train/status"

def on_connect(client, userdata, flags, rc):
    print("Connected to MQTT broker")
    client.subscribe(STATUS_TOPIC)
    print("Listening for train status...")
    print("======================")
    print("Commands:")
    print("  f = forward")
    print("  r = reverse")
    print("  s = stop")
    print("  q = quit")
    print("======================")

def on_message(client, userdata, msg):
    print(f">> Train status: {msg.payload.decode()}")

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect(BROKER, PORT)
client.loop_start()

while True:
    cmd = input("")
    if cmd == "q":
        print("Quitting")
        client.loop_stop()
        break
    elif cmd in ["f", "r", "s"]:
        client.publish(COMMAND_TOPIC, cmd)
        print(f">> Sent: {cmd}")
    else:
        print(">> Unknown command")