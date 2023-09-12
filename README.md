# Waterbox Firmware
This firmare is used by the GroundEx Waterbox v1.2 

Some features that it has are the following:
- Waterflow data acquisition
- MQTT communication
- LED indicators
- OTA updates

---
![](docs/waterbox%20v1.2.jpg)


## Waterbox Enclosure Plugout
```
1. Flow Sensor (Flow/Volume)
2. Power
```

## To Develop
- Firmware
    - Create release branch
    - Refactor the code to make it more tidy and readable
    - LED function for specific event

- PCB
    - remove RTC module, SD Card module, and GSM module
    - remove unused pins
    - switch to ESP32-WROOM-32U

