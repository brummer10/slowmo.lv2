declare id "gain";
declare name "Gain";
declare category "Tone Control";

import("stdfaust.lib");

gain = vslider("Volume[name:Volume][tooltip:gain (dB)]", 0, -20, 20, 0.1) : ba.db2linear : si.smooth(0.999);
process = *(gain);
