# OpenTapper
 Open Source EMDR Tapper Program for Arduino Nano (and other Arduino-like MCUs)

A simple program for driving PWM motors with Arduino-like devices. 

Designed for 4 rotary encoders (5-pin) and SSD1306 (4-pin) display. 

Allows inputs for:
	Strength (vibration intensity): 0 - 255 (shown as 0-100 on display)
	Length (vibration duration): 0 - ∞
	Span (length between vibrations): 0 - ∞
	Count (number of passes before program halts): 0 - ∞

Version 0.1 Utilizes the Adafruit library.
Version 0.2 is under development switching to ss_oled for faster rendering.