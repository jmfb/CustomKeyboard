include <constants.scad>

module keySwitch(extraTop, extraBottom) {
	color("green")
	translate([keyPcbXOffset, 0, 0])
	cube([keyPcbWidth, keyPcbHeight, pcbDepth]);

	translate([0, -extraTop, pcbDepth + pcbMountSpacing])
	difference() {
		cube([keySize, keySize + extraTop + extraBottom, mountingPlateDepth]);

		translate([mountingHoleOffset, extraTop + mountingHoleOffset, -1])
		cube([keyHoleSize, keyHoleSize, mountingPlateDepth + 2]);
	}
}

module keyColumn() {
	keySwitch(homeToTopRowExtension, homeToBottomRowExtension);

	translate([0, keySize + pcbBottomSpaceUnderHome, 0])
	rotate([lowerRowAngle, 0, 0])
	translate([0, pcbBottomSpaceAfterLowerBend, 0])
	keySwitch(bottomRowToHomeExtension, 0);

	translate([0, -pcbBottomSpaceAboveHome, 0])
	rotate([-upperRowAngle, 0, 0])
	translate([0, -(keySize + pcbBottomSpaceAfterUpperBend), 0])
	keySwitch(0, topRowToHomeExtension);
}

module homeRow() {
	keyColumn();

	translate([keySize, middleYOffset, middleZOffset])
	keyColumn();

	translate([2 * keySize, ringYOffset, ringZOffset])
	keyColumn();

	translate([3 * keySize, pinkyYOffset, pinkyZOffset])
	keyColumn();
}

homeRow();
