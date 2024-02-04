include <constants.scad>

module keySwitch() {
	color("green")
	translate([keyPcbXOffset, 0, 0])
	cube([keyPcbWidth, keyPcbHeight, pcbDepth]);

	translate([0, 0, pcbDepth + pcbMountSpacing])
	difference() {
		cube([keySize, keySize, mountingPlateDepth]);

		translate([mountingHoleOffset, mountingHoleOffset, -1])
		cube([keyHoleSize, keyHoleSize, mountingPlateDepth + 2]);
	}
}

module homeRow() {
	keySwitch();

	translate([keySize, middleYOffset, middleZOffset])
	keySwitch();

	translate([2 * keySize, ringYOffset, ringZOffset])
	keySwitch();

	translate([3 * keySize, pinkyYOffset, pinkyZOffset])
	keySwitch();
}

homeRow();
