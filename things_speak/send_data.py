#!/usr/bin/env python
import urllib.parse, time, http.client
from random import randint
import random

apikey = "BKCWSFJXCNC5B9CV"  # from ThingsSpeak website

while True:
    myRandom1 = randint(0, 100)
    myRandom2 = random.random()
    print(myRandom1, myRandom2)

    # define three fields for Thingspeak
    params = urllib.parse.urlencode({'field1': myRandom1, 'field2': myRandom2, 'field3': 20, 'key': apikey})
    headers = {"Content-type": "application/x-www-form-urlencoded", "Accept": "text/plain"}
    
    # Use http.client instead of httplib in Python 3
    conn = http.client.HTTPConnection("api.thingspeak.com", 80)
    conn.request("POST", "/update", params, headers)
    response = conn.getresponse()
    data = response.read()
    conn.close()
    
    time.sleep(20)  # Must be at least 15 seconds

