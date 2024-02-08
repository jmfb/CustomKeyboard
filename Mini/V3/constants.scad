keySize = 19;				// Size of a keyswitch (a perfect square)
halfKeySize = keySize / 2;	// Distance from center of keyswitch to each side
keyHoleSize = 14;			// Size of a keyswitch mounting hole (a perfect square)
epsilon = 0.1;				// Small value used to get around OpenScad difference issues

pcbDepth = 1.6;				// Thickness of the PCB (average measured value)
modelWallDepth = 1.5;		// Thickness of the vertical walls of the model
mountingPlateDepth = 1.5;	// Thickness of the mounting plate layer (needed for keys to click in place)

keyPcbWallSpacing = 0.5;	// Total gap between PCB and mounting walls (half on each side)
keyPcbXOffset = (modelWallDepth + keyPcbWallSpacing) / 2;	// Offset in x-direction for PCB
keyPcbWidth = keySize - modelWallDepth - keyPcbWallSpacing;	// Width of a key PCB baord
keyPcbHeight = keySize;		// Height of a key PCB board (normal keysize)

pcbMountSpacing = 4;	// Measured distance between bottom of mounting plate and top of PCB
mountingHoleOffset = (keySize - keyHoleSize) / 2;	// Distinct from the edge of a mounting plate location to the edge of the hole

lowerRowAngle = atan2(3, 4);	// Angle of row below home row (empirically determined)
upperRowAngle = atan2(6, 11);	// Angle of row above home row (empirically determined)

pcbBottomSpaceUnderHome = 4;		// Distance below home row PCB before bend
pcbBottomSpaceAboveHome = 3;		// Distance above home row PCB before bend
pcbBottomSpaceAfterLowerBend = 7;	// Distance after lower bend before bottom row PCB
pcbBottomSpaceAfterUpperBend = 5;	// Distance after upper bend before upper row PCB

// Offsets of fingers on home row relative to index finger
middleYOffset = 4;	// Vertical shift of middle finger
ringYOffset = 0;	// None
pinkyYOffset = -4;	// Vertical shift of pinky finger
middleZOffset = -1;	// Z-offset of middle finger
ringZOffset = -2;	// Z-offset of ring finger
pinkyZOffset = 6;	// Z-offset of pinky finger

// Extensions of mounting plate to meet at home row bends
distanceToMountingPlateBottom = pcbMountSpacing + pcbDepth;	// Distance from bottom of PCB to bottom of mounting plate
// Amount the bottom row mounting plate extends to meet the home row
bottomRowToHomeExtension = (
	pcbBottomSpaceAfterLowerBend * sin(lowerRowAngle) +
	distanceToMountingPlateBottom * cos(lowerRowAngle) -
	distanceToMountingPlateBottom
) / sin(lowerRowAngle);
// Amount the home row mounting plate extends to meet the bottom row
homeToBottomRowExtension =
	pcbBottomSpaceUnderHome +
	pcbBottomSpaceAfterLowerBend * cos(lowerRowAngle) -
	distanceToMountingPlateBottom * sin(lowerRowAngle) -
	bottomRowToHomeExtension * cos(lowerRowAngle);
// Amount the top row mounting plate extends to meet the home row
topRowToHomeExtension = (
	pcbBottomSpaceAfterUpperBend * sin(upperRowAngle) +
	distanceToMountingPlateBottom * cos(upperRowAngle) -
	distanceToMountingPlateBottom
) / sin(upperRowAngle);
// Amount the home row mounting plate extends to meet the top row
homeToTopRowExtension =
	pcbBottomSpaceAboveHome +
	pcbBottomSpaceAfterUpperBend * cos(upperRowAngle) -
	distanceToMountingPlateBottom * sin(upperRowAngle) -
	topRowToHomeExtension * cos(upperRowAngle);

extraRowExtension = 5;	// Arbitrary extra extension past top and bottom rows
minDepthBelowPcb = 5;	// Extra spacing below PCB to make room for wiring, etc.
