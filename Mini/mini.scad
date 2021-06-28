include <constants.scad>

module pegHoles() {
	for (peg = pegs) {
		translate([peg.x, peg.y, 0])
		cube([pegHoleSize, pegHoleSize, basePlateDepth]);
	}
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
	translate([connectorLeft, pinkyFingerTop, 0])
	cube([wristPadWidth - connectorLeft, facePlateHeight - pinkyFingerTop, basePlateDepth]);

	// Concave corner above connector
	concaveCornerBottomRight(connectorLeft + wallSpacing, pinkyFingerTop + 3 * keySize, wallSpacing, basePlateDepth);

	// Thumb grid to 2-key filler
	translate([thumbAnchorX, thumbGridTop, 0])
	cube([thumbGridOffset, 2 * keySize + wallSpacing, basePlateDepth]);

	// Two 2-key thumb keys, rotated, with left wall
	translate([thumbLeft, thumbTop, 0])
	rotate([0, 0, thumbAlpha])
	cube([thumbWidth + wallSpacing, twoKeySize, basePlateDepth]);

	// Fancy thumb rotation distance calculations
	dy = halfKeySize + wallSpacing + extraBottomSpacing;
	h1 = thumbWidth + wallSpacing;
	y1 = h1 * sin(thumbAlpha);
	x1 = h1 * cos(thumbAlpha);
	y2 = dy - y1;
	x2 = y2 * tan(thumbAlpha);
	h2 = sqrt(x2 * x2 + y2 * y2);
	dx = x1 - x2 - h2;
	y3 = dx * tan(thumbAlpha);
	r = dy - y3;

	translate([thumbAnchorX, thumbAnchorY, 0])
	cube([dx, dy, basePlateDepth]);

	translate([thumbAnchorX + dx, thumbAnchorY + y3])
	cylinder(basePlateDepth, r = r, $fn = circleFragments);

	// Top wall of thumb
	twx = thumbAnchorX + (twoKeySize + wallSpacing) * sin(thumbAlpha);
	twy = thumbAnchorY - (twoKeySize + wallSpacing) * cos(thumbAlpha);
	translate([twx, twy, 0])
	rotate([0, 0, thumbAlpha])
	cube([thumbWidth, wallSpacing, basePlateDepth]);

	// Top left corner of thumb
	tcx = thumbWidth * cos(thumbAlpha);
	tcy = thumbWidth * sin(thumbAlpha);
	translate([thumbLeft + tcx, thumbTop + tcy, 0])
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
		alpha = 30;
		edgeRadius = hexHeight / 2 + hexPadding;
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
		radius = screwDiameter / 2;
		translate([x, y, 0])
		cylinder(basePlateDepth, r = radius, $fn = circleFragments);
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
		pegHoles();
	}
}

module connectorNotch() {
	translate([connectorLeft, facePlateHeight - washerSize - connectorEdgeSize - 2 * connectorPadding, 0])
	cube([connectorLength + wallSpacing + connectorPadding, connectorEdgeSize + 2 * connectorPadding, basePlateDepth]);
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

	module stabilizer(centerX, centerY, alpha) {
		module rect(left, top, width, height) {
			translate([left, top, 0])
			cube([width, height, basePlateDepth]);
		}

		stabilizerWidth = 23.88;		//2u, 2.5u, 2.75u

		sideWidth = 6.65;
		sideHeight = 12.3;
		sideBottomOffset = 6.6;
		sideTopOffset = sideHeight - sideBottomOffset;
		sideTop = centerY - sideTopOffset;

		connectorTopOffset = 0.81;
		connectorBottomOffset = 0.79;
		connectorHeight = sideHeight - connectorTopOffset - connectorBottomOffset;
		connectorWidth = (stabilizerWidth - keyHoleSize - sideWidth) / 2;
		connectorTop = sideTop + connectorTopOffset;

		sideNotchOffsetX = 4.22;
		sideNotchWidth = sideNotchOffsetX - (sideWidth / 2);
		sideNotchBottomOffset = 0.51;
		sideNotchHeight = 2.8;
		sideNotchTopOffset = sideNotchHeight - sideNotchBottomOffset;
		sideNotchTop = centerY - sideNotchTopOffset;

		bottomNotchWidth = 3.05;
		bottomNotchBottomOffset = 13.46;
		bottomNotchHeight = bottomNotchBottomOffset - sideHeight;
		bottomNotchTop = sideTop + sideHeight;

		translate([centerX, centerY, 0])
		rotate([0, 0, alpha])
		translate([-centerX, -centerY, 0])
		union() {
			// Center hole
			rect(centerX - halfKeyHoleSize, centerY - halfKeyHoleSize, keyHoleSize, keyHoleSize);

			// Stabilizer holes
			rect(centerX + stabilizerWidth / 2 - sideWidth / 2, sideTop, sideWidth, sideHeight);
			rect(centerX - stabilizerWidth / 2 - sideWidth / 2, sideTop, sideWidth, sideHeight);

			// Stabilizer hole to center hole connectors
			rect(centerX + halfKeyHoleSize, connectorTop, connectorWidth, connectorHeight);
			rect(centerX - halfKeyHoleSize - connectorWidth, connectorTop, connectorWidth, connectorHeight);

			// Side notches
			rect(centerX + stabilizerWidth / 2 + sideWidth / 2, sideNotchTop, sideNotchWidth, sideNotchHeight);
			rect(centerX - stabilizerWidth / 2 - sideWidth / 2 - sideNotchWidth, sideNotchTop, sideNotchWidth, sideNotchHeight);

			// Bottom notches
			rect(centerX + stabilizerWidth / 2 - bottomNotchWidth / 2, bottomNotchTop, bottomNotchWidth, bottomNotchHeight);
			rect(centerX - stabilizerWidth / 2 - bottomNotchWidth / 2, bottomNotchTop, bottomNotchWidth, bottomNotchHeight);
		}
	}

	difference() {
		core();
		for (fingerColumn = fingerColumns) {
			column(fingerColumn.x, fingerColumn.y, fingerColumn.z);
		}
		stabilizer(thumb1CenterX, thumb1CenterY, thumbAlpha + 90);
		stabilizer(thumb2CenterX, thumb2CenterY, thumbAlpha + 270);
		connectorNotch();
		m4HexHoles();
	}
}

module upperLayer() {
	difference() {
		facePlate();

		top = ringFingerTop + 3 * keySize;
		bottom = facePlateHeight - rimSize - extraBottomSpacing;
		left = connectorOffset - pcbSpacing;
		right = thumbAnchorX;

		translate([left, top, 0])
		cube([right - left, bottom - top, basePlateDepth]);

		translate([pinkyFingerLeft, pinkyFingerTop + 2 * keySize, 0])
		cube([keySize + 2 * wallSpacing + 3 * keySize, keySize + pcbSpacing, basePlateDepth]);

		// Thumb 2-keys plus extra spacing
		tw = twoKeySize + facePlatePadding;
		th = twoKeySize + 2 * facePlatePadding;
		tx = thumbAnchorX + tw * sin(thumbAlpha);
		ty = thumbAnchorY - tw * cos(thumbAlpha);
		translate([tx, ty, 0])
		rotate([0, 0, thumbAlpha])
		cube([tw + thumbExtraSpacing, th, basePlateDepth]);
	}
}

module lowerLayer() {
	upperLayer();

	left = connectorLeft;
	top = facePlateHeight - washerSize - connectorEdgeSize - 2 * connectorPadding;
	translate([left, top, 0])
	cube([rimSize, connectorEdgeSize + 2 * connectorPadding, basePlateDepth]);
}

module pcb() {
	module pcbCore() {
		for (fingerColumn = fingerColumns) {
			x = fingerColumn.x;
			y = fingerColumn.y;
			height = fingerColumn.z * keySize;
			translate([x, y, 0])
			cube([keySize, height, basePlateDepth]);
		}

		// Area under palm
		palmTop = middleFingerTop + 3 * keySize;
		translate([connectorOffset, palmTop, 0])
		cube([indexFingerLeft - connectorOffset, pcbBottom - palmTop, basePlateDepth]);

		// Thumb 2-keys
		translate([thumbLeft, thumbTop, 0])
		rotate([0, 0, thumbAlpha])
		cube([thumbWidth, twoKeySize, basePlateDepth]);

		// Hole between thumb 2-keys and thumb grid
		translate([thumbAnchorX, thumbGridTop, 0])
		cube([thumbGridOffset, thumbAnchorY - thumbGridTop, basePlateDepth]);

		// Spiky notch next to thumb 2-keys
		dx = thumbLeft - indexExtraRight;
		dy = dx * tan(thumbAlpha);
		translate([indexExtraRight, thumbTop - dy, 0])
		rotate([0, 0, thumbAlpha])
		cube([keySize, keySize, basePlateDepth]);
	}

	difference() {
		pcbCore();
		pegHoles();
	}
}

module pcbSpacing() {
	difference() {
		core();
		lowerLayer();
		m4HexHoles();
		pcb();
		pegHoles();
	}
}

module peg() {
	translate([pegLipSize, 0, 0])
	cube([pegHoleSize, pegTopNotchHeight, basePlateDepth]);

	translate([0, pegTopNotchHeight, 0])
	cube([pegWidth, pegInnerHeight, basePlateDepth]);

	translate([pegLipSize, pegTopNotchHeight + pegInnerHeight, 0])
	cube([pegHoleSize, pegBottomNotchHeight, basePlateDepth]);
}

module everything() {
	// core();
	basePlate();
	// facePlate();
	// wristPad();
	// mountingPlate();
	// upperLayer();
	// lowerLayer();
	// pcb();
	// pcbSpacing();
	// peg();
}

projection(cut = false) {
	color([0, 0, 1])
	everything();
}
