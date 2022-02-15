# Music-Display-Stick-Node
Sample WiFi Display System for Straight Line of Lights

This is the code code for my WiFi Music Visualizer System. It is being supplied as a sample from which others can create their own display nodes.

It is ESP32 based and uses the [ESP-NOW Protocol](https://www.espressif.com/en/products/software/esp-now/overview) and [FastLED LED Library](https://www.fastled.io). The function of this node is to receive ESP-NOW WiFi packets and display the music information it contains.

## Requirements
1. Anything goes with light and size
2. NO ADDED SOUND
3. Provide your own DC power source
4. You will need to provide the MAC address of your node. The sample software reports this to the Serial Monitor at 115,200 bps.
5. Software Requirements
   - Packet Version
     - Your node should only respond to version 1 packets
   - Pattern Numbers (0 - 255 sent by TX)
     - 0 - All lights off, packet dump on local Serial Monitor
     - 1 - 255 Your pattern choices can be whatever you want them to be.
       - Depending on your hardware, pattern selection may not apply
       - 255 - Best Show
         - If you have multiple patterns, this is the one you want to show off
     - Any values outside this range (errors) or unhandled should set the display to black.
     - Startup pattern is 1
   - Brightness (0 - 255 sent by TX)
     - LED brightness MUST respond to all levels
     - Your power supply must be able to support full brightness
     - Startup brightness is 64
   - Audio Data (0-100 sent by TX)
     - 7 Audio Channels
     - 1 Average Channel
     - 0 to 100 %
     - Up to you to use the values with the numbers of lights you have.
