boardThickness = 3;
p2width = 384;
p2height = 384;

width = 345.5;
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

	hole(189.5, 6.5, 1.5, 10);
	hole(189.5, 21.5, 1.5, 10);
	hole(189.5, 36.5, 1.5, 10);
	hole(189.5, 51.5, 1.5, 10);
	hole(189.5, 79.5, 1.5, 10);
	hole(189.5, 94.5, 1.5, 10);
	hole(189.5, 109.5, 1.5, 10);
	hole(189.5, 124.5, 1.5, 10);

	hole(342.5, 6.5, 1.5, 10);
	hole(342.5, 21.5, 1.5, 10);
	hole(342.5, 36.5, 1.5, 10);
	hole(342.5, 51.5, 1.5, 10);
	hole(342.5, 79.5, 1.5, 10);
	hole(342.5, 94.5, 1.5, 10);
	hole(342.5, 109.5, 1.5, 10);
	hole(342.5, 124.5, 1.5, 10);

	hole(6.5, 1.5, 10, 1.5);
	hole(21.5, 1.5, 10, 1.5);
	hole(36.5, 1.5, 10, 1.5);
	hole(51.5, 1.5, 10, 1.5);
	hole(66.5, 1.5, 10, 1.5);
	hole(81.5, 1.5, 10, 1.5);
	hole(99.5, 1.5, 10, 1.5);
	hole(114.5, 1.5, 10, 1.5);
	hole(129.5, 1.5, 10, 1.5);
	hole(144.5, 1.5, 10, 1.5);
	hole(159.5, 1.5, 10, 1.5);
	hole(174.5, 1.5, 10, 1.5);

	hole(6.5, 138, 10, 1.5);
	hole(21.5, 138, 10, 1.5);
	hole(36.5, 138, 10, 1.5);
	hole(51.5, 138, 10, 1.5);
	hole(66.5, 138, 10, 1.5);
	hole(81.5, 138, 10, 1.5);
	hole(99.5, 138, 10, 1.5);
	hole(114.5, 138, 10, 1.5);
	hole(129.5, 138, 10, 1.5);
	hole(144.5, 138, 10, 1.5);
	hole(159.5, 138, 10, 1.5);
	hole(174.5, 138, 10, 1.5);

	hole(196, 1.5, 10, 1.5);
	hole(211, 1.5, 10, 1.5);
	hole(226, 1.5, 10, 1.5);
	hole(241, 1.5, 10, 1.5);
	hole(256, 1.5, 10, 1.5);
	hole(269, 1.5, 10, 1.5);
	hole(284, 1.5, 10, 1.5);
	hole(299, 1.5, 10, 1.5);
	hole(314, 1.5, 10, 1.5);
	hole(329, 1.5, 10, 1.5);

	hole(196, 138, 10, 1.5);
	hole(211, 138, 10, 1.5);
	hole(226, 138, 10, 1.5);
	hole(241, 138, 10, 1.5);
	hole(256, 138, 10, 1.5);
	hole(269, 138, 10, 1.5);
	hole(284, 138, 10, 1.5);
	hole(299, 138, 10, 1.5);
	hole(314, 138, 10, 1.5);
	hole(329, 138, 10, 1.5);
}
