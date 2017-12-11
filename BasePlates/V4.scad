depth = 3;
edge = 3;
baseSpacing = edge + depth + 1;
leftWidth = 343.5 + 2 * baseSpacing;
rightWidth = 372 + 2 * baseSpacing;
wristpad = 100;
baseHeight = 150 + 2 * baseSpacing;
height = baseHeight + wristpad;
wallHeight = 20.5;
faceDepth = 1.5;
horizontalMiddle = 179;
verticalMiddle = 102;
notchBig = 5;
notchLittle = 3;
wallNotchHeight = (wallHeight - faceDepth - depth) / 2;
pieceSpacing = 3;
screwPadding = 10.5;
dsubWidth = 55;
dsubHeight = 9.5;
notchOffset = 2 * edge + depth;

module notch(x, y, cx, cy) {
    translate([x, y, 0])
    cube([cx, cy, depth]);
}

module screw(x, y) {
    translate([x, y])
    cylinder(depth, 1.2, $fn = 12);
}

module handBase(width) {
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
        verticalNotch(left, baseHeight - edge - notchLittle - edge - notchBig);
    }
    
    module notches() {
        horizontalRow(edge);
        horizontalRow(baseHeight - edge - notchLittle);
        verticalRow(edge);
        verticalRow(width - edge - notchLittle);
    }
    
    module screws() {
        padding = screwPadding;
        screw(padding, padding);
        screw(horizontalMiddle, padding);
        screw(width - padding, padding);
        screw(padding, baseHeight - padding);
        screw(horizontalMiddle, baseHeight - padding);
        screw(width - padding, baseHeight - padding);
        screw(horizontalMiddle, verticalMiddle);
    }
    
    difference() {
        cube([width, height, depth]);
        notches();
        screws();
    }
}

module handTopAndBottom(width) {
    module notches(top, cy) {
        notch(0, top, notchOffset, cy);
        notch(notchOffset + notchBig, top, horizontalMiddle - notchBig / 2 - notchOffset - notchBig, cy);
        notch(horizontalMiddle + notchBig / 2, top, width - horizontalMiddle - notchBig / 2 - notchOffset - notchBig, cy);
        notch(width - notchOffset, top, notchOffset, cy);
    }
    module topNotches() {
        notches(0, faceDepth);
    }
    module bottomNotches() {
        notches(wallHeight - depth, depth);
    }
    
    module wallNotches() {
        notch(edge, faceDepth, depth, wallNotchHeight);
        notch(width - edge - depth, faceDepth, depth, wallNotchHeight);
    }

    module handTop() {
        module dsubNotch() {
            notch(68, faceDepth, dsubWidth, dsubHeight); 
        }
            
        difference() {
            cube([width, wallHeight, depth]);
            topNotches();
            bottomNotches();
            wallNotches();
            dsubNotch();
        }
    }

    module handBottom() {
        difference() {
            cube([width, wallHeight, depth]);
            topNotches();
            bottomNotches();
            wallNotches();
        }
    }

    handTop();
    translate([0, wallHeight + pieceSpacing, 0])
    handBottom();
}

module handLeftAndRight() {
    module notches(top, cy) {
        notch(0, top, notchOffset, cy);
        notch(notchOffset + notchBig, top, baseHeight - 2 * (notchOffset + notchBig), cy);
        notch(baseHeight - notchOffset, top, notchOffset, cy);
    }
    
    module topNotches() {
        notches(0, faceDepth);
    }
    
    module bottomNotches() {
        notches(wallHeight - depth, depth);
    }

    module wallNotches() {
        notch(edge, faceDepth + wallNotchHeight, depth, wallNotchHeight);
        notch(baseHeight - edge - depth, faceDepth + wallNotchHeight, depth, wallNotchHeight);
    }
    
    module handLeft() {
        difference() {
            cube([baseHeight, wallHeight, depth]);
            topNotches();
            bottomNotches();
            wallNotches();
        }
    }
    
    module handRight() {
        difference() {
            cube([baseHeight, wallHeight, depth]);
            topNotches();
            bottomNotches();
            wallNotches();
        }
    }
    
    handLeft();
    translate([0, wallHeight + pieceSpacing, 0])
    handRight();
}

module handWalls(width) {
    handTopAndBottom(width);
    translate([0, 2 * (pieceSpacing + wallHeight), 0])
    handLeftAndRight();
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
        }
    }
    
    module converterTop() {
        module topNotches(top, cy) {
            notch(0, top, notchOffset, cy);
            notch(notchOffset + notchBig, top, converterWidth - 2 * (notchOffset + notchBig), cy);
            notch(converterWidth - notchOffset, top, notchOffset, cy);
        }
        
        module notches() {
            topNotches(0, faceDepth);
            topNotches(wallHeight - depth, depth);
        }
        
        module wallNotches() {
            notch(edge, faceDepth, depth, wallNotchHeight);
            notch(converterWidth - edge - depth, faceDepth, depth, wallNotchHeight);
        }
        
        module dsubNotch() {
            notch(51.5, faceDepth, dsubWidth, dsubHeight);
        }
        
        difference() {
            cube([converterWidth, wallHeight, depth]);
            notches();
            wallNotches();
            dsubNotch();
        }
    }
    
    module converterTopAndBottom() {
        converterTop();
        translate([0, wallHeight + pieceSpacing, 0])
        converterTop();
    }
    
    module converterRight() {
        module topNotches(top, cy) {
            notch(top, 0, cy, notchOffset);
            notch(top, notchOffset + notchBig, cy, converterHeight - 2 * (notchOffset + notchBig));
            notch(top, converterHeight - notchOffset, cy, notchOffset);
        }
        
        module notches() {
            topNotches(0, faceDepth);
            topNotches(wallHeight - depth, depth);
        }
        
        module wallNotches() {
            notch(faceDepth + wallNotchHeight, edge, wallNotchHeight, depth);
            notch(faceDepth + wallNotchHeight, converterHeight - edge - depth, wallNotchHeight, depth);
        }
        
        difference() {
            cube([wallHeight, converterHeight, depth]);
            notches();
            wallNotches();
        }
    }
    
    module converterLeft() {
        module teensyNotch() {
            teensyWidth = 18;
            notch(faceDepth, (converterHeight - teensyWidth) / 2, dsubHeight, teensyWidth);
        }
        
        difference() {
            converterRight();
            teensyNotch();
        }
    }
    
    module converterLeftAndRight() {
        converterLeft();
        translate([wallHeight + pieceSpacing, 0, 0])
        converterRight();
    }
    
    converterBase();
    translate([0, converterHeight + pieceSpacing, 0])
    converterTopAndBottom();
    translate([converterWidth + pieceSpacing, 0, 0])
    converterLeftAndRight();
}

projection() {
    handBase(rightWidth);
    translate([0, height + pieceSpacing, 0])
    handTopAndBottom(rightWidth);
    translate([0, height + 3 * pieceSpacing + 2 * wallHeight, 0])
    handTopAndBottom(leftWidth);
    translate([rightWidth + pieceSpacing, height + pieceSpacing, 0])
    handLeftAndRight();
    translate([rightWidth + pieceSpacing, height + 3 * pieceSpacing + 2 * wallHeight, 0])
    handLeftAndRight();
    translate([rightWidth + pieceSpacing, 0, 0])
    handBase(leftWidth);
    translate([rightWidth + baseHeight + 2 * pieceSpacing, height + pieceSpacing, 0])
    converter();
}
