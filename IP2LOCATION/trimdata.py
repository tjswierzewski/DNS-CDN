#!/bin/python3

with open("IP2LOCATION-LITE-DB5.CSV") as in_file:
    with open("IPLocation.csv", "w") as out_file:
        while(True):
            line = in_file.readline()
            if line == "":
                break
            columns = line.split(",")
            output = (",".join((columns[:2] + columns [-2:])))
            out_file.write(output)