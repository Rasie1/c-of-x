#!/usr/bin/python

import json
import os

f = open('../test/tests.json')
  
data = json.load(f)

  
for test_config in data:
    if "disabled" in test_config:
        continue
    # print("./main '" + test_config["expr"] + "' -log")
    stream = os.popen('./main \'' + test_config["expr"] + '\'')
    output = stream.read()
    def fail():
        print("==========================================")
        print("Failed test: " + test_config["name"])
        if "result" in test_config:
            print("Expected:    " + test_config["result"])
        print("Result:      " + output)
        print("Command:") 
        print("./main '" + test_config["expr"] + "' -log")
    if "result" in test_config and output[:-1] != test_config["result"]:
        fail()
    else:
        continue

    if output[:-1] == "cx::nothing": 
        fail()
  
f.close()