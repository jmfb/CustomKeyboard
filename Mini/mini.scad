include <constants.scad>

module pegHole(x, y) {
	translate([x, y, 0])
	cube([pegNotch, pegNotch, basePlateDepth]);
}

module core() {
	module convexCorner(x, y, r, depth) {
		translate([x, y])
		cylinder(depth, r = r, $fn = circleFragments);
	}

	module concaveCornerTopRight(x, y, r, depth) {
		translate([x - 2 * r, y - 2 * r, 0])
		difference() {
			cube([r, r, depth]);
			cylinder(depth, r = r, $fn = circleFragments);
		}
	}

	module concaveCornerBottomRight(x, y, r, depth) {
		translate([x - 2 * r, y + r, 0])
		difference() {
			cube([r, r, depth]);
			translate([0, r, 0])
			cylinder(depth, r = r, $fn = circleFragments);
		}
	}

	module keyColumn(left, top, count) {
		width = keySize;
		height = count * keySize;
		depth = basePlateDepth;
		border = wallSpacing;

		right = left + width;
		bottom = top + height;

		translate([left, top, 0])
		cube([width, height, depth]);

		translate([left - border, top, 0])
		cube([border, height, depth]);

		translate([right, top, 0])
		cube([border, height, depth]);

		translate([left, top - border, 0])
		cube([width, border, depth]);

		translate([left, bottom, 0])
		cube([width, border, depth]);

		convexCorner(left, top, border, depth);
		convexCorner(left, bottom, border, depth);
		convexCorner(right, top, border, depth);
		convexCorner(right, bottom, border, depth);
	}

	module thumbScrewOutline() {
		translate([thumbScrewX, thumbScrewY, 0])
		cylinder(basePlateDepth, r = washerRadius, $fn = circleFragments);

		cornerRadius = washerRadius - rimSize;
		h = washerRadius + cornerRadius;

		// Rounded corner to 2-keys
		x1 = h * cos(thumbAlpha);
		y1 = h * sin(thumbAlpha);
		difference() {
			translate([thumbScrewX, thumbScrewY, 0])
			rotate([0, 0, thumbAlpha])
			cube([h, cornerRadius, basePlateDepth]);

			translate([thumbScrewX + x1, thumbScrewY + y1, 0])
			cylinder(basePlateDepth, r = cornerRadius, $fn = circleFragments);
		}

		// Rounded corner to index extra
		difference() {
			translate([thumbScrewX - cornerRadius, thumbScrewY - h, 0])
			cube([cornerRadius, h, basePlateDepth]);

			translate([thumbScrewX, thumbScrewY - h, 0])
			cylinder(basePlateDepth, r = cornerRadius, $fn = circleFragments);
		}
	}

	module pinkyScrewOutline() {
		cx = pinkyScrew.x;
		cy = pinkyScrew.y;
		height = pinkyExtraOffset + wallSpacing;
		extra = height - washerRadius - wallSpacing;

		translate([cx, cy - washerRadius, 0])
		cube([washerRadius, height, basePlateDepth]);

		translate([cx - washerRadius, cy, 0])
		cube([washerSize, height - washerRadius, basePlateDepth]);

		translate([cx, cy, 0])
		cylinder(basePlateDepth, r = washerRadius, $fn = circleFragments);

		// Cover the start of the rounded edge above the pinky finger since we have extended it
		translate([pinkyFingerLeft - facePlatePadding, pinkyFingerTop - wallSpacing, 0])
		cube([wallSpacing, wallSpacing, basePlateDepth]);

		translate([cx - washerRadius - extra, cy, 0])
		difference() {
			cube([extra, extra, basePlateDepth]);
			cylinder(basePlateDepth, r = extra, $fn = circleFragments);
		}
	}

	// Standard 17 keys
	for (fingerColumn = fingerColumns) {
		keyColumn(fingerColumn.x, fingerColumn.y, fingerColumn.z);
	}

	// Concave corner above pinky (TODO: move for screw)
	concaveCornerTopRight(pinkyFingerLeft, pinkyExtraTop, wallSpacing, basePlateDepth);

	// Concave corner below pinky
	concaveCornerBottomRight(pinkyFingerLeft, pinkyExtraTop + 2 * keySize, wallSpacing, basePlateDepth);

	// Top (missing rounded corner) portion of island
	translate([washerRadius, facePlateHeight - washerSize, 0])
	cube([wristPadWidth - washerRadius, washerSize, basePlateDepth]);

	// Bottom (unrounded) portion of island
	translate([0, facePlateHeight - washerRadius, 0])
	cube([washerRadius, washerRadius, basePlateDepth]);

	// Convex corner on top right of island above wrist pad
	convexCorner(washerRadius, facePlateHeight - washerRadius, washerRadius, basePlateDepth);

	// Box filling in right of thumb grid and left of connector (under palm)
	translate([connectorOffset, pinkyFingerTop, 0])
	cube([wristPadWidth - connectorOffset, facePlateHeight - pinkyFingerTop, basePlateDepth]);

	// Concave corner above connector
	concaveCornerBottomRight(connectorOffset + wallSpacing, pinkyFingerTop + 3 * keySize, wallSpacing, basePlateDepth);

	// Thumb grid to 2-key filler
	translate([thumbAnchorX, thumbGridTop, 0])
	cube([thumbGridOffset, 2 * keySize + wallSpacing, basePlateDepth]);

	// Two 2-key thumb keys, rotated, with left wall
	translate([thumbLeft, thumbTop, 0])
	rotate([0, 0, thumbAlpha])
	cube([2 * keySize + wallSpacing, twoKeySize, basePlateDepth]);

	// Fancy thumb rotation distance calculations
	th = twoKeySize + wallSpacing;
	sth = halfKeySize + wallSpacing;
	x = th * cos(thumbAlpha) - tan(thumbAlpha) * (sth - th * sin(thumbAlpha));
	h = (sth - th * sin(thumbAlpha)) / cos(thumbAlpha);
	dx = x - h;
	y = dx * tan(thumbAlpha);
	h2 = sqrt(dx * dx + y * y);
	r = th - h2;

	// Bottom (unrotated) portion of thumb
	translate([thumbAnchorX, thumbAnchorY, 0])
	cube([dx, halfKeySize + wallSpacing, basePlateDepth]);

	// Lower left corner of thumb
	translate([thumbAnchorX + dx, thumbAnchorY + y, 0])
	cylinder(basePlateDepth, r = r, $fn = circleFragments);

	// Top wall of thumb
	twx = thumbAnchorX + (twoKeySize + wallSpacing) * sin(thumbAlpha);
	twy = thumbAnchorY - (twoKeySize + wallSpacing) * cos(thumbAlpha);
	translate([twx, twy, 0])
	rotate([0, 0, thumbAlpha])
	cube([2 * keySize, wallSpacing, basePlateDepth]);

	// Top left corner of thumb
	translate([thumbLeft + thumbDY, thumbTop + thumbDX, 0])
	cylinder(basePlateDepth, r = wallSpacing, $fn = circleFragments);

	thumbScrewOutline();
	pinkyScrewOutline();
}

module wristPad() {
	top = facePlateHeight;
	translate([0, top, 0])
	cube([wristPadWidth, wristPadHeight - wristPadCornerRadius, basePlateDepth]);

	translate([0, top + wristPadHeight - wristPadCornerRadius, 0])
	cube([wristPadWidth - wristPadCornerRadius, wristPadCornerRadius, basePlateDepth]);

	translate([wristPadWidth - wristPadCornerRadius, top + wristPadHeight - wristPadCornerRadius, 0])
	cylinder(basePlateDepth, r = wristPadCornerRadius, $fn = circleFragments);
}


module m4HexHoles() {
	module hole(x, y) {
		hexHeight = 6; // TODO: measured height (flat to flat) of M4 hex standoff
		padding = 0.5; // Padding between side and cutout (not too snug it gets stuck)
		alpha = 30;
		edgeRadius = hexHeight / 2 + padding;
		halfHoleSide = edgeRadius * tan(alpha);
		holeSide = 2 * halfHoleSide;
		holeWidth = 2 * holeSide;

		translate([x - halfHoleSide, y - edgeRadius, 0])
		cube([holeSide, 2 * edgeRadius, basePlateDepth]);

		translate([x + halfHoleSide, y - edgeRadius, 0])
		rotate([0, 0, 2 * alpha])
		cube([holeSide, 2 * edgeRadius, basePlateDepth]);

		translate([x - holeSide, y, 0])
		rotate([0, 0, -2 * alpha])
		cube([holeSide, 2 * edgeRadius, basePlateDepth]);
	}

	for (screw = screws) {
		hole(screw.x, screw.y);
	}
}


module m4ScrewHoles() {
	module hole(x, y) {
		diameter = 4; 			// TODO: measure diameter of screw threads (should be 4mm for M4)
		radius = diameter / 2;
		padding = 0.25;			// Radial padding to keep from being too snug
		translate([x, y, 0])
		cylinder(basePlateDepth, r = (radius + padding), $fn = circleFragments);
	}

	for (screw = screws) {
		hole(screw.x, screw.y);
	}
}

module basePlate() {
	difference() {
		union() {
			core();
			wristPad();
		}

		m4ScrewHoles();

		// TODO: 3x3 peg holes (?)
	}
}

module connectorNotch() {
	// Connector notch
	translate([connectorOffset, facePlateHeight - washerSize - connectorEdgeSize, 0])
	cube([connectorLength + wallSpacing, connectorEdgeSize, basePlateDepth]);
}

module facePlate() {
	module column(x, y, count) {
		translate([x - facePlatePadding, y - facePlatePadding, 0])
		cube([keySize + 2 * facePlatePadding, count * keySize + 2 * facePlatePadding, basePlateDepth]);
	}

	difference() {
		core();

		for (fingerColumn = fingerColumns) {
			column(fingerColumn.x, fingerColumn.y, fingerColumn.z);
		}

		// Thumb 2-keys
		tw = twoKeySize + facePlatePadding;
		th = twoKeySize + 2 * facePlatePadding;
		tx = thumbAnchorX + tw * sin(thumbAlpha);
		ty = thumbAnchorY - tw * cos(thumbAlpha);
		translate([tx, ty, 0])
		rotate([0, 0, thumbAlpha])
		cube([tw, th, basePlateDepth]);

		// Hole between thumb areas
		translate([thumbAnchorX, thumbGridTop - facePlatePadding, 0])
		cube([thumbGridOffset + 2 * facePlatePadding, thumbAnchorY - thumbGridTop + facePlatePadding, basePlateDepth]);

		// Spiky point near thumb screw
		dx = tx - indexExtraRight;
		dy = dx * tan(thumbAlpha);
		dh = sqrt(dx * dx + dy * dy);
		translate([indexExtraRight, ty - dy, 0])
		rotate([0, 0, thumbAlpha])
		cube([dh, keySize, basePlateDepth]);

		connectorNotch();
		m4HexHoles();
	}
}

module mountingPlate() {
	module column(x, y, count) {
		for (index = [0 : count - 1]) {
			translate([x + keyHolePadding, y + keyHolePadding + index * keySize, 0])
			cube([keyHoleSize, keyHoleSize, basePlateDepth]);
		}
	}

	module stabilizer(index) {
		hx = keyHolePadding + index * keySize;
		hy = keyHoleSize + twoKeyHolePadding;
		xa = hx * cos(thumbAlpha);
		ya = hx * sin(thumbAlpha);
		xb = hy * sin(thumbAlpha);
		yb = hy * cos(thumbAlpha);
		x = thumbAnchorX + xa + xb;
		y = thumbAnchorY + ya - yb;
		translate([x, y, 0])
		rotate([0, 0, thumbAlpha])
		cube([keyHoleSize, keyHoleSize, basePlateDepth]);

		// TODO: full stabilizer mounting holes for 2-key stabilizer
	}

	difference() {
		core();

		for (fingerColumn = fingerColumns) {
			column(fingerColumn.x, fingerColumn.y, fingerColumn.z);
		}

		// Full 2-key hole for stabilizer
		stabilizer(0);
		stabilizer(1);

		connectorNotch();
		m4HexHoles();
	}
}

module upperLayer() {
	difference() {
		facePlate();

		top = ringFingerTop + 3 * keySize;
		bottom = facePlateHeight - rimSize;
		left = connectorLeft - pcbSpacing;
		right = thumbAnchorX;

		translate([left, top, 0])
		cube([right - left, bottom - top, basePlateDepth]);

		translate([pinkyFingerLeft, pinkyFingerTop + 2 * keySize, 0])
		cube([keySize + 2 * wallSpacing + 3 * keySize, keySize + pcbSpacing, basePlateDepth]);
	}
}

module lowerLayer() {
	upperLayer();

	left = connectorLeft - wallSpacing;
	top = facePlateHeight - washerSize - connectorEdgeSize;
	translate([left, top, 0])
	cube([rimSize, connectorEdgeSize, basePlateDepth]);
}

module pcb() {
	difference() {
		core();
		lowerLayer();
		m4HexHoles();
		// TODO: technically this needs to remvoe the pcbSpacing (1mm border), but close
	}
}

module peg(x, y) {
	height = 2 * basePlateDepth - pcbDepth;

	translate([x + pegLip, y, 0])
	cube([pegNotch, pcbDepth, pegDepth]);

	translate([x, y + pcbDepth, 0])
	cube([pegNotch + 2 * pegLip, height, pegDepth]);

	translate([x + pegLip, y + pcbDepth + height, 0])
	cube([pegNotch, basePlateDepth, pegDepth]);
}

module everything() {
	// core();
	basePlate();
	// facePlate();
	// mountingPlate();
	// upperLayer();
	// lowerLayer();
	// pcb();
	// peg(0, 0);
}

projection(cut = false) {
	color([0, 0, 1])
	everything();
}
