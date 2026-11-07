print("Hello Core IOT")

# Thu vien va module dung de ket noi MQTT, xu ly thoi gian va du lieu JSON.
import paho.mqtt.client as mqttclient
import time
import json

# Cau hinh ket noi den broker CoreIoT va thong tin xac thuc thiet bi.
BROKER_ADDRESS = "app.coreiot.io"
PORT = 1883
ACCESS_PASSWORD = "ACCESS_TOKEN"
ACCESS_USERNAME = "DEVICE_01"

# Ham duoc goi khi thiet bi dang ky topic MQTT thanh cong.
def subscribed(client, userdata, mid, granted_qos):
    print("Subscribed...")


# Ham xu ly tin nhan RPC va gui lai gia tri thuoc tinh cho CoreIoT.
def recv_message(client, userdata, message):
    print("Received: ", message.payload.decode("utf-8"))
    temp_data = {'value': True}
    try:
        jsonobj = json.loads(message.payload)
        if jsonobj['method'] == "setValue":
            temp_data['value'] = jsonobj['params']
            client.publish('v1/devices/me/attributes', json.dumps(temp_data), 1)
    except:
        pass


# Ham xu ly trang thai ket noi va dang ky nhan yeu cau RPC.
def connected(client, usedata, flags, rc):
    if rc == 0:
        print("Connected successfully!!")
        client.subscribe("v1/devices/me/rpc/request/+")
    else:
        print("Connection is failed")


# Khoi tao MQTT client, cau hinh xac thuc va ket noi den broker.
client = mqttclient.Client("device_01")
client.username_pw_set(ACCESS_USERNAME, ACCESS_PASSWORD)
client.on_connect = connected
client.connect(BROKER_ADDRESS, 1883)
client.loop_start()

# Dang ky cac ham xu ly khi dang ky topic va khi nhan tin nhan.
client.on_subscribe = subscribed
client.on_message = recv_message

# Gia tri du lieu duoc mo phong va tang dan theo tung chu ky.
temp = 30
humi = 50
light_intesity = 100
counter = 0

# Lap vo han moi 5 giay, gom du lieu JSON va gui telemetry len CoreIoT.
while True:
    collect_data = {'temperature': temp, 'humidity': humi, 'light':light_intesity}
    temp += 1
    humi += 1
    light_intesity += 1
    client.publish('v1/devices/me/telemetry', json.dumps(collect_data), 1)
    time.sleep(5)