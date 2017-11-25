boardThickness = 3;
p2width = 384;
p2height = 384;

width = 163;
height = 211;

module hole(x, y, cx, cy)
{
	translate([x, y, 0])
	cube([cx, cy, boardThickness]);
}

projection()
difference()
{
	cube([p2width, p2height, boardThickness]);
	hole(width, 0, p2width, p2height);
	hole(0, height, p2width, 10);
	hole(0, height + 80, p2width, 10);
	hole(0, height	+ 160, p2width, p2height);
	hole(1.5, 6.5, 1.5, 10);
	hole(1.5, 21.5, 1.5, 10);
	hole(1.5, 36.5, 1.5, 10);
	hole(1.5, 51.5, 1.5, 10);
	
	hole(1.5, 79.5, 1.5, 10);
	hole(1.5, 94.5, 1.5, 10);
	hole(1.5, 109.5, 1.5, 10);
	hole(1.5, 124.5, 1.5, 10);

	hole(160, 6.5, 1.5, 10);
	hole(160, 21.5, 1.5, 10);
	hole(160, 36.5, 1.5, 10);
	hole(160, 51.5, 1.5, 10);
	
	hole(160, 79.5, 1.5, 10);
	hole(160, 94.5, 1.5, 10);
	hole(160, 109.5, 1.5, 10);
	hole(160, 124.5, 1.5, 10);

	hole(6.5, 1.5, 10, 1.5);
	hole(21.5, 1.5, 10, 1.5);
	hole(36.5, 1.5, 10, 1.5);
	hole(51.5, 1.5, 10, 1.5);
	hole(66.5, 1.5, 10, 1.5);

	hole(86.5, 1.5, 10, 1.5);
	hole(101.5, 1.5, 10, 1.5);
	hole(116.5, 1.5, 10, 1.5);
	hole(131.5, 1.5, 10, 1.5);
	hole(146.5, 1.5, 10, 1.5);

	hole(6.5, 138, 10, 1.5);
	hole(21.5, 138, 10, 1.5);
	hole(36.5, 138, 10, 1.5);
	hole(51.5, 138, 10, 1.5);
	hole(66.5, 138, 10, 1.5);

	hole(86.5, 138, 10, 1.5);
	hole(101.5, 138, 10, 1.5);
	hole(116.5, 138, 10, 1.5);
	hole(131.5, 138, 10, 1.5);
	hole(146.5, 138, 10, 1.5);

}
