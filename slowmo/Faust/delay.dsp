declare id "delay";
declare name "Delay";
declare category "Echo / Delay";

import("stdfaust.lib");

msec	= ma.SR/1000.0;
interp  = 100*msec;
N       = int( 2^19);
gain    = vslider("gain[name:Gain]", 0, -20, 20, 0.1) : ba.db2linear : si.smooth(0.999);
d       = ba.tempo(hslider("bpm[name:BPM][tooltip:Delay in Beats per Minute]",120,24,360,1));
feedback = vslider("feedback[name:feedback]", 0.5, 0, 1, 0.01) : si.smooth(0.999);

process = _ <: _ + gain * (+:de.sdelay(N, interp,d))~(de.sdelay(N, interp,d)*(feedback)) :> _;
