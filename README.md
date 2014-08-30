solarWorldClock
===============

The Arduino based Solar World Clock<br>

Bart van der Aa<br>
info@bartvanderaa.com<br>
2013-2014<br>
 
General notes:<br>
The Solar World Clock (SWC) project has been in my mind for about 10 years now, and finally I found a suitable platform (the Arduino) to actually built it. 
But, first things first, what is the WSC project about? And secondly, how can we built one?<br>

The first question is actually fairly easy to answer. Imagine the map of the world where the sun's intensity is projected on the surface of this map. One could read the approximate time by tracing the location of the sun, just by identifying on which parts of the globe it is day- or nighttime. This, in brief, is the working principle of the WSC. Obviously, it is difficult to know the time in absolute terms, but who needs that anyway ;-) <br>
 
Notes on algorithm:<br>
The program uses a real-time-clock (RTC) to keep track of date and time. Based on date, time, and location the solar intensity is computed. A vector of intensity values (0-255) is then send to ShiftPWM to control an array of LEDs. Time can now be read based on the digitally projected solar intensity. <br>
 
Sunset-Sunrise part:<br>
Eastern longitude positive, Western longitude negative, Northern latitude positive and Southern latitude negative. <br>
 
PWM part:<br>
If flickering of the LEDs occurs try to adjust the PWM frequency.<br>
 
Credits go to:<br>
Solar intensity part by The Renewable Resource Data Center (RReDC), downloaded 2014;<br>
RTC part by Michael Margolis, downloaded 2014<br>
PWM part by Elco Jacobs, updated August 2012<br>