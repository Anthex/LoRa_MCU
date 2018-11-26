
# TO52 - LoRa_MCU
Arduino code used for LoRa communication between a B-L072Z-LRWAN1 board and a dragino LG01 gateway

## Disclaimer
This project is currently under development, it is meant to showcase an example of communication between LoRa modules and is subjet to frequent change. The code currently available in this repo is not optimized nor an example of good practices.

This project is being developped in the context of the TO52 course at [UTBM](http://utbm.fr)

## Description
This repository contains 2 sketches, used to communicate between a [B-L072Z-LRWAN1](https://www.st.com/en/evaluation-tools/b-l072z-lrwan1.html) LoRa discovery board and a [Dragino LG01](http://www.dragino.com/products/lora/item/119-lg01-s.html) LoRa gateway.
 - [Lora_envoi_simple.ino](https://github.com/Anthex/LoRa_MCU/blob/master/Lora_envoi_simple.ino "Lora_envoi_simple.ino") : Discovery board sketch : reads a sensor value (in this case an ultrasonic ping time), displays it on MAX7219EWG-multiplexed 7-segments display array and sends it to the gateway via the LoRa module.
	 - Requires the [LoraRadio](https://github.com/GrumpyOldPizza/ArduinoCore-stm32l0) library by GrumpyOldPizza
	 - Requires the [LedControl](https://github.com/wayoda/LedControl) library by wayoda (optional : for 7-segments display)
	 
 - [gateway_reception_forwarding.ino](https://github.com/Anthex/LoRa_MCU/blob/master/gateway_reception_forwarding.ino "gateway_reception_forwarding.ino") : Reads the incoming LoRa packets, stores it in an array and sends the corresponding values to a web server via [python script](https://github.com/Anthex/LoRa/blob/master/python_example/rest.py)
	 - Requires the [LoRa](https://github.com/sandeepmistry/arduino-LoRa ) library by sandeepmistry
