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

connectorEdgeSize = 14;	// Width of the connector on the edge of the PCB
connectorLength = 12.3;	// Distance from edge of PCB to back of connector
connectorOffset = 42;	// Distance from edge of PCB to portion of cable at outer edge
connectorLeft = connectorOffset - wallSpacing; // Distance to edge of base next to connector

thumbGridOffset = 7;			// Shift back from innermost finger column
middleFingerOffset = 4;			// Shift up from index and ring finger rows
pinkyOffset = 4;				// Shift down from ringer finger row
pinkyExtraOffset = halfKeySize;	// Shift down from pinky finger row

wristPadHeight = 100;	// Height of the wrist pad (Glorious PC gaming model)
wristPadWidth = 6 * keySize - thumbGridOffset + pcbSpacing + rimSize;
wristPadCornerRadius = 7;	// Radius of the thumb side bottom coner (only one rounded since other is cut)

washerDiameter = 9;									// Diameter of the M4 washers
washerSpacing = 1;									// Spacing between edge of washer and edge of case
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
thumbDX = twoKeySize * sin(thumbAlpha);				// Distance from anchor to top left (X)
thumbDY = twoKeySize * cos(thumbAlpha);				// Distance from anchor to top left (Y)
thumbLeft = thumbAnchorX + thumbDX;					// Top/left corner (X)
thumbTop = thumbAnchorY - thumbDY;					// Top/left corner (Y)

facePlateHeight = 5 * keySize + middleFingerOffset + 2 * wallSpacing;
facePlatePadding = 1;	// 1mm spacing between edge of faceplate and holes for keycaps (prevent rub/catch)

pegLip = 1;
pegNotch = 3;
pegDepth = 3;

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
