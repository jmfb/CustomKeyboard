include <constants.scad>

module keySwitch(extraTop, extraBottom) {
	color("green")
	translate([keyPcbXOffset, 0, 0])
	cube([keyPcbWidth, keyPcbHeight, pcbDepth]);

	translate([0, -extraTop, pcbDepth + pcbMountSpacing])
	difference() {
		cube([keySize, keySize + extraTop + extraBottom, mountingPlateDepth]);

		translate([mountingHoleOffset, extraTop + mountingHoleOffset, -epsilon])
		cube([keyHoleSize, keyHoleSize, mountingPlateDepth + 2 * epsilon]);
	}
}

module keyColumn() {
	keySwitch(homeToTopRowExtension, homeToBottomRowExtension);

	translate([0, keySize + pcbBottomSpaceUnderHome, 0])
	rotate([lowerRowAngle, 0, 0])
	translate([0, pcbBottomSpaceAfterLowerBend, 0])
	union() {
		keySwitch(bottomRowToHomeExtension, extraRowExtension);

		translate([0, keySize + extraRowExtension - modelWallDepth, 0])
		cube([keySize, modelWallDepth, distanceToMountingPlateBottom]);
	}

	translate([0, -pcbBottomSpaceAboveHome, 0])
	rotate([-upperRowAngle, 0, 0])
	translate([0, -(keySize + pcbBottomSpaceAfterUpperBend), 0])
	union() {
		keySwitch(extraRowExtension, topRowToHomeExtension);

		translate([0, -extraRowExtension, 0])
		cube([keySize, modelWallDepth, distanceToMountingPlateBottom]);
	}
}

module homeRow() {
	keyColumn();

	translate([keySize, -middleYOffset, middleZOffset])
	keyColumn();

	translate([2 * keySize, -ringYOffset, ringZOffset])
	keyColumn();

	translate([3 * keySize, -pinkyYOffset, pinkyZOffset])
	keyColumn();
}

// Left hand (invert x for Right Hand)
homeRow();
