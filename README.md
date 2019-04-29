# Christmasqualizer!

This code allows you to use a [MSGEQ7](http://www.sparkfun.com/products/10468) to control christmas lights!
While the wiring is basically up to you, I found [this site](http://nuewire.com/info-archive/msgeq7-by-j-skoba/)
to be an awesome help with wiring it. I also used a bunch of [S108T02s](http://www.sparkfun.com/products/10636)
for the relays actually controlling the christmas lights and I have to
recommend these if you don't have a better solid-state relay decided on.

As far as the code goes, you'll need to edit the first four lines once
you have your MSGEQ7 wired in so that it all links to the right pins.
The array of pins on the 4th line is your relay / light pins and go
from the lowest channel to the highest in order. You can use less than 7
sets of lights, just set the first and last value in that array to something
or the code won't work (it absolutely must have 7 values!)

Good luck and if you make something with this please send it my way!  
(@kylehotchkiss on Twitter)

## @ndrewGele's Changeblog

### 4/29/2019 (1am)

Happy 2 year anniversary! I just couldn't pull myself away.  
The loop now uses a legit running average and standard deviation to determine whether or not to power each pin.  
I also put some adjustable variables near the top of the script. If anyone wants to tune the settings, it should be relatively easy. :)  

### 4/28/2019

Woah, it's been almost exactly 2 years since I forked this and started tinkering. :O  
I wanted to revisit this project, but I just ended up tweaking some values to get the lights blinking the way I want.  
I thought of a way to use an actual running average/median for the threshold, so I'll open an issue for that and hopefully do it soon.  

### 4/29/2017

Forked!
Threshold is now a pseudo running average instead of just using an arbitrary value. I'm sure this can be improved upon. I'm not used to programming things this difficult. LOL  
Also made thresholds specific to each pin. Otherwise you just wouldn't see any treble lights in a bassy song.  