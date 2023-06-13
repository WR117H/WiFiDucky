#include "USB.h"
#include "USBHIDKeyboard.h"
#include <WiFi.h>
#include <WebServer.h>
#include <stdbool.h>
#include <string.h>
USBHIDKeyboard Keyboard;
const char* ssid = "WiFi-Ducky";
const char* password = "badusb";
const int serverPort = 80;


WebServer server(serverPort);

const char htmlPage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32-S2 WiFiDucky</title>
  <style>



    body {
      background-color: black;
      color: #14FF00;
      font-family: 'Space Mono', monospace;
      font-weight: bold;
      width: 42%;
      margin: 72px auto;
      


    }

    h1 {
      font-size: 52px;
      text-align: center;
      margin-bottom: 16px;
    }

    .button-container {
      display: flex;
      justify-content: center;
      margin-bottom: 16px;
    }

    .button-container button {
      width: 100%;
      height: 54px;
      font-size: 24px;
      border: 2px solid #14FF00;
      background: transparent;
      font-family: 'Space Mono', monospace;

      border-radius: 8px;
      color: #14FF00;
      margin: 0 4px auto;
      cursor: pointer;
      
    }

    .button-container button:hover {
      border: 2px solid #AEFFA7;
      color: #AEFFA7;      
    }

    .input-container {
      display: flex;
      justify-content: center;
      align-items: center;
      margin-bottom: 16px;
    }

    .input-container textarea {
      width: 100%;
      height: 240px;
      min-height: 120px;
      font-size: 18px;
      border: 2px solid #14FF00;
      border-radius: 10px;
      padding: 10px;
      overflow: hidden;
      font-family: 'Space Mono', monospace;
      background-color: black;
      color: lime;
      resize: vertical;
    }
    .input-container textarea:hover {
      border: 2px solid #AEFFA7;
      color: #AEFFA7;
    }
    .button-sub {
      display: flex;
      justify-content: center;
      align-items: center;
      margin-bottom: 16px;
    }
    
    .button-sub button {
      width: 100%;
      height: 54px;
      font-size: 24px;
      border: 2px solid #14FF00;
      background: transparent;
      font-family: 'Space Mono', monospace;
      border-radius: 8px;
      color: #14FF00;

      cursor: pointer;
    }

    .button-sub button:hover {
      border: 2px solid #AEFFA7;
      color: #AEFFA7;
    }

    #output-container {
      margin-top: 16px;
      text-align: center;
      visibility: hidden;
    }
  </style>
  <script>
    function convertDuckyToArduino() {
      var duckyScript = document.getElementById('duckyScript').value;
      var arduinoCode = '';

      var lines = duckyScript.split('\n');
      for (var i = 0; i < lines.length; i++) {
        var line = lines[i].trim();

        if (line.startsWith('GUI')) {
          var modifierKey = line.substring(3);
          arduinoCode += "GUI";
          arduinoCode += modifierKey;
          arduinoCode += "\n";
        } else if (line.startsWith('DELAY')) {
          var delayTime = line.substring(5).trim();
          arduinoCode += "DELAY " + delayTime + "\n";
        } else if (line.startsWith('STRING')) {
          var text = line.substring(6).trim();
          arduinoCode += "STRING "+ text + '\n';
        } else if (line.startsWith('REM')) {
          arduinoCode += "";
        } else if (line.startsWith('ENTER')) {
          arduinoCode += "ENTER" + "\n";
        }
      }

      document.getElementById('output-container').textContent = arduinoCode;
      sendText(arduinoCode);
    }

    function sendText(text) {
      var lines = text.split('\n');
      for (var i = 0; i < lines.length; i++) {
        var line = lines[i].trim();
        if (line !== '') {
          const url = '/sendText';
          const xhttp = new XMLHttpRequest();
          xhttp.open('POST', url, true);
          xhttp.setRequestHeader('Content-type', 'text/plain');
          xhttp.send(line);
        }
      }
    }




    function addButtonClickListener(buttonId) {
      var button = document.getElementById(buttonId);
      var textarea = document.getElementById('duckyScript');

      button.addEventListener('click', function() {
        textarea.value += buttonId+" ";
      });
    }

    window.addEventListener('load', function() {
      addButtonClickListener('GUI');
      addButtonClickListener('REM');
      addButtonClickListener('STRING');
      addButtonClickListener('DELAY');
    });
    
  </script>
</head>
<body onload="init()">
  <h1>ESP32-S2 WiFI-Ducky</h1>
  <div class="button-container">
    <button id="GUI" onclick="addButtonName('button1')">GUI</button>
    <button id="DELAY" onclick="addButtonName('button2')">DELAY</button>
    <button id="STRING" onclick="addButtonName('button3')">STRING</button>
    <button id="REM" onclick="addButtonName('button4')">REM</button>
  </div>
  <div class="input-container">
    <textarea id="duckyScript" placeholder="Enter your Ducky Script here..."></textarea>
    
  </div>
  <div class="button-sub">
  
    <button  onclick="convertDuckyToArduino()">SEND</button>
  </div>
  <div id="output-container"></div>
</body>
</html>
)=====";




void handleRoot() {
    server.send(200, "text/html", htmlPage);
}
bool startsWith(const char* str, const char* prefix) {
    size_t strLen = strlen(str);
    size_t prefixLen = strlen(prefix);

    if (prefixLen > strLen) {
        return false;
    }

    return strncmp(str, prefix, prefixLen) == 0;
}

void handleText() {
    String text = server.arg("plain");
    Serial.print("Received text: ");


    
    char lines[10][256]; // Adjust the array size and line length as per your needs
    int lineCount = 0;
    int startIndex = 0;
    int endIndex = 0;
    while ((endIndex = text.indexOf('\n', startIndex)) >= 0 && lineCount < 10) {
      text.substring(startIndex, endIndex).toCharArray(lines[lineCount++], 256);
      startIndex = endIndex + 1;
    }
// Handle the last line
    if (startIndex < text.length() && lineCount < 10) {
      text.substring(startIndex).toCharArray(lines[lineCount++], 256);
    }

// Process each line
    for (int i = 0; i < lineCount; i++) {
    
      String line = String(lines[i]);
      line.trim();

      if (line.startsWith("GUI")) {
        int GUI = 0;
        Keyboard.press(KEY_LEFT_GUI);
       
        String text1 = line.substring(4);
        if(text1=="r" or text1=="R"){
          GUI+=114;
        }
        Serial.println(text1);
        Serial.println(GUI);
        Keyboard.press(GUI);
        Keyboard.releaseAll();
    } else if (line.startsWith("DELAY")) {
        int delayTime = atoi(line.substring(6).c_str());
        delay(delayTime);
    } else if (line.startsWith("STRING")) {
        

        String text = line.substring(7);


        Keyboard.print(text);
        
    } else if (line.startsWith("REM")) {
        // Do nothing for REM (comment) lines
    } else if (line.startsWith("ENTER")) {
        // Simulate pressing and releasing the Enter key
        Keyboard.press(KEY_RETURN);
        delay(100);
        Keyboard.release(KEY_RETURN);
        Keyboard.releaseAll();
    }
}


    

    Serial.println(text);
    server.send(200, "text/plain", "Text received");
}

void setup() {
    Serial.begin(115200);
    delay(2000);
    
    Keyboard.begin();
    IPAddress localIP(192, 168, 1, 1);
    IPAddress gateway(192, 168, 1, 1);
    IPAddress subnet(255, 255, 255, 0);

    WiFi.softAPConfig(localIP, gateway, subnet);
    WiFi.softAP(ssid, password);

    IPAddress myIP = WiFi.softAPIP();
    Serial.print("Access Point IP address: ");
    Serial.println(myIP);


    server.on("/", handleRoot);
    server.on("/sendText", handleText);
    server.begin();
    Serial.println("Server started");
}

void loop() {
    server.handleClient();
}
