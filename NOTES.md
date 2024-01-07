- SensorHandler as shared
    - WaterFlowData is passed through
- case of wifi disconnected for an uncertain time: before, it assumes data will always succeed in sending data through wifi. now not
- assuming device will never turn off

todo:
- add internal/programatical rtc
    v create simple project: [esp-rtc-ntp](https://github.com/royyandzakiy/esp32-rtc-ntp)
    - do checking frequently to sever or ntp server
- local storage using SPIFFs to store unsent data
    - having historical record of unsent data, including timestamp. create a temporary data queue
    - adding timestamp to each datapoint to be sent
    - after reconnection, will do flushing immidiately
    - retains last 5 datapoints for error checking purpose
- has an error log stored in spiffs
    - log all wifi connect disconnect
        - fail to connect to wifi
    - log all rtc calibration to ntp
    - detect if rtc error
        - fail to sync time with ntp
        - check by comparing current rtc with last temporary data queue
    - log all turn on and off
    - log all restart behaviors
        - log forced restarts