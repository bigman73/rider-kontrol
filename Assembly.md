# Assembly

The `Rider Kontrol` unit is composed of the following main components:

1. Enclosure - for the prototype version an existing enclosure 3D model design was used, by [BarButtons](https://jaxeadv.com/barbuttons/)
2. Hardware - Buttons, LED, USB Cable
3. Micro controller - Lolin C3 Mini v2.1, using ESP32 architecture
4. Firmware - The Rider Kontrol embedded software that runs the micro controller

## Important note!

The `BarButtons` firmware and button mounting design were not used. Rider Kontrol takes a very different approach, and its firmware was written from scratch.
In Rider Kontrol, each button is connected to its own GPIO pin. BarButtons uses a button matrix instead. That added complexity isn’t necessary here, because the Lolin C3 Mini provides enough GPIO pins.
Furthermore, the Rider Kontrol utilizes more memory (due to libraries) and therefore uses a larger partition size.
Do not follow the BarButtons mounting instructions. You can use their build page for reference only, but be aware of the incompatibilities in both hardware and software.

# Components

## Enclosure

The `3D Model` provided by BarButtons is contained in a zip file. Download and extract it to a folder:
[Enclousre body](https://jaxeadv.com/barbuttons-files/barbuttons-3d-files.zip)

There are 5 parts to the enclosure.

<img src="https://raw.githubusercontent.com/bigman73/rider-kontrol/main/assets/rider-kontrol-assembled.jpeg" alt="Assembled Unit" width="300" />

### Enclosure Body Front

Path: `navibox 2.7.10/nb-2.7.10_Navibox v2.7 v10_Front.3mf`
The front of the enclosure that has mounting holes for the buttons and LED

### Enclosure Body Back

Path: `navibox 2.7.10/nb-2.7.10_Navibox v2.7 v10_Back.3mf`
The back of the enclosure that contains the micro controller and connects to the spacer or handlebar bracket.

### Front Handlebar Bracket

Path: `navibox 2.7.10/nb-2.7.10_Navibox v2.7 v10_Bracket.3mf`
A semi circle handle bracket that connects the back handle bracket to the enclosure body back, or the spacer (if used)

### Back Handlebar Bracket

Path: `navibox 2.7.10/nb-2.7.10_Navibox v2.7 v10_Handlebar adapter.3mf`
A semi circle handle bracket that is mounting the enclosure on the motorcycle handle bar. It is the back with respect to the enclosure.

### Spacer

Path: `navibox 2.7.10/nb-2.7.10_Navibox v2.7 v10_Spacer.3mf`
A 5mm spacer that enables the enclosure to move closer to the rider and clear any conflict with other handlebar component (typically the base of the left mirror)

The spacer is optional, but can also be required for some motorcycle models, and with some 2 spacers might be needed (e.g, Husqvarna 701 Enduro/Supermoto).

> ⚠️ Any modification to the number of spacers used would require different length of handle bar bolts

## 3D Print

Print all parts with a high quality 3D printer.
After printing assemble the enclosure and try to "dry" mount it on the motorcycle for fitness. Use sand paper as needed to smoothen the surface of the body front and back parts.
Start with printing 1 spacer, print another if needed.

For the 4 enclosure screws use a T8 screwdriver.

## Mount hardware

Mount all 8 buttons and LED. Make sure they fit in the slot properly and hand tighten the nuts in the back of each button and the LED.

> ⚠️ Do not over tighten the nuts or the component might be damaged!

## Microcontroller

The firmware needs to be uploaded to the Lolin C3 Mini microcontroller.

See the [firmware readme](firmware/README.md) for instructions

## Physical Connections / Soldering ⚡️

wires need to be soldered from each button and LED leg to the right GPIO pin on the Lolin micrcontroller.
One side of the USB cable needs to be cut and stripped. The red and black wires should be soldered to VBUS and GND respectively. There is no usage for the green and white USB wires, cut them off.

| From                               | To                             | Description                     |
| ---------------------------------- | ------------------------------ | ------------------------------- |
| B1 negative leg (any [1])          | B2 negative leg (any [1])      | Button negative bus             |
| B2 negative leg                    | B3 negative leg (any [1], [3]) | Button negative bus             |
| B3 negative leg [3]                | B4 negative leg (any [1])      | Button negative bus             |
| B3 negative leg [3]                | B8 negative leg (any [1])      | Button negative bus             |
| B8 negative leg                    | B5 negative leg (any [1])      | Button negative bus             |
| B5 negative leg                    | B6 negative leg (any [1])      | Button negative bus             |
| B6 negative leg                    | B7 negative leg (any [1])      | Button negative bus             |
| B6 negative leg                    | C3 GND                         | Negative bus connection to C3   |
| USB GND (Black wire) [4]           | C3 GND PIN                     | External power source GND (0V)  |
| USB Power (Red wire) [4]           | C3 Vbus PIN                    | External power source Vbus (5V) |
| B1 positive leg [2]                | GPIO Pin #0                    | Button 1 signal                 |
| B2 positive leg [2]                | GPIO Pin #1                    | Button 2 signal                 |
| B3 positive leg [2]                | GPIO Pin #2                    | Button 3 signal                 |
| B4 positive leg [2]                | GPIO Pin #3                    | Button 4 signal                 |
| B5 positive leg [2]                | GPIO Pin #4                    | Button 5 signal                 |
| B6 positive leg [2]                | GPIO Pin #5                    | Button 6 signal                 |
| B7 positive leg [2]                | GPIO Pin #6                    | Button 7 signal                 |
| B8 positive leg [2]                | GPIO Pin #8                    | Button 8 signal                 |
| External LED Negative wire (black) | C3 GND                         | External LED ground             |
| External LED Positive wire (red)   | GPIO Pin #10                   | External LED signal             |
