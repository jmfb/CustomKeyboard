boardThickness = 1.5;
border = 10;
spacing = 5;
keysize = 14;
additionalFunctionSpacing = 9;
additionalNumberPadSpacing = 5;
doubleSpacing = (keysize + spacing) / 2;

left0 = border;
left1 = left0 + keysize + spacing;
left2 = left1 + keysize + spacing;
left3 = left2 + keysize + spacing + additionalNumberPadSpacing;
left4 = left3 + keysize + spacing;
left5 = left4 + keysize + spacing;
left6 = left5 + keysize + spacing;

top0 = border;
top1 = top0 + keysize + spacing + additionalFunctionSpacing;
top2 = top1 + keysize + spacing;
top3 = top2 + keysize + spacing;
top4 = top3 + keysize + spacing;
top5 = top4 + keysize + spacing;

width = left6 + keysize + border;
height = top5 + keysize + border;

cableWidth = 39;
cableLeft = width - cableWidth;
cableHeight = 13;

module keySwitch(x, y)
{
	translate([x, y, 0])
	cube([keysize, keysize, boardThickness]);
}


projection()
difference()
{
	cube([width, height, boardThickness]);

	keySwitch(left0, top0);
	keySwitch(left1, top0);
	keySwitch(left2, top0);

	keySwitch(left0, top1);
	keySwitch(left1, top1);
	keySwitch(left2, top1);

	keySwitch(left0, top2);
	keySwitch(left1, top2);
	keySwitch(left2, top2);

	keySwitch(left1, top4);

	keySwitch(left0, top5);
	keySwitch(left1, top5);
	keySwitch(left2, top5);

	keySwitch(left3, top1);
	keySwitch(left4, top1);
	keySwitch(left5, top1);
	keySwitch(left6, top1);

	keySwitch(left3, top2);
	keySwitch(left4, top2);
	keySwitch(left5, top2);
	keySwitch(left6, top2 + doubleSpacing);

	keySwitch(left3, top3);
	keySwitch(left4, top3);
	keySwitch(left5, top3);

	keySwitch(left3, top4);
	keySwitch(left4, top4);
	keySwitch(left5, top4);
	keySwitch(left6, top4 + doubleSpacing);

	keySwitch(left3 + doubleSpacing, top5);
	keySwitch(left5, top5);

	translate([cableLeft, 0, 0])
	cube([cableWidth, cableHeight, boardThickness]);
}
