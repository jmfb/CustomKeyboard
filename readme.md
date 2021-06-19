# Custom Keyboards

## V1

I took a prefabricated tenkeyless PCB and used a dremel to cut it into 3 pieces:
- Left hand board (jagged cut at F5, 6, T, G, B)
- Right hand board (jagged cut at F6, 7, Y, H, N)
- Arrow keys

I proceeded to use the dremel to sever all PCB traces for the keys but left the diode traces.
Cherry MX brown switches were then directly mounted to the PCB.  Note that these switches were
the ones designed to be plate mounted (did not include the extra two plastic pegs for direct PCB mounting) so the keys ended up looking crooked.

The switches were then wired into the standard row/column matrix and wires were all routed through to an external Teensy2.0 controller.  2x5 and 2x3 headers were used for the connecting cables.  The keycaps were a standard profile blank set.

The next phase was designing a custom PCB for the number pad (to "complete" the full keyboard) and testing out both 3D printed and lasercut mounting plates.

### Lessons Learned

- Direct PCB mounting was crooked (at least using the non-PCB mount keys).
- Lasercut acrylic mounting plates were sturdier, cheaper, and looked cooler than the 3D printed versions.
- Arrow keys and number pad could be combined into a single board.
- Blank keycaps were difficult to use (I guess I still occasionally look).
- The placement of the right hand spacebar required the right thumb to tuck.
- The external controller for the Teensy and the external cable management boards made for too many components.
- The keys come with holes for LEDs that I might as well be using.
- A wristpad was necessary to use the keyboard and would nice if it were affixed.

## V2

I designed 3 custom PCBs using KiCad:
- Left hand board (capslock key replaced with Teensy2.0 controller)
- Right hand board
- Accessory board (arrows and numbpad)

I designed lasercut mounting plates using OpenSCAD.

I upgraded to using Cherry MX blue switches and double shot DCS keycaps and installed green LEDs on each key.

I extended the base plate to make room to glue wristpads onto the boards.

### Lessons Learned

- Direct soldering of the LEDs to the PCB made it difficult to replace them when they loosened or burned out.
- The double shot keycaps felt great but didn't allow for the LED light to show through.  Translucent keycaps would have been better.
- A better solution than just gluing the PCB to a board would be needed for a case (someting that could be assembled/disassembled, and that had walls).

## V3

This design used the same PCBs and mounting plates as V2.

Cherry MX green switches were used.  After using a sampler board with several types of switches I thought the ultra stiff key would be awesome.

Lasercut faceplates, lasercut baseplates, and 3D printed case walls were designed using OpenSCAD.  Three versions of the case walls were printed to test out different materials and different 3D printing vendors: i.Materialise, Sculpteo, and Shapeways.

SIP LED sockets were put into each of the key switches to allow for the LEDs to be replaced.  A mix of blue and green LEDs were used (blue for control keys).

Doubleshot translucent keycaps were used to allow for the LED light to show through.

The controller logic was updated to make the left hand space be a hyper key.

### Lessons Learned

- The 3D walls designed for the case were too tight of a fit and bowed out.  It made for a sturdy design but required too precise measurements for a perfect fit.  The next case should use screws/standoffs to mount the PCB vs. grooved walls.
- 3D printing was too expensive and slow compared to laser cutting.  The next case should be made solely from lasercut parts.
- The hyper key functionality was great, but I occassionally missed having the left hand spacebar.  More keys next time.
- The 2x5/2x3 connector cables looked unprofessional (probably because of the rainbow colored cables).
- I measured the baseplates based on the previous wristpad size and when I couldn't reorder the same type of wristpad had to use bigger ones that hung over the edge of the baseplates.  Obtain wristpads prior to measuring baseplates.
- The cutoff edge of the wristpads looked unprofessional.
- After double checking the PCB and laser cutting capabilities there is no need to keep the accessory board separate.
- Stabilizers for the 2+ keys would be nice.  Need to update PCB for holes, mounting plates for holes, and move PCB components in the way of the footprints.
- Not sure how I feel about the green switches.  I like the stiff feel but I don't think I love them like I did the blues.
- The LEDs were mounted on the bottom of the keys but the translucent keycaps were designed to shine through the top.  Will need to flip key switches to optimize translucent effects of the keycaps.  Note that vertical keys should have LEDs to the left.
- The key spacing for larger unit keys (1.25, 1.5, 1.75, 2, 2.25, and 2.75) had been using measured positions and worked.  However, when attempting to make a perfect rectangle for each hand it was discovered that 19mm * keysize was needed to prevent 1mm error for several rows.  This increased resolution for key spacing to 1/8th mm from 1/2 mm.

## V4

3 new PCBs were designed using KiCad:
- Extended left hand board (include arrow keys and slightly inverted numpad to mirror right hand board).
- Extended right hand board (combined right hand board and accessory board).
- External connector board (houses Teensy and 2 25-pin D-sub connectors).

The left hand board had arrow keys added to the left of the previous left edge.  A numpad was added to the left of that but with the outside edge of keys mirrored and the zero and period flipped.  The mirroring was so the enter would still be hit with the pinky and the zero still with the thumb.  The numbers were not flipped because I didn't want to retrain my brain for my left hand.  Above the numpad, four programmable buttons were added (N1-4, sometimes used for volume controls or extended operators for the numpad).  To the left of the numpad a vertical row of programmable buttons was added (M1-6, as part of the more keys strategy) that used some of the gaps in the row/column matrix.  To the right of the jagged edge, a vertical row of programmable keys was added (L1-6) and different sized keys were used to make the board a perfect rectangle.  A new hyper key (LEFT_HYPER) was also added to the left of the spacebar to fill in the gap between alt and the spacebar.  The capslock key was re-added since the Teensy is being moved onto its own board (whether this gets remapped is programmable).  The main part of this board is 8.5 standard keys wide.

The right hand board was combined with the previous accessory board.  The same jagged gap was filled on this board to make it a perfect rectangle though one more key was added (two keys were added to the left of the function key row) for an additional 7 keys (R1-7).  A new hyper key was added to the right of the spacebar (RIGHT_HYPER) and the right windows key was re-added (previously just had the right menu key between alt and ctrl).  The same N1-4 and M1-6 keys were also added to this board (note they are not independently programmable because they map to the same row/column scan position).  The main part of this board is 10 standard keys wide.

The full keyboard contains 170 keys; 130 distinct row/column scan positions.  Note that there are still 2 unused row/scan positions in the 12x11 row/scan matrix.  There wasn't anywhere convenient left to put them though.

A single 25-pin D-sub connector exists on each board. 12 row pins, 11 column pins, VCC, and GND (for LEDs).  Instead of chaining the left board to the right board and having the USB cable and connector cable on the left board, the Teensy was moved off onto a controller board.  The controller board just maps the Teensy pins to 2 D-sub connectors (one for the left hand board and one for the right hand board, with both cables having the same pin assignments).

The case design was switched to use all lasercut pieces.  The walls are notched together and slide pegs into the baseplate and faceplate.  A 3mm overhang, 3mm wall depth, and 1mm spacing between wall and PCB makes each dimension of the base/face 14mm longer than the PCB/mounting plate sizes.  Screw holes are drilled into the border and one near the arrow keys for some center support.  M2 screws (5mm male end), 0.5mm washers, 5mm M/F standoffs (3mm wide with 4mm male end), and 8mm F/F standoffs (3mm wide) are used to affix the PCB to the base and affix the faceplate to the walls.  From the bottom, it goes: screw, base, 5mm M/F standoff, PCB, washer, 8mm F/F standoff (though hole for mounting plate), 2 washers, faceplate, washer, screw.

All case parts for all three boards can be combined into a single CAD file and lasercut from two P3 pieces from Ponoko (one for the baseplates and walls and one for the mounting plates and faceplates).  Since there was space for two connector faceplates, I decided to make one with a little bit of artwork and cut out a flux capacitor.  Because if you're going to build your own keyboard why not build one with style!

Cherry PCB mounted stabilizers were added.  Two sets of translucent keycaps were needs due to the number of additional keys.

### Lessons Learned

- D-Sub connector height measurement was imprecise and pushed the faceplate up about 1mm.  Filing it down took part of the edge off, but using a micrometer will yield better wall/faceplate height measurements.
- Using the washers on the screw/standoff connections was impractical due to the small size of the washers, the inconvenient location of the screws (near walls or central to the board and under the mounting plate).  The screw/standoff premise is solid though and made a very sturdy and professional looking case.
- The location of the D-Sub connectors on the boards was inconvenient for placement near monitors.  I think the outside edge would make the most sense for the next version.
- More keys was a bad idea.  The inner row keys ended up getting pressed by accident too often and ruined the tactile feel for finding my position on the keyboard without looking.  While there was potential for adding useful keys (left hand enter, backspace, right hand tab, escape) the opposite was experienced.  The extra left hand board made it difficult to find home for my left hand (naturally always seeking leftmost position).  The extra numpad and outer macro keys filled out the keyboard but were too out of the way to add anything useful to them (it was more burdensome to try to figure out what to assign to the keys).  I am not sure about how I feel about the hyper key near the spacebar because of the other things that made typing difficult.  I do still think a hyperkey or two is a good idea.
- The offboard teensy and extra cabling has the benefit of keeping a single cable going to each board but uncovered a problem with wiring resistance that was solved with a delay in the controller logic during rowscan to allow the signals in the lines to stabilize (which was taking at least one extra clock cycle and causing phantom keypresses in the first two columns of the scan matrix).
- I didn't glue down the wristpad on this one because I was tired of buying more and more wristpads.  However, they slip around too much otherwise.  They need to be fixed to the keyboard case.  Sewing the edge flap down did make them look much better than the glued end of cut off end.
- I think the Cherry MX Green switches were cool for a while but in the end not as nice to type on.  Going back to the blue switches from V2 felt amazingly good by comparison.
- The mix of blue and green LEDs was kind of cool but I don't love it.  I think short of RGB LEDs I will stick with a single color.  Probably green but blue was fine too.  The SIP sockets rock.
- Having the CapsLock key back was a mistake - definitely never use it and every time it is pressed was an accidental hit.  I might leave it off next time.  Would the overall look suffer though?
- The flux capacitor design in the connector board was basically invisible because of the clear faceplate.  However, the top leg did provide a hole through to the teensy reset button which came in handy.  Might just keep that hole.
- The cherry stabilizers worked out great.  Clean fit into PCB, perfect cutout for mounting plate.  There was one resistor that was close to a support bar.
- Pro-tip: prior to soldering the keys onto the PCB make sure all key pins went through.  One pin had bent down and I didn't discover it until half of the keys were soldered.  That meant I was almost fucked.  I ended up shoving a bit of wire through the hole and drenching it in solder hoping that surface tension would pull enough solder down the wire to make the connection to the bent pin.  It worked, but had it not I don't know what I would have done.
- A few more screws in certain locations would make the faceplate more stable (at least on the right hand board).

## V5

### Left Hand

- Getting rid of left hand accessory board (arrows and numpad).
- Getting rid of outside column macro keys (M1-6) and inside column macro keys (L1-6).
- Removing the caps lock key.  Could have left it for looks and mapped it to a hyper key or something, but not worth the mistakes.
- Keeping the left hyper key.
- Moving D-Sub connector to left edge at the top.
- Using a total of 7 screw holes to build case.
- Total of 35 keys (2 2+ keys) with max row width being 7.5 keys (bottom row).

### Right Hand

- Getting rid of outside column macro keys (M1-6) and inside column macro keys (R1-7).
- Keeping numpad macro keys (N1-4).
- Keeping right hyper key and right windows key.
- Moving D-Sub connector to right edge at the top.
- Using a total of 15 screw holes to build case.
- Total of 75 keys (7 2+ keys) with max row width being 9 keys (bottom row, excluding accessory keys).

### Connector Board

- Reusing design from V4.  Will be able to use the same PCB.
- Leaving a hole in the faceplate for the Teensy reset button.
- Using a total of 4 screw holes to build case.

### General Updates

- Fixing case wall height to take into account micrometer measurements for D-Sub connectors (10.3mm vs. previous measurement of 9.5mm).
- Using 10mm F/F standoffs to connect PCB to faceplate (a 0.3mm gap will exist, should be fine).
- Not using washers in any part of screw assembly (too small and can't get in most spots).
- Using same color LED for all keys (green).
- Going back to Cherry MX Blue switches.
- Plan on getting custom DCS row 6 keycaps for hyper keys (left is 1.5, right is 1.75) and space keys (both 2.25).

### Lessons Learned

- This keyboard is just plain awesome.
- Right hand board is still very large and is sometimes awkward to fit onto desk near monitors.
- Large right hand board occasionally gets wobble when crossing desk boundaries.
- Very minor use of hyper keys due to completely normal keyboard layout.
- N1-4 keys are never used (and completely out of the way).
- Uses 3 2.25 keys (left shift and both spacebars) which makes complete keycap sets difficult.
- Using unused capslock as right hyper is perfect.
- Still do not miss the capslock key (nor do I wish there were a key there I could hit).
- Single color keyboard is okay (looks neat but not awesome).  Have built both a green and blue version.
- Ran out of blue switches for second one and used green for function keys - don't even notice because of how rarely they are used.
- D-sub cables give a classic/punk look to the keyboard but are a bit large (bump into monitors still, even on the outside top).
- Cost for materials (case/PCB) is higher than ideal due to large right hand board.
- Connector board is larger than it needs to be.

## Mini V1

### Goals

- Build a miniature keyboard (40%-like) and use hyper-functionality to improve current coding workflows.
- Have an easy to hit underscore (for snake casing, lambdas, garbage identifier prefixes, etc.).
- Reduce PCB/case material costs (just in case I want two of them again!).
- Have smaller and more hidden cables connecting the boards (keep off-board controller for symmetry).
- Columnar layout (will definitely need to re-learn typing).
- Minimal "reach" keys (1 row up, 1 row down, 1 row over to either side).
- Rely on thumb buttons for extra keys (no clue what layout yet).
- RGB LEDs so I can have that awesome feel of dynamic color and responsive changes.

### Parts, Prices, and Datasheets

- Cherry MX Blue RGB PCB Mount Switches (x23 x2)
	- MX1A-EN1B (NOTE: Only wanted MX1A-EN1A, plate mounted, but were constantly out of stock)
	- [Blue Datasheet](https://www.cherrymx.de/_Resources/Persistent/a531cb6598bc849cbcf131fd7a31814282b74545/EN_CHERRY_MX_BLUE.pdf)
	- [Footprint Datasheet](https://cdn.sparkfun.com/datasheets/Components/Switches/MX%20Series.pdf)
	- [LED Cutout Datasheet](https://drive.google.com/file/d/0B5JcED9YINdnNU1qajJmVmtEcUU/view?resourcekey=0-LZfcyjo3jcX8JEKJD6tRKA)
	- [MechanicalKeyboards](https://mechanicalkeyboards.com/shop/index.php?l=product_detail&p=4451)
	- $0.50 / switch @ 100 = $50.00 (bulk discount)
- Keyswitch Diode (x23 x2)
	- 1N4148 (1N4148FS-ND)
	- [Datasheet](https://rocelec.widen.net/view/pdf/bhc41r7lwc/ONSM-S-A0003539415-1.pdf?t.download=true&u=5oefqw)
	- [Digikey](https://www.digikey.com/en/products/detail/on-semiconductor/1N4148/458603?s=N4IgTCBcDaIIwDkAsckA4BiBlAtAgIiALoC%2BQA)
	- $0.01622 / diode @ 1000 = $16.22 (bulk discount)
- RGB LED with IC (x23 x2)
	- APA102C (iPixel LED)
	- [Datasheet](https://cdn.sparkfun.com/datasheets/Components/LED/APA102C.pdf)
	- [Sparkfun](https://www.sparkfun.com/products/16345)
	- $0.379 / LED @ 50 = $18.95
- 9pin Mini-DIM Female Connector, Through Hole, Right Angle (x1 x2 + connector x2)
	- MD-90S (CP-2490-ND)
	- [Datasheet](https://www.cuidevices.com/product/resource/md-s.pdf)
	- [Digikey](https://www.digikey.com/en/products/detail/cui-devices/MD-90S/145320)
	- $2.95 / connector @ 4 = $10.80
- 9pin Mini-DIM Male/Make 3ft. Cable (x2)
	- MD9MM3BLK
	- [Datasheet](https://www.kraydad.com/mini-din-9-pin-b-type-cables/)
	- [Kraydad](https://www.kraydad.com/mini-din-9-pin-b-type-3-ft-male-to-male-black-cable/)
	- $12.49 / cable @ 2 = $24.98
- Cherry PCB Snap-in Stabilizers (2u x2 x2)
	- 60%/TKL (NOTE: includes 5 2u wires, only need 4)
	- [Amazon](https://www.amazon.com/Genuine-Cherry-PCB-Snap-Stabilizers/dp/B085T8QZJC)
	- $14.99 / pack @ 1 = $14.99
- M4 Hex Brass F/M Standoff 18mm/6mm w/ Screw and Nut (x4 x2)
	- Set of 20 (need 4 or 5 per board)
	- [Amazon](https://www.amazon.com/gp/product/B08ZXSR7MP)
	- $0.5745 / set @ 20 = $11.49
- M4 Brass Washer 9mm OD, 4.3mm ID, 0.8mm Thick (x4 x2)
	- Pack of 100 (need 4 or 5 per board)
	- [Amazon](https://www.amazon.com/gp/product/B009OK2KPU)
	- $0.0827 / washer @ 100 = $8.27
- Clear Soft Close Drawer Cabinet Door Bumpers (x6 x2 + connector x4)
	- 9.5mm x 3.8mm, 72pc sheet
	- [Amazon](https://www.amazon.com/dp/B06XCGM8JT)
	- $0.0968 / bumper @ 72 = $6.97
- Mini USB A to Mini-B Cable, Black, 3ft (x1)
	- 103896 (any Mini USB cable will do if you already have one)
	- [Amazon](https://www.amazon.com/Monoprice-3-Feet-mini-B-28AWG-103896/dp/B003L18SHC)
	- $5.23 / cable @ 1 = $5.23
- Teensy 2.0 (x1)
	- No pins
	- [Datasheet](https://www.pjrc.com/store/teensy.html)
	- [Amazon](https://www.amazon.com/gp/product/B00NC43256)
	- $18.75 / chip @ 1 = $18.75 ($16 from PJRC but with shipping)
	- NOTE: Plans to discontinue this model in 2022; recommend switching to 32-bit boards.
- Header Pins (x12 x2, though more pins available on Teensy)
	- Breakaway header, straight, brass
	- [Sparkfun](https://www.sparkfun.com/products/116)
	- $1.50 / 40pin strip @ 1 = $1.50
	- NOTE: Larger volumes and lower per-pin cost are on amazon but not brass pins
- Demultiplexer, 3:8 (x1 x2)
	- SN74HC138N (296-1575-5-ND)
	- [Datasheet](https://www.ti.com/lit/ds/scls107f/scls107f.pdf)
	- [Digikey](https://www.digikey.com/en/products/detail/texas-instruments/SN74HC138N/277221)
	- $0.461 / chip @ 10 = $4.61 (bulk discount)
- Parallel Load, Shift Register (x1 x2)
	- SN74HC165N (296-8251-5-ND)
	- [Datasheet](https://www.ti.com/lit/ds/symlink/sn74hc165.pdf)
	- [Digikey](https://www.digikey.com/en/products/detail/texas-instruments/SN74HC165N/376966)
	- $0.525 / chip @ 10 = $5.25 (bulk discount)
- 10kOhm Pullup Resistors (x8 x2)
	- CF18JT10K0 (CF18JT10K0CT-ND - Cut Tape (CT))
	- [Datasheet](https://www.seielect.com/catalog/sei-cf_cfm.pdf)
	- [Digikey](https://www.digikey.com/en/products/detail/stackpole-electronics-inc/CF18JT10K0/1741566)
	- $0.01116 / resistor @ 500 = $5.58 (bulk discount)
- Keycaps
	- TODO: Doubleshot Translucent DCS for main set, unsure for extra non-standard keys
- Laser Cut Parts
	- TODO: Ponoko
- PCB
	- TODO: Seeed Studio
