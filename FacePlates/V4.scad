depth = 1.5;
edge = 3;
baseSpacing = edge + 3 + 1;
pcbLeftWidth = 343.5;
pcbRightWidth = 372;
pcbHeight = 150;
leftWidth = pcbLeftWidth + 2 * baseSpacing;
rightWidth = pcbRightWidth + 2 * baseSpacing;
height = pcbHeight + 2 * baseSpacing;
verticalMiddle = 102;
notchBig = 5;
notchLittle = 3;
pieceSpacing = 3;
screwPadding = 10.5;
dsubWidth = 55;
notchOffset = 2 * edge + depth;
keyHoleSize = 14;
converterHeight = 67;
converterWidth = 130.5;

module notch(x, y, cx, cy) {
    translate([x, y, 0])
    cube([cx, cy, depth]);
}

module screw(x, y) {
    translate([x, y])
    cylinder(depth, d = 2.4, $fn = 12);
}

module standoff(x, y) {
    translate([x, y])
    cylinder(depth, d = 3.5, $fn = 12);
}

module handBase(width, horizontalMiddle) {
    module horizontalNotch(left, top) {
        notch(left, top, notchBig, notchLittle);
    }
    
    module horizontalRow(top) {
        horizontalNotch(edge + notchLittle + edge, top);
        horizontalNotch(horizontalMiddle - (notchBig / 2), top);
        horizontalNotch(width - edge - notchLittle - edge - notchBig, top);
    }
    
    module verticalNotch(left, top) {
        notch(left, top, notchLittle, notchBig);
    }
    
    module verticalRow(left) {
        verticalNotch(left, edge + notchLittle + edge);
        verticalNotch(left, height - edge - notchLittle - edge - notchBig);
    }
    
    module notches() {
        horizontalRow(edge);
        horizontalRow(height - edge - notchLittle);
        verticalRow(edge);
        verticalRow(width - edge - notchLittle);
    }
    
    module screws() {
        padding = screwPadding;
        screw(padding, padding);
        screw(horizontalMiddle, padding);
        screw(width - padding, padding);
        screw(padding, height - padding);
        screw(horizontalMiddle, height - padding);
        screw(width - padding, height - padding);
        screw(horizontalMiddle, verticalMiddle);
    }
    
    difference() {
        cube([width, height, depth]);
        notches();
        screws();
    }
}

module converter() {
    module converterBase() {
        module horizontalNotch(left, top) {
            notch(left, top, notchBig, notchLittle);
        }
        
        module horizontalRow(top) {
            horizontalNotch(edge + notchLittle + edge, top);
            horizontalNotch(converterWidth - edge - notchLittle - edge - notchBig, top);
        }
        
        module verticalNotch(left, top) {
            notch(left, top, notchLittle, notchBig);
        }
        
        module verticalRow(left) {
            verticalNotch(left, edge + notchLittle + edge);
            verticalNotch(left, converterHeight - edge - notchLittle - edge - notchBig);
        }
        
        module notches() {
            horizontalRow(edge);
            horizontalRow(converterHeight - edge - notchLittle);
            verticalRow(edge);
            verticalRow(converterWidth - edge - notchLittle);
        }
        
        module screws() {
            padding = screwPadding;
            screw(padding, padding);
            screw(converterWidth - padding, padding);
            screw(padding, converterHeight - padding);
            screw(converterWidth - padding, converterHeight - padding);
        }

        difference() {
            cube([converterWidth, converterHeight, depth]);
            notches();
            screws();
            notch(9 + 5 + 37.5, 0, 55, 8);
            notch(9 + 5 + 37.5, 67 - 8, 55, 8);
        }
    }

    converterBase();
}

module converter2() {
    centerX = converterWidth / 2;
    centerY = converterHeight / 2;

    holeRadius = 2;
    
    module hole(x, y) {
        translate([x, y])
        cylinder(depth, d = holeRadius * 2, $fn = 12);
    }
    
    module bar(xc, yc, rz) {
        barWidth = 20.4;
        barHeight = holeRadius * 2;
        
        translate([xc, yc, 0])
        rotate([0, 0, rz])
        translate([-barWidth / 2, -barHeight / 2, 0])
        cube([barWidth, barHeight, depth]);
    }

    module fluxCapacitor() {
        holeRadius = 2;
        centerHoleX = 57.5;
        epsilon = 0.3;

        hole(centerHoleX, centerY);
        hole(29.1, centerY);
        hole(49.5, centerY);
        hole(77 + epsilon, 14 - epsilon);
        hole(77 + epsilon, converterHeight - 14 + epsilon);
        hole(57.5 + 5.66, centerY - 5.66);
        hole(57.5 + 5.66, centerY + 5.66);
        
        bar(39.3, centerY);
        bar(centerHoleX + 12.87, centerY - 12.87, -45);
        bar(centerHoleX + 12.87, centerY + 12.87, 45);
    }

    difference() {
        converter();
        fluxCapacitor();
    }
}

module rightHandFace() {
    difference() {
        handBase(rightWidth, rightWidth - 179);
        notch(14, 26, 39, 20);
        notch(71, 26, 134, 20);
        notch(14, 54, 191, 96);
        notch(209, 26, 58, 20);
        notch(209, 54, 58, 39);
        notch(228, 111, 20, 19);
        notch(209, 130, 58, 20);
        notch(271, 26, 77, 20);
        notch(271, 54, 77, 96);
        notch(352, 26, 20, 20);
        notch(352, 54, 20, 96);
        notch(rightWidth - 55 - 61 - 7, 0, 55, 8);
    }
}

module leftHandFace() {
    difference() {
        handBase(leftWidth, 179);
        notch(14, 26, 20, 20);
        notch(14, 54, 20, 96);
        notch(38, 26, 77, 20);
        notch(38, 54, 77, 96);
        notch(119, 26, 58, 20);
        notch(119, 54, 58, 39);
        notch(138, 111, 20, 19);
        notch(119, 130, 58, 20);
        notch(181, 26, 20, 20);
        notch(219, 26, 96, 20);
        notch(323.5, 26, 20, 20);
        notch(181, 54, 162.5, 96);
        notch(7 + 61, 0, 55, 8);
    }
}

module key(x, y) {
    notch(x, y, keyHoleSize, keyHoleSize);
}

module keys(xs, y) {
    for (x = xs) {
        key(x, y);
    }
}

module key2(x, y) {
    stabilizerWidth = 23.8;
    centerX = x + keyHoleSize / 2;
    centerY = y + keyHoleSize / 2;
    
    sideWidth = 6.65;
    sideHeight = 12.3;
    
    key(x, y);
    notch(
        centerX + stabilizerWidth / 2 - sideWidth / 2,
        centerY - 5.53,
        sideWidth,
        sideHeight);
    notch(
        centerX - stabilizerWidth / 2 - sideWidth / 2,
        centerY - 5.53,
        sideWidth,
        sideHeight);
    notch(
        centerX + stabilizerWidth / 2 + sideWidth / 2,
        centerY - 2.3,
        0.875,
        2.8);
    notch(
        centerX - stabilizerWidth / 2 - sideWidth / 2 - 0.875,
        centerY - 2.3,
        0.875,
        2.8);
    notch(
        centerX + stabilizerWidth / 2 - 1.5,
        centerY + 6.77,
        3,
        1.2);
    notch(
        centerX - stabilizerWidth / 2 - 1.5,
        centerY + 6.77,
        3,
        1.2);
    notch(
        centerX + keyHoleSize / 2,
        centerY - 4.73,
        1.575,
        10.7);
    notch(
        centerX - keyHoleSize / 2 - 1.575,
        centerY - 4.73,
        1.575,
        10.7);
}

module key2vertical(x, y) {
    centerX = x + keyHoleSize / 2;
    centerY = y + keyHoleSize / 2;
    translate([centerX, centerY])
    rotate([0, 0, -90])
    translate([-centerX, -centerY, 0])
    key2(x, y);
}

module rightHandMount() {
    difference() {
        cube([pcbRightWidth, pcbHeight, depth]);
        notch(pcbRightWidth - 61 - 55, 0, 55, 11);
        standoff(3.5, 3.5);
        standoff(pcbRightWidth - 3.5, 3.5);
        standoff(3.5, pcbHeight - 3.5);
        standoff(pcbRightWidth - 3.5, pcbHeight - 3.5);
        standoff(pcbRightWidth - 172, 3.5);
        standoff(pcbRightWidth - 172, pcbHeight - 3.5);
        standoff(pcbRightWidth - 172, 95);
        
        keys([10, 29, 67, 86, 105, 124, 143, 162, 181, 205, 224, 243, 267, 286, 305, 324, 348], 22);

        keys([48, 67, 86, 105, 124, 143, 205, 224, 243, 267, 286, 305, 324, 348], 50);
		key2(19.5, 50); //r3
		key2(171.5, 50); //backspace

        keys([14.75, 38.5, 57.5, 76.5, 95.5, 114.5, 133.5, 152.5, 176.25, 205, 224, 243, 267, 286, 305, 348], 69);
		key2vertical(324, 78.5); //plus

        keys([17.125, 43.25, 62.25, 81.25, 100.25, 119.25, 138.25, 267, 286, 305, 348], 88);
		key2(169.125, 88); //enter

        keys([52.75, 71.75, 90.75, 109.75, 128.75, 224, 267, 286, 305, 348], 107);
		key2(21.875, 107); //r6
		key2(164.375, 107); //shift
		key2vertical(324, 116.5); //enter

        keys([10, 78.875, 107.375, 131.125, 154.875, 178.625, 205, 224, 243, 305, 348], 126);
		key2(40.875, 126); //space
		key2(276.5, 126); //0
    }
}

module leftHandMount() {
    difference() {
        cube([pcbLeftWidth, pcbHeight, depth]);
        notch(61, 0, 55, 11);
        standoff(3.5, 3.5);
        standoff(pcbLeftWidth - 3.5, 3.5);
        standoff(3.5, pcbHeight - 3.5);
        standoff(pcbLeftWidth - 3.5, pcbHeight - 3.5);
        standoff(172, 3.5);
        standoff(172, pcbHeight - 3.5);
        standoff(pcbLeftWidth - 172, 95);

        keys([10, 34, 53, 72, 91, 115, 134, 153, 177, 215, 234, 253, 272, 291, 319.5], 22);

        keys([10, 34, 53, 72, 91, 115, 134, 153, 177, 196, 215, 234, 253, 272, 291, 314.75], 50);

        keys([10, 53, 72, 91, 115, 134, 153, 181.75, 205.5, 224.5, 243.5, 262.5, 281.5], 69);
		key2vertical(34, 57 + 9.5 + 12); //plus
		key2(310, 57 + 12); //l3

        keys([10, 53, 72, 91, 184.125, 210.25, 229.25, 248.25, 267.25, 286.25, 312.375], 88);

        keys([10, 53, 72, 91, 134, 219.75, 238.75, 257.75, 276.75, 295.75, 317.125], 107);
		key2vertical(34, 95 + 9.5 + 12); //enter
		key2(188.875, 95 + 12); //shift

        keys([10, 53, 115, 134, 153, 179.375, 203.125, 226.875, 253, 319.5], 126);
		key2(81.5, 114 + 12); //0
		key2(288.625, 114 + 12); //space
    }
}

module everything() {
    rightHandFace();
    translate([rightWidth + pieceSpacing, 0, 0])
    leftHandFace();
    translate([14 + pieceSpacing, 54 + pieceSpacing, 0])
    converter();
    translate([590, 54 + pieceSpacing, 0])
    converter2();
    translate([0, height + pieceSpacing, 0])
    rightHandMount();
    translate([pcbRightWidth + pieceSpacing, height + pieceSpacing, 0])
    leftHandMount();
}

projection() {
    everything();
}
