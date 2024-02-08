include <constants.scad>

module keySwitch(extraTop, extraBottom) {
	// PCB footprint
	color("green")
	translate([keyPcbXOffset, 0, 0])
	cube([keyPcbWidth, keyPcbHeight, pcbDepth]);

	translate([0, -extraTop, pcbDepth + pcbMountSpacing])
	difference() {
		// Mounting plate level
		cube([keySize, keySize + extraTop + extraBottom, mountingPlateDepth]);

		// Key hole cutout
		translate([mountingHoleOffset, extraTop + mountingHoleOffset, -epsilon])
		cube([keyHoleSize, keyHoleSize, mountingPlateDepth + 2 * epsilon]);
	}
}

module keyColumn() {
	// Home row
	keySwitch(homeToTopRowExtension, homeToBottomRowExtension);

	// Bottom row
	translate([0, keySize + pcbBottomSpaceUnderHome, 0])
	rotate([lowerRowAngle, 0, 0])
	translate([0, pcbBottomSpaceAfterLowerBend, 0])
	union() {
		keySwitch(bottomRowToHomeExtension, extraRowExtension);

		// Diagonal wall to vertical edge
		translate([0, keySize + extraRowExtension - modelWallDepth, 0])
		cube([keySize, modelWallDepth, distanceToMountingPlateBottom]);
	}

	// Top row
	translate([0, -pcbBottomSpaceAboveHome, 0])
	rotate([-upperRowAngle, 0, 0])
	translate([0, -(keySize + pcbBottomSpaceAfterUpperBend), 0])
	union() {
		keySwitch(extraRowExtension, topRowToHomeExtension);

		// Diagonal wall to vertical edge
		translate([0, -extraRowExtension, 0])
		cube([keySize, modelWallDepth, distanceToMountingPlateBottom]);
	}
}

module homeRow() {
	// Index finger
	keyColumn();

	// Middle finger
	translate([keySize, -middleYOffset, middleZOffset])
	keyColumn();

	// Ring finger
	translate([2 * keySize, -ringYOffset, ringZOffset])
	keyColumn();

	// Pinky finger
	translate([3 * keySize, -pinkyYOffset, pinkyZOffset])
	keyColumn();
}

// Left hand (invert x for Right Hand)
homeRow();
