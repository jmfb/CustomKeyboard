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
	translate([connectorLeft, facePlateHeight - washerSize - connectorEdgeSize - connectorPadding, 0])
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

		// Dual inline microchips (demux and register)
		translate([48, 68.5, 0])
		cube([20, 10, basePlateDepth]);
		translate([56, 82, 0])
		cube([10, 20, basePlateDepth]);
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

module echoThumb2KeyLedPcbCutouts() {
	module rotatePoint(name, anchorX, anchorY, x, y) {
		// Triangle o1,a1,h1 has side x rotated down from horizontal axis
		a1 = x;
		o1 = a1 * tan(thumbAlpha);
		h1 = a1 / cos(thumbAlpha);
		// Triangle o2,a2,h2 has side h2 rotated right of vertical axis
		h2 = y - o1;
		o2 = h2 * sin(thumbAlpha);
		a2 = h2 * cos(thumbAlpha);
		// Rotated distances from anchor
		dx = o2 + h1;
		dy = a2;
		// Rotated coordinates (subtracting for right hand)
		rx = anchorX + dx;
		ry = anchorY - dy;
		echo(name, rx, ry);
	}

	module rotateCube(name, anchorX, anchorY, bottom, right, width, height) {
		// Top, left, bottom, and right are distances from anchor in unrotated world.
		top = bottom + height;
		left = right + width;
		rotatePoint(str(name, "_TopLeft"), anchorX, anchorY, left, top);
		rotatePoint(str(name, "_TopRight"), anchorX, anchorY, right, top);
		rotatePoint(str(name, "_BottomLeft"), anchorX, anchorY, left, bottom);
		rotatePoint(str(name, "_BottomRight"), anchorX, anchorY, right, bottom);
	}

	portionOfLedUnderKeyFootprint = 4;
	ledSize = 5;
	left1 = keyHolePadding + keyHoleSize - portionOfLedUnderKeyFootprint;
	left2 = left1 + keySize;
	top = twoKeyHolePadding + (keyHoleSize - ledSize) / 2;
	// PCB coordinate system is offset from case coordinate system; use PCB coordinates
	pcbAnchorX = 119;
	pcbAnchorY = 101.5;

	rotateCube("ThumbKey1", pcbAnchorX, pcbAnchorY, top, left1, ledSize, ledSize);
	rotateCube("ThumbKey2", pcbAnchorX, pcbAnchorY, top, left2, ledSize, ledSize);
}

module echoThumb2KeyPositions() {
	x0 = 119;
	y0 = 101.5;
	o1 = thumbGridOffset;
	a1 = keySize + halfKeySize;
	alpha = thumbAlpha;
	h1 = o1 / sin(alpha);
	a2 = twoKeySize - h1;
	h2 = a2 / cos(alpha);
	yul = y0 - a1 - h2;
	echo("yul", yul);

	h3 = 2 * keySize + thumbExtraSpacing;
	o3 = h3 * sin(alpha);
	a3 = h3 * cos(alpha);
	xlr = x0 + a3;
	ylr = y0 + o3;
	echo("xlr, ylr", xlr, ylr);

	h4 = twoKeySize;
	o4 = h4 * sin(alpha);
	a4 = h4 * cos(alpha);
	xur = xlr + o4;
	yur = ylr - a4;
	echo("xur, yur", xur, yur);

	module thumbCenter(name, dx, dy) {
		h1 = dx;
		o1 = h1 * sin(alpha);
		a1 = h1 * cos(alpha);
		h2 = dy;
		o2 = h2 * sin(alpha);
		a2 = h2 * cos(alpha);
		xc = x0 + a1 + o2;
		yc = y0 + o1 - a2;
		echo(str(name, "(xc, yc)"), xc, yc);
	}

	thumbCenter("T2_1", halfKeySize, halfTwoKeySize);
	thumbCenter("T2_2", keySize + halfKeySize, halfTwoKeySize);
}

module controllerBase() {
	module topBottomNotch(x, y) {
		translate([x, y, 0])
		cube([controllerNotchSize, basePlateDepth, basePlateDepth]);
	}

	module leftRightNotch(x, y) {
		translate([x, y, 0])
		cube([basePlateDepth, controllerNotchSize, basePlateDepth]);
	}

	module screwHole(x, y) {
		translate([x, y, 0])
		cylinder(basePlateDepth, r = controllerScrewDiameter / 2, $fn = circleFragments);
	}

	difference() {
		cube([controllerWidth, controllerLength, basePlateDepth]);

		topBottomNotch(topBottomLeft1, topNotchTop);
		topBottomNotch(topBottomLeft2, topNotchTop);
		topBottomNotch(topBottomLeft1, bottomNotchTop);
		topBottomNotch(topBottomLeft2, bottomNotchTop);

		leftRightNotch(leftNotchLeft, leftRightTop1);
		leftRightNotch(leftNotchLeft, leftRightTop2);
		leftRightNotch(rightNotchLeft, leftRightTop1);
		leftRightNotch(rightNotchLeft, leftRightTop2);

		distanceToScrew = controllerOverhang + controllerWallSize + pcbSpacing + controllerScrewPcbOffset;
		screwTop1 = distanceToScrew;
		screwTop2 = controllerLength - distanceToScrew;
		screwLeft1 = distanceToScrew;
		screwLeft2 = controllerWidth - distanceToScrew;
		screwHole(screwLeft1, screwTop1);
		screwHole(screwLeft1, screwTop2);
		screwHole(screwLeft2, screwTop1);
		screwHole(screwLeft2, screwTop2);
	}
}

module controllerFace() {
	notchSize = controllerOverhang + controllerWallSize + pcbSpacing + connectorLength + connectorPadding;
	module connector(x, y) {
		translate([x, y, 0])
		cube([notchSize, connectorEdgeSize + 2 * connectorPadding, basePlateDepth]);
	}

	difference() {
		controllerBase();

		connectorTop = (controllerLength - connectorEdgeSize - 2 * connectorPadding) / 2;
		connector(0, connectorTop);
		connector(controllerWidth - notchSize, connectorTop);

		translate([controllerWidth / 2, controllerOverhang + controllerWallSize + pcbSpacing + 24.94 + 1.27, 0])
		cylinder(basePlateDepth, r = 1.5, $fn = circleFragments);
	}
}

module controllerWall(width, slotOffset) {
	module notch(x, y) {
		translate([x, y, 0])
		cube([controllerNotchSize, basePlateDepth, basePlateDepth]);
	}

	module slot(x) {
		translate([x, slotOffset, 0])
		cube([basePlateDepth, controllerHeight / 2, basePlateDepth]);
	}

	difference() {
		union() {
			translate([0, basePlateDepth, 0])
			cube([width, controllerHeight - 2 * basePlateDepth, basePlateDepth]);

			left1 = distanceToNotch;
			left2 = width - distanceToNotch - controllerNotchSize;
			top2 = controllerHeight - basePlateDepth;
			notch(left1, 0);
			notch(left2, 0);
			notch(left1, top2);
			notch(left2, top2);
		}

		slot(controllerOverhang);
		slot(width - controllerOverhang - controllerWallSize);
	}
}

module controllerBottomWall() {
	controllerWall(controllerWidth, 0);
}

module controllerTopWall() {
	difference() {
		controllerBottomWall();

		translate([(controllerWidth - teensyWidth) / 2, 0, 0])
		cube([teensyWidth, controllerTopSpacing + basePlateDepth, basePlateDepth]);
	}
}

module controllerSideWall() {
	difference() {
		controllerWall(controllerLength, controllerHeight / 2);

		holeSize = connectorEdgeSize + 2 * connectorPadding;
		translate([(controllerLength - holeSize) / 2, 0, 0])
		cube([holeSize, controllerTopSpacing + basePlateDepth, basePlateDepth]);
	}
}

module controller() {
	controllerBase();

	translate([controllerWidth + 1, 0, 0])
	controllerFace();

	translate([0, controllerLength + 1, 0])
	controllerBottomWall();

	translate([controllerWidth + 1, controllerLength + 1, 0])
	controllerTopWall();

	translate([0, controllerLength + 1 + controllerHeight + 1, 0])
	controllerSideWall();

	translate([controllerLength + 1, controllerLength + 1 + controllerHeight + 1, 0])
	controllerSideWall();

	pegTop = controllerLength + 1 + controllerHeight + 1;
	pegLeft = 2 * (controllerLength + 1);
	for (row = [0:1], column = [0:3]) {
		translate([pegLeft + column * (pegWidth + 1), pegTop + row * (pegOuterHeight + 1), 0])
		peg();
	}
}

module everything() {
	// core();
	// basePlate();
	// facePlate();
	// wristPad();
	mountingPlate();
	// upperLayer();
	// lowerLayer();
	// pcb();
	// pcbSpacing();
	// peg();
	// echoThumb2KeyLedPcbCutouts();
	// echoThumb2KeyPositions();
	// controllerBase();
	// controllerFace();
	// controllerBottomWall();
	// controllerTopWall();
	// controllerSideWall();
	// controller();
}

projection(cut = false) {
	color([0, 0, 1])
	everything();
}
