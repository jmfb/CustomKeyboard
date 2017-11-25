boardThickness = 1.5;
width = 153;
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
	hole(114, 0, 39, 13);
	hole(7, 7, 58, 20);
	hole(7, 35, 58, 39);
	hole(26, 92, 20, 19);
	hole(7, 111, 58, 20);
	hole(69, 35, 77, 96);
}
