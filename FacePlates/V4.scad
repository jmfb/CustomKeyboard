depth = 1.5;
edge = 3;
baseSpacing = edge + 3 + 1;
pcbLeftWidth = 343.5;
pcbRightWidth = 371;
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
    converterHeight = 67;
    converterWidth = 130.5;
    
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
    notch(x, y, 14, 14);
}

//TODO: mounting plate cutouts for 2, 2.25, 2.75 key stabilizers

module rightHandMount() {
    difference() {
        cube([pcbRightWidth, pcbHeight, depth]);
        notch(pcbRightWidth - 61 - 55, 0, 55, 1);
        standoff(3.5, 3.5);
        standoff(pcbRightWidth - 3.5, 3.5);
        standoff(3.5, pcbHeight - 3.5);
        standoff(pcbRightWidth - 3.5, pcbHeight - 3.5);
        standoff(pcbRightWidth - 172, 3.5);
        standoff(pcbRightWidth - 172, pcbHeight - 3.5);
        standoff(pcbRightWidth - 172, 95);
        
        key(10, 10 + 12);
        key(29, 10 + 12);
        key(67, 10 + 12);
        key(86, 10 + 12);
        key(105, 10 + 12);
        key(124, 10 + 12);
        key(143, 10 + 12);
        key(162, 10 + 12);
        key(181, 10 + 12);
        key(205, 10 + 12);
        key(224, 10 + 12);
        key(243, 10 + 12);
        key(267, 10 + 12);
        key(286, 10 + 12);
        key(305, 10 + 12);
        key(324, 10 + 12);
        key(348, 10 + 12);
        
		key(19.5, 38 + 12);
		key(48, 38 + 12);
		key(67, 38 + 12);
		key(86, 38 + 12);
		key(105, 38 + 12);
		key(124, 38 + 12);
		key(143, 38 + 12);
		key(171.5, 38 + 12);
		key(205, 38 + 12);
		key(224, 38 + 12);
		key(243, 38 + 12);
		key(267, 38 + 12);
		key(286, 38 + 12);
		key(305, 38 + 12);
		key(324, 38 + 12);
		key(348, 38 + 12);

		key(14.75, 57 + 12);
		key(38.5, 57 + 12);
		key(57.5, 57 + 12);
		key(76.5, 57 + 12);
		key(95.5, 57 + 12);
		key(114.5, 57 + 12);
		key(133.5, 57 + 12);
		key(152.5, 57 + 12);
		key(176.25, 57 + 12);
		key(205, 57 + 12);
		key(224, 57 + 12);
		key(243, 57 + 12);
		key(267, 57 + 12);
		key(286, 57 + 12);
		key(305, 57 + 12);
		key(324, 57 + 9.5 + 12);
		key(348, 57 + 12);

		key(17.125, 76 + 12);
		key(43.25, 76 + 12);
		key(62.25, 76 + 12);
		key(81.25, 76 + 12);
		key(100.25, 76 + 12);
		key(119.25, 76 + 12);
		key(138.25, 76 + 12);
		key(169.125, 76 + 12);
		key(267, 76 + 12);
		key(286, 76 + 12);
		key(305, 76 + 12);
		key(348, 76 + 12);

		key(21.875, 95 + 12);
		key(52.75, 95 + 12);
		key(71.75, 95 + 12);
		key(90.75, 95 + 12);
		key(109.75, 95 + 12);
		key(128.75, 95 + 12);
		key(164.375, 95 + 12);
		key(224, 95 + 12);
		key(267, 95 + 12);
		key(286, 95 + 12);
		key(305, 95 + 12);
		key(324, 95 + 9.5 + 12);
		key(348, 95 + 12);

		key(10, 114 + 12);
		key(40.875, 114 + 12);
		key(78.875, 114 + 12);
		key(107.375, 114 + 12);
		key(131.125, 114 + 12);
		key(154.875, 114 + 12);
		key(178.625, 114 + 12);
		key(205, 114 + 12);
		key(224, 114 + 12);
		key(243, 114 + 12);
		key(276.5, 114 + 12);
		key(305, 114 + 12);
		key(348, 114 + 12);
    }
}

module leftHandMount() {
    difference() {
        cube([pcbLeftWidth, pcbHeight, depth]);
        notch(61, 0, 55, 1);
        standoff(3.5, 3.5);
        standoff(pcbLeftWidth - 3.5, 3.5);
        standoff(3.5, pcbHeight - 3.5);
        standoff(pcbLeftWidth - 3.5, pcbHeight - 3.5);
        standoff(172, 3.5);
        standoff(172, pcbHeight - 3.5);
        standoff(pcbLeftWidth - 172, 95);

		key(10, 10 + 12);
		key(34, 10 + 12);
		key(53, 10 + 12);
		key(72, 10 + 12);
		key(91, 10 + 12);
		key(115, 10 + 12);
		key(134, 10 + 12);
		key(153, 10 + 12);
		key(177, 10 + 12);
		key(215, 10 + 12);
		key(234, 10 + 12);
		key(253, 10 + 12);
		key(272, 10 + 12);
		key(291, 10 + 12);
		key(319.5, 10 + 12);

		key(10, 38 + 12);
		key(34, 38 + 12);
		key(53, 38 + 12);
		key(72, 38 + 12);
		key(91, 38 + 12);
		key(115, 38 + 12);
		key(134, 38 + 12);
		key(153, 38 + 12);
		key(177, 38 + 12);
		key(196, 38 + 12);
		key(215, 38 + 12);
		key(234, 38 + 12);
		key(253, 38 + 12);
		key(272, 38 + 12);
		key(291, 38 + 12);
		key(314.75, 38 + 12);

		key(10, 57 + 12);
		key(34, 57 + 9.5 + 12);
		key(53, 57 + 12);
		key(72, 57 + 12);
		key(91, 57 + 12);
		key(115, 57 + 12);
		key(134, 57 + 12);
		key(153, 57 + 12);
		key(181.75, 57 + 12);
		key(205.5, 57 + 12);
		key(224.5, 57 + 12);
		key(243.5, 57 + 12);
		key(262.5, 57 + 12);
		key(281.5, 57 + 12);
		key(310, 57 + 12);

		key(10, 76 + 12);
		key(53, 76 + 12);
		key(72, 76 + 12);
		key(91, 76 + 12);
		key(184.125, 76 + 12);
		key(210.25, 76 + 12);
		key(229.25, 76 + 12);
		key(248.25, 76 + 12);
		key(267.25, 76 + 12);
		key(286.25, 76 + 12);
		key(312.375, 76 + 12);

		key(10, 95 + 12);
		key(34, 95 + 9.5 + 12);
		key(53, 95 + 12);
		key(72, 95 + 12);
		key(91, 95 + 12);
		key(134, 95 + 12);
		key(188.875, 95 + 12);
		key(219.75, 95 + 12);
		key(238.75, 95 + 12);
		key(257.75, 95 + 12);
		key(276.75, 95 + 12);
		key(295.75, 95 + 12);
		key(317.125, 95 + 12);

		key(10, 114 + 12);
		key(53, 114 + 12);
		key(81.5, 114 + 12);
		key(115, 114 + 12);
		key(134, 114 + 12);
		key(153, 114 + 12);
		key(179.375, 114 + 12);
		key(203.125, 114 + 12);
		key(226.875, 114 + 12);
		key(253, 114 + 12);
		key(288.625, 114 + 12);
		key(319.5, 114 + 12);
    }
}

projection() {
    rightHandFace();
    translate([rightWidth + pieceSpacing, 0, 0])
    leftHandFace();
    translate([14 + pieceSpacing, 54 + pieceSpacing, 0])
    converter();
    translate([590, 54 + pieceSpacing, 0])
    converter();
    translate([0, height + pieceSpacing, 0])
    rightHandMount();
    translate([pcbRightWidth + pieceSpacing, height + pieceSpacing, 0])
    leftHandMount();
}
