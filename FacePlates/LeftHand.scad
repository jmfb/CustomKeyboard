boardThickness = 1.5;
width = 160;
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
	hole(7, 7, 20, 20);
	hole(45, 7, 96, 20);
	hole(147, 0, 13, 59);
	hole(41, 35, 91, 77);
	hole(7, 35, 34, 39);
	hole(7, 92, 74, 39);
	hole(132, 35, 9, 20);
	hole(132, 73, 5, 19);
	hole(132, 92, 14, 19);
	//TODO: Figure out hole for spacebar
	hole(110, 111, 44, 20); 
}
