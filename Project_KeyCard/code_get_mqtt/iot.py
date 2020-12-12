#iot.py
import paho.mqtt.client as mqtt
import sqlite3
import requests
data = " "
conn = sqlite3.connect('/home/pi/sqlite/update.db')
cursor = conn.cursor()

print ("Opened database successfully")

def on_connect(client, userdata, flags, rc):    
    print("Result from connect: {}".format(
            mqtt.connack_string(rc)))    
# Subscribe to the senors/alitmeter/1 topic filter 
    client.subscribe("outTopic")
    client.subscribe("delete")
    client.subscribe("clear")
    client.subscribe("deletefile")

def on_subscribe(client, userdata, mid, granted_qos):    
    print("I've subscribed")
def on_message(client, userdata, msg):
    dataTopic = " "
    dataPayload = " "
    dataTopic = format(str(msg.topic))
    dataPayload = format(str(msg.payload))
    dataID = str(dataPayload.replace('b','', 1))
    dataID = dataID.strip("'")
    print(dataTopic)
    print(dataID)
    
    if(dataTopic == "outTopic"):
        cursor.execute("SELECT id, Position, name from Quanlylop")
        record = cursor.fetchall()
        check = False
        for result in record:
            ids = result[0]
            Position = result[1]
            Name = result[2]
            if(dataID==ids):
                if(Position=="GV"):
                    client.publish(topic="inTopic", payload=Position)
                    client.publish(topic="nameTopic", payload=Name)
                    check = False
                    break
                if(Position=="SV"):
                    client.publish(topic="inTopic", payload=Position)
                    client.publish(topic="inTopic", payload=Name)
                    with open("/home/pi/file/diem_danh_lop_Coking2.txt", "a+", encoding="utf-8") as reader:
                        reader.write(Name + "\n")
                    check = False
                    break
                if(Position=="NV"):
                    client.publish(topic="inTopic", payload=Position)
                    client.publish(topic="nameTopic", payload=Name)
                    check = False
                    break
                if((Position=="UK") and (Position!= "GV") and (Position != "SV") and (Position != "NV") and (Position != "")):
                    client.publish(topic="inTopic", payload="UK")
                    check = False
                    break
            if(dataID != ids):
                check = True 
        if(check == True):
            client.publish(topic="inTopic", payload="UK")
            
    if(dataTopic == "delete"):
        cursor.execute("SELECT id, Position from Quanlylop")
        record = cursor.fetchall()
        for result in record:
            ids = result[0]
            Position = result[1]
            if(dataID==ids):
                print(ids)
                cursor.execute('DELETE FROM Quanlylop WHERE id=?', (ids,))
                conn.commit()
                print("Phat")
    if(dataTopic == "clear"):
        if(dataID=="clear"):
            client.publish(topic="inTopic", payload="clear")
    if(dataTopic == "deletefile"):
        with open("/home/pi/file/diem_danh_lop_Coking2.txt", "w", encoding="utf-8") as reader:
            reader.write(" ")
    
if __name__ == "__main__":    
    client = mqtt.Client(protocol=mqtt.MQTTv311)    
    client.on_connect = on_connect    
    client.on_subscribe = on_subscribe    
    client.on_message = on_message    
    client.connect(host="127.0.0.1", port=1883)
    client.loop_forever()