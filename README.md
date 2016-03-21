# trumpmask

This repo accompanies a performative object -- a Donald Trump mask that interrupts anyone who speaks with loud and rude phrases clipped from Trump's speeches. Should the wearer of the mask try to muffle the sounds coming from their own (mask-)mouth, Trump's exclamations only become louder.

It uses an Arduino topped with an Adafruit Wave Shield (for audio playback), with two sensors: a microphone (Adafruit Microphone Amplifier Breakout), and a QRD1114 IR emitter to detect proximity of the wearer's hand.

The sketch on the Arduino detects microphone input to trigger WAV playback from the SD card in the Wave shield, while simultaneously monitoring input from an IR proximity sensor to manipulating WAV playback volume based on this analog input. The sketch draws from the daphc and softVolume examples included in the WaveHC library for the Adafruit Arduino Waveshield, as well as the adavoice sketch for the Adafruit Microphone Amplifier Breakout.

Hardware requirements:
* – Arduino Uno, Duemilanove or Diecimila (not Mega or Leonardo compatible).
* – Adafruit Wave Shield
* – Speaker attached to Wave Shield output
* – Battery for portable use
* – Adafruit Microphone Breakout
* – SD card
* – QRD1114 IR emitter / Phototransistor
* – Donald Trump halloween mask
* – Baseball cap to hide components

* Software requirements:
* – WaveHC library for Arduino
* – WAV files on FAT-formatted SD card
* – the .ino contained in this repo

May Trump's reign be short.
