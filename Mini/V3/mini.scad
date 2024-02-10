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

module keyColumn(lowerRowMountHeight) {
	// Home row
	keySwitch(homeToTopRowExtension, homeToBottomRowExtension);

	// Bottom row
	translate([0, keySize + pcbBottomSpaceUnderHome, 0])
	rotate([lowerRowAngle, 0, 0])
	translate([0, pcbBottomSpaceAfterLowerBend, 0])
	union() {
		keySwitch(bottomRowToHomeExtension, extraRowExtension);

		// Diagonal wall to vertical edge
		translate([0, keySize + extraRowExtension - modelWallDepth, -minDepthBelowPcb - lowerRowMountHeight + mountHeight])
		cube([keySize, modelWallDepth, lowerRowMountHeight]);
	}

	// Top row
	translate([0, -pcbBottomSpaceAboveHome, 0])
	rotate([-upperRowAngle, 0, 0])
	translate([0, -(keySize + pcbBottomSpaceAfterUpperBend), 0])
	union() {
		keySwitch(extraRowExtension, topRowToHomeExtension);

		// Diagonal wall to vertical edge
		translate([0, -extraRowExtension, -minDepthBelowPcb])
		cube([keySize, modelWallDepth, mountHeight]);
	}
}

module homeRow() {
	// Index finger
	keyColumn(mountHeight);

	// Middle finger
	translate([keySize, -middleYOffset, middleZOffset])
	keyColumn(mountHeight);

	// Ring finger
	translate([2 * keySize, -ringYOffset, ringZOffset])
	union() {
		keyColumn(bottomRowWallMountHeightRing);

		// Bottom row wall
		translate([0, bottomRowWallDistanceRing - modelWallDepth, -minDepthBelowPcb])
		cube([keySize, modelWallDepth, bottomRowWallHeightRing]);
	}

	// Pinky finger
	translate([3 * keySize, -pinkyYOffset, pinkyZOffset])
	union() {
		keyColumn(mountHeight);

		// Bottom row wall
		translate([0, bottomRowWallDistancePinky - modelWallDepth, -minDepthBelowPcb + ringZOffset - pinkyZOffset])
		cube([keySize, modelWallDepth, bottomRowWallHeightPinky - ringZOffset + pinkyZOffset]);
	}
}

// Left hand (invert x for Right Hand)
homeRow();
