# riot-saul-coap

This application offers saul data via coap.

## Available resources
There are multiple ways supported to receive data from / about
sensors.

### Resource by sensor type
- `/temp` to retrieve the temperature (GET)
- `/hum` to retrieve the humidity (GET)

These resources offer a quick path, to fetch sensor data by type. They
return the values of the first sensor of the requested type; multiple
sensors of the same type are ignored. More sensor types need to be
manually added to the code base. These paths can be used without
knowledge about the RIOT-intern type representation.

### `/sensor`
The `/sensor` resource is reachable with an `GET` request. As payload
it needs the ID of a saul sensor type (as they are defined in
[`drivers/include/saul.h`][saul.h]). GET command has parameter of a 3-digit decimal number to specify sensor type. The query parameter is class.  It will return the values of the first sensor of the requested sensor type. All sensor types will work
out of the box. However, the systems calling this resource, need
information about the RIOT-intern saul type IDs. This could be used by
other RIOT powered boards.

[saul.h]: https://github.com/RIOT-OS/RIOT/blob/d42c032998e77e122380b3d270ceedb7fff48cda/drivers/include/saul.h#L74

### `/saul/cnt` and `/saul/dev` (incomplete)
The idea of these resources is, to offer similar functionality as the
`saul` shell command via a CoAP interface.

- `/saul/cnt` (GET) returns the number of devices, that are connected
  to the board. This can be used to know, what IDs can be used to
  request information about connected sensors.
- `/saul/dev` (POST) needs an ID as argument. Returns some information
  about the sensor for that ID (name and type). 


## Build and Execute
Enter shell with board command (Phytec)

    SERIAL=... BOARD=pba-d-01-kw2x BUILD_IN_DOCKER=1 make all flash term

To distinguish multiple boards using SERIAL number

    make list-ttys 
    
    
###########TO DO###############

human readable parameter(input) -> parse it -> phydat
cbor standard format but still parse 

