## Assumptions
- Internet connection will be available on first boot
- Internet connection may be lost along the life cycle, but is expected to return back
- NTP server for timehandler calibration is never down
- Flow sensor will never break

## Class Architecture
- "Data Management"
    - SensorHandler < WaterFlowData >
    - WaterflowSensor : Sensor
    - Datalogger
        - waterflowDataQueue
        - errorLog
- "Core Functions"
    - TimeHandler
    - InternetHandler
    - MqttHandler
- "Misc"
    - OtaHandler
    - LedHandler

![](docs/component-diagram.png)

## WaterFlowData Sequence Flow
- init (happens only when first boot or after restart)
    - wifi
        - HAS to be connected, else log error schedule restart
    - timehandler
        - calibrate timehandler using `configTime` until timeSyncCallback, else log error schedule restart
        - if timesyncCallback success, check latest stored data from Datalogger; if timestamp > latest ts in Datalogger continue, else log error schedule restart
        - calls configTime in XX day intervals (default is never called again after called once in init)
- loop
    - WaterflowSensor function return of flowrate & volume
        - ticks every 1 sec, add up volume, tick resets to 0
    - SensorHandler temporary data holder 
        - acquires new data every 30 mins --> fills WaterFlowData struct, acquire timestamp from timehandler
        - write to Datalogger before mqtt publish, reset volume to 0
        - check internet, mqtt publish (volume reset)
    - Datalogger permanent data holder (until explicitly deleted) 
        - delete when successful mqtt publish (flag as isPublished). retain at least 1 latest data for next timehandler check when esp just turned on, and doing timehandler calibration from ntp 
        - after internet reconnect and WaterFlowData in Datalogger not empty, flush (mqtt publish) as seperate task until empty

## Data Format
- WaterFlowData format:
```
struct WaterFlowData {
    unsigned long timestamp;
    float flowRate;
    float totalVolume;
    bool isPublished;
};
```

- stored Datalogger format:
```
ts:<timestamp>,fr:<flowRate>,vol:<volume>,ispub:<isPublished>;
```

## Error Logging
- log all boot up of device
- log all restart behaviors
    - log forced restarts
    - log all restart reasons
- log all wifi connect disconnect
    - fail to connect to wifi
- log all timehandler calibration to ntp
- detect if timehandler error
    - fail to sync time with ntp
    - check by comparing current timehandler with last temporary data queue

---

## To Do:
- [ ] add internal/programatical timehandler
    - [x] create sample project: [esp-rtc-ntp](https://github.com/royyandzakiy/esp32-rtc-ntp)
    - [ ] redo TimeHandler using configTime to make more simple
    - [ ] do mandatory calibration to sever or ntp server when init; give option to do interval based calibration (default is never)
- [ ] local storage using SPIFFs to store unsent data
    - [ ] create sample project: TBD
    - [ ] having historical record of unsent data, including timestamp. create a temporary data queue
    - [ ] adding timestamp to each datapoint to be sent
    - [ ] after reconnection, will do flushing immidiately
    - [ ] retains last 5 datapoints for error checking purpose
- [ ] has an error log stored in spiffs