#!/bin/sh
echo "Setup environment for testing"

usage="$(basename "$0") [-h] [-a <ip address>] [-k] [-t]-- script that setup local environment for Domestic Heating project

where:
    -h  show this help text
    -a  set ci40 board ip address (mandatory)
    -k  kill all running awa components (optional)
    -t  enable test environment (optional)"

if [ ${#@} == 0 ]; then
	echo "Usage: $usage "
  exit
fi

BOARD_IP=
KILL=0
TEST=0

while getopts 'ha:kt' option
do
  case "${option}"
  in
  h) echo "$usage"
     exit
     ;;
	a) BOARD_IP=${OPTARG}
		 ;;
	k) KILL=1
		 ;;
	t) TEST=1
		;;
  esac
done
shift $((OPTIND - 1))

if [ $KILL -eq 1 ]
then
  echo "Killing AWA components"
  killall awa_clientd
	killall awa_bootstrapd
	killall awa_serverd
fi


COAP_PORT=5700
IPC_PORT=50000
BOOTSTRAP_COAP_PORT=25700
BOOTSTRAP=coap://$BOARD_IP:$BOOTSTRAP_COAP_PORT

# source the properties:
. psk.properties


echo "[Server] Setting up local awa server at coapPort: $COAP_PORT, ipcPort $IPC_PORT"
awa_serverd --port $COAP_PORT --ipcPort $IPC_PORT --daemonize

echo "[Bootstrap] Setting up bootstrap at coapPort: $BOOTSTRAP_COAP_PORT"
#Dual bootstrap configuration
awa_bootstrapd -c /etc/lwm2m/project5-ds.bsc -c /etc/lwm2m/project5-localhost.bsc --port $BOOTSTRAP_COAP_PORT --daemonize

COAP_PORT=$((COAP_PORT+1))
IPC_PORT=$((IPC_PORT+1))

echo "[Client] Setting up MasterControllerClient at coapPort: $COAP_PORT, ipcPort $IPC_PORT"
awa_clientd -b $BOOTSTRAP --endPointName MasterControllerClient --port $COAP_PORT --ipcPort $IPC_PORT --daemonize -s --pskIdentity $PSK_IDENTITY1 --pskKey $PSK_SECRET1

awa-client-define --ipcPort $IPC_PORT -o 20011 -j CustomEvent -y single -r 5823 -n "EventIdentifier" -t string -u single -q optional -k rw
awa-client-set --ipcPort $IPC_PORT --create /20011/0
awa-client-set --ipcPort $IPC_PORT --create /20011/0/5823

awa-client-define --ipcPort $IPC_PORT -o 20010 -j CustomConfig -y multiple -r 3500 -n "TimeSchedule" -t string -u single -q optional -k rw \
																										   -r 3501 -n "SensorId" -t string -u single -q optional -k rw
awa-client-set --ipcPort $IPC_PORT --create /20010/0
#time schedule
awa-client-set --ipcPort $IPC_PORT --create /20010/0/3500
#temperature
awa-client-set --ipcPort $IPC_PORT --create /20010/0/3501
#motion
awa-client-set --ipcPort $IPC_PORT --create /20010/1
awa-client-set --ipcPort $IPC_PORT --create /20010/1/3501
#actuator
awa-client-set --ipcPort $IPC_PORT --create /20010/2
awa-client-set --ipcPort $IPC_PORT --create /20010/2/3501
#----------------------------------------------------------------------------------------------------------------------

if [ $TEST -eq 1 ]
then

	COAP_PORT=$((COAP_PORT+1))
	IPC_PORT=$((IPC_PORT+1))

	echo "[Client] Setting up RelayDeviceClient at coapPort: $COAP_PORT, ipcPort: $IPC_PORT"
	awa_clientd -b $BOOTSTRAP --endPointName RelayDeviceClient --port $COAP_PORT --ipcPort $IPC_PORT --daemonize -s --pskIdentity $PSK_IDENTITY2 --pskKey $PSK_SECRET2
	awa-client-define --ipcPort $IPC_PORT -o 3201 -j RelayDevice -y single -r 5550 -n "Digital Output State" -t boolean -u single -q optional -k rw
	awa-client-set 		--ipcPort $IPC_PORT --create /3201/0
	awa-client-set 		--ipcPort $IPC_PORT --create /3201/0/5550
	awa-client-set 		--ipcPort $IPC_PORT /3201/0/5550=true

	COAP_PORT=$((COAP_PORT+1))
	IPC_PORT=$((IPC_PORT+1))

	echo "[Client] Setting up MotionDeviceClient at coapPort: $COAP_PORT, ipcPort: $IPC_PORT"
	awa_clientd -b $BOOTSTRAP --endPointName MotionDeviceClient --port $COAP_PORT --ipcPort $IPC_PORT --daemonize -s --pskIdentity $PSK_IDENTITY3 --pskKey $PSK_SECRET3
	awa-client-define --ipcPort $IPC_PORT -o 3302 -j MotionDevice -y single -r 5500 -n "Sensor Value" -t boolean -u single -q optional -k r
	awa-client-set    --ipcPort $IPC_PORT --create /3302/0
	awa-client-set 	 	--ipcPort $IPC_PORT --create /3302/0/5500
	awa-client-set 	 	--ipcPort $IPC_PORT /3302/0/5500=true

	COAP_PORT=$((COAP_PORT+1))
	IPC_PORT=$((IPC_PORT+1))

	echo "[Client] Setting up TemperatureDeviceClient at coapPort: $COAP_PORT, ipcPort: $IPC_PORT"
	awa_clientd -b $BOOTSTRAP --endPointName TemperatureDeviceClient --port $COAP_PORT --ipcPort $IPC_PORT --daemonize -s --pskIdentity $PSK_IDENTITY4 --pskKey $PSK_SECRET4
	awa-client-define --ipcPort $IPC_PORT -o 3303 -j TemperatureDevice -y single -r 5700 -n "Sensor Value" -t float -u single -q optional -k r
	awa-client-set 		--ipcPort $IPC_PORT --create /3303/0
	awa-client-set 		--ipcPort $IPC_PORT --create /3303/0/5700
	awa-client-set 		--ipcPort $IPC_PORT /3303/0/5700=0.0

	#delta control
	awa-client-define --ipcPort $IPC_PORT -o 3308 -j DeltaDevice -y single -r 5900 -n "Sensor Value" -t float -u single -q optional -k rw
	awa-client-set 		--ipcPort $IPC_PORT --create /3308/0
	awa-client-set 		--ipcPort $IPC_PORT --create /3308/0/5900
	awa-client-set 		--ipcPort $IPC_PORT /3308/0/5900=0.0

fi
