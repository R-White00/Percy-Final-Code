from this import d
import firebase_admin
from firebase_admin import credentials
from firebase_admin import firestore
import threading
import csv
import time

cred = credentials.Certificate('./percy-dashboard-service-account.json')
firebase_admin.initialize_app(cred)
db = firestore.client()

doc_ref = db.collection(u'Sensors').document(u'ROv8LT7lIkFOlNLmChAR')
cmd_ref = db.collection(u'Commands').document(u'PercyCommands')
# doc_ref.set({
#         u'Battery': 90,

#     })

currentData={'Pressure':0,'Humidity':0,'Temperature':0,'lightIntensity':0,'UV':0,'IR':0,'VOC':0,'Voltage':0,'Current':0, "Moisture":0}
manualOperation = False
def on_snapshot(doc_snapshot, changes, read_time):
    global manualOperation
    for doc in doc_snapshot:
        print('Received document snapshot: ')
        data = doc.to_dict()
        if data["Manual"] == True:
            manualOperation = True
        else:
            manualOperation=False
        if manualOperation == True:
                # doc = cmd_ref.get()

                Data = doc.to_dict()
                # print(f'Document data: {Data["Forward"]}')
                with open('commands.csv','w') as csvfile:
                    writer = csv.writer(csvfile)
                    if(Data["Forward"] == True):
                        writer.writerow(["1","0","0"])
                        print("Onwards To Victory!!!!! ")
                    elif Data["Reverse"] == True:
                        writer.writerow(["2","0","0"])
                    elif Data["Left"] == True:
                        writer.writerow(["3","0","0"])
                    elif Data["Right"] == True:
                        writer.writerow(["4","0","0"])
                    else:
                        writer.writerow(["0","0","0"])
                        print("Nah Chief I'm good")  
            

doc_watch = cmd_ref.on_snapshot(on_snapshot)
    # Create an Event for notifying main thread.

def main():
    print("waiting...   ")
    
    dataList=[]
    dataToSend={}
    currentPressure = 0
    currentHumidity=0
    currentVOC=0
    currentTemp=0
    currentIntesity=0
    currentIR=0
    currentVoltage=0
    currentCurrent=0
    currentUV = 0
    newData=False

    with open('envi_data.csv', 'r') as file:
        reader = csv.DictReader(file)
        for row in reader:
            dataList.append(row)
    for data in dataList[-1]:
        if dataList[-1][data] != currentData[data]:
            newData=True
            currentData[data] = dataList[-1][data]
            # dataToSend.update({data:currentData[data]})
       




    if newData==True:
        # dataToSend.update({u'TimeStamp': firestore.SERVER_TIMESTAMP})
        # doc_ref.set(dataToSend,merge=True)    ##To create one Document with only current measurements
        dataToSend=dataList[-1]
        dataToSend.update({u'TimeStamp': firestore.SERVER_TIMESTAMP})
        db.collection(u'Sensors').add(dataToSend)

    

    # currentPressure=dataList[-1]["Pressure"]
    # currentHumidity=dataList[-1]["Humidity"]
    # currentIntesity=dataList[-1]["Light Intensity"]
    # currentTemp = dataList[-1]["Temperature"]
    # currentUV = dataList[-1]["UV"]
    # currentIR = dataList[-1]["IR"]
    # currentCurrent=dataList[-1]["Current"]
    # currentVoltage=dataList[-1]["Voltage"]
    # currentVOC=dataList[-1]["VOC"]
    # doc_ref.set({
    #      u'Battery': currentVoltage,
    #      u'Pressure': currentPressure,
    #      u'Humidity': currentHumidity,
    #      u'Intensity': currentIntesity,
    #      u'IR': currentIR,
    #      u'VOC': currentVOC,
    #      u'Temperature':currentTemp,
    #      u'UV':currentUV,
    #      u'TimeStamp': firestore.SERVER_TIMESTAMP,


    # })
    print(currentHumidity)
    time.sleep(0.5) 
    
    # Create a callback on_snapshot function to capture changes
    

    # Watch the document


   


if __name__ == "__main__":
    while True:

        main()