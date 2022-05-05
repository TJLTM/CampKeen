# CampKeen
An OpenSource RV monitoring hardware project. Currently i'm targeting my setup but leaving enough to adapt to other systems. This will allow a central place to monitor all of the various things in an RV and will be able to be integrated with an external system like Homeassistant (when I get to that it will have a separate Repo for the component) or custom setups. As well control some things around the RV. Like in this case, i have small holding tanks and to keep them from overflowing i have added waterpump and city (external water hook up control) in both the bathroom and in the kitchen. This does not have a wifi or ethernet capability. This is RS232 or USB (via the MEGA2560) device only. 

What this can monitor: 
1. Holding Tanks - Grey/Sewage via galvanic sensors (posts in the tank) these are the ones i'm using (https://www.amazon.com/gp/product/B07YVK2V2K/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1 and https://www.amazon.com/gp/product/B07SVDTHHG/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1)
2. Fresh water tank - Resistance (https://www.amazon.com/gp/product/B00Y831WPM/ref=ppx_yo_dt_b_asin_title_o07_s01?ie=UTF8&th=1&psc=1)
3. Camper Battery Voltage
4. Power/Energy Monitoring (https://github.com/CircuitSetup/Split-Single-Phase-Energy-Meter)
5. Tempetures around the RV especially and Fridge and Freezer (https://www.amazon.com/gp/product/B087NMHGX8/ref=ppx_yo_dt_b_asin_title_o07_s01?ie=UTF8&psc=1)
6. Generator Temps (https://www.amazon.com/gp/product/B071DVVZHC/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1) 
7. Generator Fuel Pressure (https://www.amazon.com/gp/product/B07N8SX347/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&th=1 becareful of these they may or may not be DOA or last only a few weeks depending i haven't found a good source that isn't a crap shoot) 
8. LPG tank level - No link as this is the original tank and sensor that came with the 1978 Pace Arrow it's resistence based but single wire it is similar to https://parts.unitedrv.com/products/remote-ohm-sender-lp-gas-tank-90-degree?currency=USD&variant=36385562881&_vsrefdom=adwords&msclkid=ae7c158e8c431c0739b97415690580d9&utm_source=bing&utm_medium=cpc&utm_campaign=**LP%20Shopping%20-%20Catch%20All&utm_term=4583245501779595&utm_content=Catch%20All

What this can control:
1. WaterPump
2. City Water Valve (https://www.amazon.com/gp/product/B07N6246YB/ref=ppx_yo_dt_b_asin_title_o05_s00?ie=UTF8&th=1)
I do this with push buttons (two one in the kitchen and one in the bathroom for my set up) with LEDs to let you know that the water is on. Be it the City Water or Tank Via the pump. 
i have also included a timer. that can be set for how long you would ike the water to remain on when either button is pressed.


Other Stuff
1. There are 6 spare outputs that can be used to control whatever you want 
2. Same goes for 6 spare inputs 
3. I2C output for an 20x4 LCD to display current status (https://www.amazon.com/gp/product/B07MTFDHXZ/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1)
4. Siren to alert when the tanks are full
5. Warning LED (This and the siren can be programmed for other items for warnings) 
6. 2 Spare Buffered ADCs with POTs 

Some Specs and Installation Notes: 
1. I designed this for 12Vdc systems only so i'm not sure if it would work on a 24Vdc system might. might not. 
2. The Inputs are buffered and you can just supply 12Vdc in as a logical 'On' 
3. Outputs, Althought technically you could power the waterpump directly off of the unit. this is not recommended. nor encouraged in any way, shape or form. These outputs are used to trigger remove 12Vdc relays like the following: https://www.summitracing.com/parts/pco-5591pt

currently working on https://github.com/TJLTM/CampKeen-HomeAssistant-Custom-Integration this may roll into a true python package later on to handle communication 


FUTURE: 
Better docs with installatioin pictures for my setup. 

![alt text](https://github.com/TJLTM/CampKeen/blob/master/Docs/Pictures/3DR1.PNG)
![alt text](https://github.com/TJLTM/CampKeen/blob/master/Docs/Pictures/3Dr2.PNG)
