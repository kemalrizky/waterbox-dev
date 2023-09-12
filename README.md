# Waterbox Firmware
This firmare is used by the GroundEx Waterbox v1.0.1. 

Some features that it has are the following:

- Waterflow data acquisition
- MQTT communication
- LED indicators
- OTA updates

---
![](https://drive.google.com/file/d/1cnpkQm3ad5SrHhxpOwgmNId2cG3FwG7V/view?usp=drive_link)


## Waterbox Enclosure Plugout
```
1. Flow Sensor (Flow/Volume)
2. Power
```

## To Develop
- Firmware
    - Create release branch
    - Refactor the code to make it more tidy and readable
        - dumb down, make sure reliable on basic functions: 
            - flow sensing (ISR, Sensor Class)
            - sensor data publishing (qos0)
            - wifi (connect & reconnect nonblocking, check connection) --> must always be connected to internet
            - mqtt (connect & reconnect nonblocking, check connection, enqueue & store unsent messages)
            - tests: (hardware module & software components)

- PCB
    - remove RTC module, SD Card module, and GSM module
    - remove unused pins
    - switch to ESP32-WROOM-32U
