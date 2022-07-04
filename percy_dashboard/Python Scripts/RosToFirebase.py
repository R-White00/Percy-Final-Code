import firebase_admin
from firebase_admin import credentials
from firebase_admin import firestore

cred = credentials.Certificate('./percy-dashboard-service-account.json')
firebase_admin.initialize_app(cred)
db = firestore.client()

doc_ref = db.collection(u'Sensors').document(u'ROv8LT7lIkFOlNLmChAR')
doc_ref.set({
    u'Battery': 80,

})

import csv
with open('data1.csv', 'r') as file:
    reader = csv.DictReader(file)
    for row in reader:
        print(row)