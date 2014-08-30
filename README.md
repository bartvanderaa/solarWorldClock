solarWorldClock
===============

The Arduino based Solar World Clock

Bart van der Aa
info@bartvanderaa.com
2013-2014
 
General notes:
The Solar World Clock (SWC) project has been in my mind for about 10 years now, and finally I found a suitable platform (the Arduino) to actually built it. 
But, first things first, what is the WSC project about? And secondly, how can we built one?

The first question is actually fairly easy to answer. Imagine the map of the world where the sun's intensity is projected on the surface of this map.   
One could read the approximate time by tracing the location of the sun, just by identifying on which parts of the globe it is day- or nighttime. 
This, in brief, is the working principle of the WSC. Obviously, it is difficult to know the time in absolute terms, but who needs that anyway ;-)
 
Notes on alorithm:
The program uses a real-time-clock (RTC) to keep track of date and time. Based on date, time, and location the solar intensity is computed.
A vector of intensity values (0-255) is then send to ShiftPWM to control an array of LEDs. Time can now be read based on the digitally projected solar intensity. 
 
Sunset-Sunrise part:
Eastern longitude positive, Western longitude negative       
Northern latitude positive, Southern latitude negative
 
PWM part:
If flickering of the LEDs occurs try to adjust the PWM frequency.
 
Credits go to:
Solar intensity part by The Renewable Resource Data Center (RReDC), downloaded 2014
RTC part by Michael Margolis, downloaded 2014 
PWM part by Elco Jacobs, updated August 2012