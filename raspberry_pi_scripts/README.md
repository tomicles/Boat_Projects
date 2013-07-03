


* temp
* voltage
* speed
* altitude
* heading
* number of satelites

All information is read from sensors and written to stat/file

Process Architecture
--------------------

1 Process spawns N daemons to read each datagram
1.1 Process Reads a datagram(configurable) and saves it to disk

2 Process reads data from files and updates display
3 Process reads data fro mfiles and sends it via udp



1.1 Architecture



* Readers are specific to counter they are reading

    read_data.sh
        read_temp 
        read_volt  A0 A1 A2 A3
        read_humidity
        read_gps
