boardThickness = 1.5;
width = 188;
height = 138;

module hole(x, y, cx, cy)
{
	translate([x, y, 0])
	cube([cx, cy, boardThickness]);
}

projection()
difference()
{
	cube([width, height, boardThickness]);
	hole(0, 0, 39, 13);
	hole(0, 13, 13, 59);
	hole(47, 7, 134, 20);
	hole(32, 35, 149, 77);
	hole(28, 35, 4, 19);
	hole(18, 54, 14, 20);
	hole(23, 74, 9, 19);
	hole(7, 111, 44, 20);
	hole(107, 112, 74, 19);
}
