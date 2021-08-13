keySize = 19;				// Size of keyswitch (a perfect square)
halfKeySize = keySize / 2;	// Distance from center of key to edge of keyswitch

keyHoleSize = 14;					// Mounting plate keyswitch hole size (a perfect square)
halfKeyHoleSize = keyHoleSize / 2;	// Distance from center of key to edge of hole

twoKeySize = keySize * 2;			// Long length of a 2-key switch (short length is 1-key size)
halfTwoKeySize = twoKeySize / 2;	// Half the long length of a 2-key switch

keyHolePadding = halfKeySize - halfKeyHoleSize;			// Distance from mounting hole to edge of keyswitch
twoKeyHolePadding = halfTwoKeySize - halfKeyHoleSize;	// Distance from mounting hole to distant edge of keyswitch

basePlateDepth = 3;			// Base plate depth (bottom layer)
wallLayerDepth = 3;			// Wall layer depth (spacing layer)
mountingPlateDepth = 1.5;	// Mounting plate depth (keyswitch mounts)
pcbDepth = 1.6;				// PCB depth (circuit board, not connected to walls)
facePlateDepth = 1.5;		// Face plate depth (top layer)

rimSize = 3;						// Size of the wall layer rim
pcbSpacing = 1;						// Space between the PCB and the rim
wallSpacing = pcbSpacing + rimSize;	// Space between edge of keyswitch and edge of case
extraBottomSpacing = 3;				// Extra spacing at the bottom to allow clearance for thumb screw

connectorEdgeSize = 14;	// Width of the connector on the edge of the PCB
connectorLength = 12.3;	// Distance from edge of PCB to back of connector
connectorOffset = 42;	// Distance from edge of PCB to portion of cable at outer edge
connectorLeft = connectorOffset - wallSpacing; // Distance to edge of base next to connector
connectorPadding = 0.5;	// Distance from edge of connector to mounting plate/face plate (prevent rubbing)

thumbGridOffset = 7;			// Shift back from innermost finger column
middleFingerOffset = 4;			// Shift up from index and ring finger rows
pinkyOffset = 4;				// Shift down from ringer finger row
pinkyExtraOffset = halfKeySize;	// Shift down from pinky finger row

wristPadHeight = 100;	// Height of the wrist pad (Glorious PC gaming model)
wristPadWidth = 6 * keySize - thumbGridOffset + pcbSpacing + rimSize;
wristPadCornerRadius = 7;	// Radius of the thumb side bottom coner (only one rounded since other is cut)

hexHeight = 6;		// Measured height (flat to flat) of M4 hex standoff
hexPadding = 0;		// Padding between side and cutout (not too snug it gets stuck)
screwDiameter = 4; 	// Diameter of screw threads (with sufficient padding)

washerDiameter = 9;									// Diameter of the M4 washers
washerSpacing = 2;								// Spacing between edge of washer and edge of case
washerSize = washerDiameter + 2 * washerSpacing;	// Size of washer plus spacing
washerRadius = washerSize / 2;						// Radius of washer plus spacing

middleFingerTop = wallSpacing;
indexFingerTop = middleFingerTop + middleFingerOffset;
indexExtraTop = indexFingerTop;
ringFingerTop = middleFingerTop + middleFingerOffset;
pinkyFingerTop = ringFingerTop + pinkyOffset;
pinkyExtraTop = pinkyFingerTop + pinkyExtraOffset;
thumbGridTop = indexExtraTop + 3 * keySize;

// Left/right nomenclature for left hand board positioning (mirrored for right hand board).
pinkyExtraLeft = wallSpacing;
pinkyFingerLeft = pinkyExtraLeft + keySize;
ringFingerLeft = pinkyFingerLeft + keySize;
middleFingerLeft = ringFingerLeft + keySize;
indexFingerLeft = middleFingerLeft + keySize;
indexExtraLeft = indexFingerLeft + keySize;
indexExtraRight = indexExtraLeft + keySize;
thumbGridLeft = indexFingerLeft - thumbGridOffset;

// x, y, virtical count
pinkyExtra = [pinkyExtraLeft, pinkyExtraTop, 2];
pinkyFinger = [pinkyFingerLeft, pinkyFingerTop, 3];
ringFinger = [ringFingerLeft, ringFingerTop, 3];
middleFinger = [middleFingerLeft, middleFingerTop, 3];
indexFinger = [indexFingerLeft, indexFingerTop, 3];
indexExtra = [indexExtraLeft, indexExtraTop, 3];
thumbGrid1 = [thumbGridLeft, thumbGridTop, 2];
thumbGrid2 = [thumbGridLeft + keySize, thumbGridTop, 2];

fingerColumns = [
	pinkyExtra,
	pinkyFinger,
	ringFinger,
	middleFinger,
	indexFinger,
	indexExtra,
	thumbGrid1,
	thumbGrid2
];

thumbAnchorOffset = keySize + halfKeySize;			// Anchor 2-key 1.5 keys down
// Rotation of 2-key thumb buttons such that the bottom corner touches the middle of the lower thumb grid row
// and the size touches the bottom corner of the bottom row index-extra column.
thumbAlpha = atan2(thumbGridOffset, thumbAnchorOffset);
thumbAnchorX = thumbGridLeft + 2 * keySize;			// Anchor 2-key to side of thumb grid
thumbAnchorY = thumbGridTop + thumbAnchorOffset;	// Anchor 2-key to mid-point of lower row in grid
thumbExtraSpacing = 3;								// Extra spacing past thumb 2-keys to account for stabilizer holes
thumbWidth = 2 * keySize + thumbExtraSpacing;		// Total width of thumb 2-keys and extra spacing
thumbDX = twoKeySize * sin(thumbAlpha);				// Distance from anchor to top left (X)
thumbDY = twoKeySize * cos(thumbAlpha);				// Distance from anchor to top left (Y)
thumbLeft = thumbAnchorX + thumbDX;					// Top/left corner (X)
thumbTop = thumbAnchorY - thumbDY;					// Top/left corner (Y)

thumb1CenterX = thumbAnchorX + halfKeySize * cos(thumbAlpha) + halfTwoKeySize * sin(thumbAlpha);
thumb1CenterY = thumbAnchorY + halfKeySize * sin(thumbAlpha) - halfTwoKeySize * cos(thumbAlpha);
thumb2CenterX = thumbAnchorX + (halfKeySize + keySize) * cos(thumbAlpha) + halfTwoKeySize * sin(thumbAlpha);
thumb2CenterY = thumbAnchorY + (halfKeySize + keySize) * sin(thumbAlpha) - halfTwoKeySize * cos(thumbAlpha);

facePlateHeight = 5 * keySize + middleFingerOffset + 2 * wallSpacing + extraBottomSpacing;
facePlatePadding = 1;	// 1mm spacing between edge of faceplate and holes for keycaps (prevent rub/catch)
pcbBottom = facePlateHeight - wallSpacing - extraBottomSpacing;

circleFragments = 30;

leftWristScrew = [washerRadius, facePlateHeight - washerRadius];
rightWristScrew = [thumbAnchorX + facePlatePadding + washerRadius, facePlateHeight - washerRadius];
pinkyScrew = [wallSpacing + keySize - facePlatePadding - washerRadius, pinkyFingerTop - wallSpacing + washerRadius];

thumbScrewX = indexExtraRight + facePlatePadding + washerRadius;
dToScrew = twoKeySize + facePlatePadding + washerRadius;
dToScrewX = thumbAnchorX + dToScrew * sin(thumbAlpha);
dToScrewY = thumbAnchorY - dToScrew * cos(thumbAlpha);
deltaX = dToScrewX - thumbScrewX;
deltaY = deltaX * tan(thumbAlpha);
thumbScrewY = dToScrewY - deltaY;
thumbScrew = [thumbScrewX, thumbScrewY];

screws = [
	leftWristScrew,
	rightWristScrew,
	pinkyScrew,
	thumbScrew
];

pegHoleSize = 3;									// Width and height of PCB to base peg holes
halfPegHoleSize = pegHoleSize / 2;					// Half of the hole size (for centering)
pegLipSize = 1;										// Size of lip on eithr side of peg notch
pegWidth = pegHoleSize + 2 * pegLipSize;			// Total width of the peg
socketHeight = 1;									// Height of the keyswitch sockets above the PCB
pegInnerHeight = 2 * basePlateDepth - pcbDepth - socketHeight; // Distance between bottom of PCB and top of base plate
pegSpacing = 3;										// Min distance to edge of PCB from hole
pegTopNotchHeight = pcbDepth;
pegBottomNotchHeight = basePlateDepth;
pegOuterHeight = pegInnerHeight + pegTopNotchHeight + pegBottomNotchHeight;

lowerRightPeg = [connectorOffset + pegSpacing, pcbBottom - pegSpacing - pegHoleSize];
thumbPeg = [thumbAnchorX, thumbGridTop];
indexPeg = [indexExtraLeft - halfPegHoleSize, indexExtraTop + keySize - halfPegHoleSize];
pinkyPeg = [pinkyFingerLeft - halfPegHoleSize, pinkyExtraTop + keySize - halfPegHoleSize];

pegs = [
	lowerRightPeg,
	thumbPeg,
	indexPeg,
	pinkyPeg
];

// Controller box
teensyWidth = 18;
teensyLength = 31;
controllerConnectorSpacing = 4.7;
controllerPcbWidth = (connectorLength + controllerConnectorSpacing) * 2 + teensyWidth;
controllerOverhang = basePlateDepth;
controllerWallSize = basePlateDepth;
controllerWidth = controllerPcbWidth + (controllerOverhang + controllerWallSize + pcbSpacing) * 2;
controllerPcbLength = teensyLength;
controllerLength = controllerPcbLength + (controllerOverhang + controllerWallSize + pcbSpacing) * 2;
controllerScrewPcbOffset = 3;
controllerScrewDiameter = 2.4;
controllerNotchSize = 5;
controllerNotchOffset = 2;
controllerBottomSpacing = 5;
controllerTopSpacing = 10;
controllerHeight = basePlateDepth + controllerBottomSpacing + pcbDepth + controllerTopSpacing + basePlateDepth;

// Controller notches to connect walls to base and face
distanceToNotch = controllerOverhang + controllerWallSize + controllerNotchOffset;
topNotchTop = controllerOverhang;
bottomNotchTop = controllerLength - controllerOverhang - basePlateDepth;
topBottomLeft1 = distanceToNotch;
topBottomLeft2 = controllerWidth - distanceToNotch - controllerNotchSize;
leftNotchLeft = controllerOverhang;
rightNotchLeft = controllerWidth - controllerOverhang - basePlateDepth;
leftRightTop1 = distanceToNotch;
leftRightTop2 = controllerLength - distanceToNotch - controllerNotchSize;
