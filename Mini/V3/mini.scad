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
	union() {
		keyColumn(bottomRowWallMountHeightIndex);

		// Bottom row wall
		translate([0, bottomRowWallDistanceIndex - modelWallDepth, -minDepthBelowPcb + ringZOffset])
		cube([keySize, modelWallDepth, bottomRowWallHeightIndex - ringZOffset]);
	}

	// Middle finger
	translate([keySize, -middleYOffset, middleZOffset])
	union() {
		keyColumn(bottomRowWallMountHeightMiddle);

		// Bottom row wall
		translate([0, bottomRowWallDistanceMiddle - modelWallDepth, -minDepthBelowPcb + middleZOffset])
		cube([keySize, modelWallDepth, bottomRowWallHeightMiddle - middleZOffset]);

		// Top row wall
		translate([0, -topRowWallDistanceMiddle, -minDepthBelowPcb + middleZOffset])
		cube([keySize, modelWallDepth, topRowWallHeightMiddle - middleZOffset]);
	}

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

	//Pinky plus one column
	translate([4 * keySize + pp1XOffset, -pinkyYOffset, pinkyZOffset])
	union() {
		translate([0, halfKeySize, distanceToMountingPlateBottom + mountingPlateDepth])
		rotate([halfPinkyRowAngle, -pinkyColumnAngle, 0])
		translate([0, 0, -distanceToMountingPlateBottom - mountingPlateDepth])
		keySwitch(0, 0);

		translate([0, halfKeySize, distanceToMountingPlateBottom + mountingPlateDepth])
		rotate([-halfPinkyRowAngle, -pinkyColumnAngle, 0])
		translate([0, -keySize, -distanceToMountingPlateBottom - mountingPlateDepth])
		keySwitch(0, 0);
	}
}

// Left hand (invert x for Right Hand)
homeRow();
