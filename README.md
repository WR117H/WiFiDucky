# WiFi-Ducky


![image](https://github.com/freakpunk/WiFi-Ducky/assets/97615989/7191d3be-e2df-4199-a90c-b93777c535b0)

Using WiFiDucky, the ESP32-S2 / WiFi and USB library you can preform HID attacks


## How do I make it?
All you need is an [ESP-32 S2 WiFi microcontroller]()!  



<!-- Features -->
## What are its features

- BadUsb
- Controling device from distances away
- Web Interface


<!-- Installation -->
## How to upload the code on it?

Before uploading the code you need to install the required library in Arduino IDE. Follow these steps:

- Follow this path File> Prefrences
- Look for the "Additional Boards Manager URLs"
- Paste this into it https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_dev_index.json

- Now follow this path Sketch > Include Libraries > Manage Libraries
- Then install esp32 library

   
<!-- Usage -->
## How to interact with it?

After uploading the code, Connect to "WiFiDucky" with "WiFiDucky" password, then head to 192.168.1.1 and the web interface would appear on the screen and you can control the badhsb through the internet

![192 168 1 1_ (1)](https://github.com/spaceeeeboy/Wifi-Ducky/assets/97615989/95a227b3-1617-4f7d-a187-3d3b88518d56)

## Commands

| Command | Description |
| --- | --- |
| `GUI` | Pressing windows key |
| `STRING` | Typing strings |
| `REM` | Just discription |
| `DELAY` | Waiting for a specefic time |
| `ENTER` | To press ENTER button |

<!-- Examples -->
## Examples

```
REM ----EXAMPLE----
DELAY 500
GUI r
DELAY 500
STRING cmd
DELAY 500
ENTER
DELAY 500
STRING YOU'RE HACKED
```

<!-- License -->
## License

Distributed under the MIT License. See LICENSE.txt for more information.


<!-- Contact -->
## Contact

SpaceBoy - spaceboy1112@gmali.com

Project Link: [https://github.com/spaceeeeboy/WifiDucky](https://github.com/spaceb07/WiFi-Ducky)

<!-- Acknowledgments -->
## Acknowledgements 

 - [Spacehun Wi-Fi Ducky](https://github.com/spacehuhn/wifi_ducky)
 - [Seytonic MalDuino](https://github.com/Seytonic/malduino)

