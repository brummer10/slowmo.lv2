declare id "bandsplit";
declare name "Bandsplit";
declare category "Filter";

import("stdfaust.lib");

geq = fi.filterbank(3, (51.0, 160.0, 284.5, 427.0, 590.5, 778.0, 992.5, 1238.0, 1519.0, 1841.0, 2210.0, 2632.0, 3115.0, 3668.5, 4302.5, 5028.5, 5859.5));

process = geq;
