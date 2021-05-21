depth = 1.5;
edge = 3;
baseSpacing = edge + 3 + 1;
pcbLeftWidth = 169.5;
pcbRightWidth = 341;
pcbHeight = 138;
leftWidth = pcbLeftWidth + 2 * baseSpacing;
rightWidth = pcbRightWidth + 2 * baseSpacing;
height = pcbHeight + 2 * baseSpacing;
notchBig = 5;
notchLittle = 3;
pieceSpacing = 3;
screwPadding = 10.5;
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

module handBase(width) {
    module horizontalNotch(left, top) {
        notch(left, top, notchBig, notchLittle);
    }
    
    module horizontalRow(top) {
        horizontalNotch(edge + notchLittle + edge, top);
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
        screw(width - padding, padding);
        screw(padding, height - padding);
        screw(width - padding, height - padding);
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
            translate([41.5, converterHeight / 2])
            cylinder(depth, d = 3, $fn = 12);
        }
    }

    converterBase();
}

module rightHandFace() {
    difference() {
        handBase(rightWidth);
        notch(52, 14, 134, 20);
        notch(190, 14, 58, 20);
        notch(252, 14, 77, 20);
        notch(33, 42, 153, 20);
        notch(190, 42, 58, 20);
        notch(252, 42, 77, 20);
        notch(23.5, 61, 162.5, 20);
        notch(190, 61, 58, 20);
        notch(252, 61, 77, 20);
        notch(28.25, 80, 157.75, 20);
        notch(252, 80, 77, 20);
        notch(37.75, 99, 148.25, 20);
        notch(209, 99, 20, 20);
        notch(252, 99, 77, 20);
        notch(14, 118, 172, 20);
        notch(190, 118, 58, 20);
        notch(252, 118, 77, 20);

        screw(36 + baseSpacing, 17 + baseSpacing); //left of F6
        screw(17 + baseSpacing, 45 + baseSpacing); //left of 7
        screw(21.75 + baseSpacing, 102 + baseSpacing); //left of N
        
        notch(181 + baseSpacing - notchBig / 2, edge, notchBig, notchLittle);
        screw(181 + baseSpacing, 3.5 + baseSpacing);
        screw(181 + baseSpacing, 31 + baseSpacing);
        screw(181 + baseSpacing, 83 + baseSpacing);
        screw(181 + baseSpacing, height - screwPadding);
        notch(181 + baseSpacing - notchBig / 2, height - edge - notchLittle, notchBig, notchLittle);
        
        notch(243 + baseSpacing - notchBig / 2, edge, notchBig, notchLittle);
        screw(243 + baseSpacing, 3.5 + baseSpacing);
        screw(243 + baseSpacing, 31 + baseSpacing);
        screw(243 + baseSpacing, 83 + baseSpacing);
        screw(243 + baseSpacing, height - screwPadding);
        notch(243 + baseSpacing - notchBig / 2, height - edge - notchLittle, notchBig, notchLittle);

        notch(rightWidth - 8, 24, 8, 55);
    }
}

module leftHandFace() {
    difference() {
        handBase(leftWidth);
        notch(26, 14, 20, 20);
        notch(64, 14, 96, 20);
        notch(26, 42, 134, 20);
        notch(26, 61, 124.5, 20);
        notch(59.25, 80, 96, 20);
        notch(26, 99, 138.75, 20);
        notch(26, 118, 143.5, 20);

        screw(48 + baseSpacing, 17 + baseSpacing); //between Esc and F1
        screw(36.125 + baseSpacing, 83 + baseSpacing); //where CapsLock would have been
        screw(152.5 + baseSpacing, 64 + baseSpacing); //right of T
        
        notch(0, 24, 8, 55);
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
        notch(pcbRightWidth - 11, 17, 11, 55);
        standoff(3.5, 3.5);
        standoff(pcbRightWidth - 3.5, 3.5);
        standoff(3.5, pcbHeight - 3.5);
        standoff(pcbRightWidth - 3.5, pcbHeight - 3.5);
        standoff(36, 17); //left of F6
        standoff(17, 45); //left of 7
        standoff(21.75, 102); //left of N
        standoff(181, 3.5);
        standoff(181, 31);
        standoff(181, 83);
        standoff(181, pcbHeight - 3.5);
        standoff(243, 3.5);
        standoff(243, 31);
        standoff(243, 83);
        standoff(243, pcbHeight - 3.5);
        
        keys([48, 67, 86, 105, 124, 143, 162, 186, 205, 224, 248, 267, 286, 305], 10);

        keys([29, 48, 67, 86, 105, 124, 186, 205, 224, 248, 267, 286, 305], 38);
		key2(152.5, 38); //backspace

        keys([19.5, 38.5, 57.5, 76.5, 95.5, 114.5, 133.5, 157.25, 186, 205, 224, 248, 267, 286], 57);
		key2vertical(305, 66.5); //plus

        keys([24.25, 43.25, 62.25, 81.25, 100.25, 119.25, 248, 267, 286], 76);
		key2(150.125, 76); //enter

        keys([33.75, 52.75, 71.75, 90.75, 109.75, 205, 248, 267, 286], 95);
		key2(145.375, 95); //shift
		key2vertical(305, 104.5); //enter

        keys([59.875, 88.375, 112.125, 135.875, 159.625, 186, 205, 224, 286], 114);
		key2(21.875, 114); //space
		key2(257.5, 114); //0
    }
}

module leftHandMount() {
    difference() {
        cube([pcbLeftWidth, pcbHeight, depth]);
        notch(0, 17, 11, 55);
        standoff(3.5, 3.5);
        standoff(pcbLeftWidth - 3.5, 3.5);
        standoff(3.5, pcbHeight - 3.5);
        standoff(pcbLeftWidth - 3.5, pcbHeight - 3.5);
        standoff(48, 17); //between Esc and F1
        standoff(36.125, 83); //where CapsLock would have been
        standoff(152.5, 64); //right of T

        keys([22, 60, 79, 98, 117, 136], 10);

        keys([22, 41, 60, 79, 98, 117, 136], 38);

        keys([26.75, 50.5, 69.5, 88.5, 107.5, 126.5], 57);

        keys([55.25, 74.25, 93.25, 112.25, 131.25], 76);

        keys([64.75, 83.75, 102.75, 121.75, 140.75], 95);
		key2(33.875, 95); //shift

        keys([24.375, 48.125, 71.875, 98], 114);
		key2(133.625, 114); //space
    }
}

module everything() {
    rightHandFace();
    translate([rightWidth + pieceSpacing, 0, 0])
    leftHandFace();
    translate([42 + pieceSpacing, 54 + pieceSpacing, 0])
    converter();
    translate([0, height + pieceSpacing, 0])
    rightHandMount();
    translate([pcbRightWidth + pieceSpacing, height + pieceSpacing, 0])
    leftHandMount();
}

projection() {
    color([0, 0, 1]);
    everything();
}
