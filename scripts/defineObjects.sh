#!/bin/sh

IPC_ADDRESS="127.0.0.1"
IPC_PORT=50000

PARAMS="--ipcAddress $IPC_ADDRESS --ipcPort $IPC_PORT"

echo "Preparing MasterController"
awa-server-define $PARAMS -o 20011 -j CustomEvent -y single -r 5823 -n "EventId" -t string -u single -q optional -k rw
awa-server-define $PARAMS -o 20010 -j CustomConfig -y multiple -r 3500 -n "TimeSchedule" -t string -u single -q optional -k rw \
																											 -r 3501 -n "SensorId" -t string -u single -q optional -k rw

echo "Preparing RelayDevice"
awa-server-define $PARAMS -o 3201 -j RelayDevice -y single -r 5550 -n "Digital Output State" -t boolean -u single -q optional -k r

echo "Preparing MotionDevice"
awa-server-define $PARAMS -o 3302 -j MotionDevice -y single -r 5500 -n "Sensor Value" -t boolean -u single -q optional -k r

echo "Preparing TemperatureDevice"
awa-server-define $PARAMS -o 3303 -j TemperatureDevice -y single -r 5700 -n "Sensor Value" -t float -u single -q optional -k r

echo "Preparing DeltaDevice"
awa-server-define $PARAMS -o 3308 -j DeltaDevice -y single -r 5900 -n "Sensor Value" -t float -u single -q optional -k rw

echo "Done"