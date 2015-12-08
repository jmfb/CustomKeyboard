boardThickness = 1.5;
border = 10;
spacing = 5;
keysize = 14;
additionalFunctionSpacing = 9;

keyCap1 = 18;
keyCap15 = 28;
keyCap175 = 33;
keyCap225 = 42;
keyCap125 = 23;

additional15 = (keyCap15 - keyCap1) / 2;
additional175 = (keyCap175 - keyCap1) / 2;
additional225 = (keyCap225 - keyCap1) / 2;
additional125 = (keyCap125 - keyCap1) / 2;

additionalSpaceBar = 23;

top0 = border;
top1 = top0 + keysize + spacing + additionalFunctionSpacing;
top2 = top1 + keysize + spacing;
top3 = top2 + keysize + spacing;
top4 = top3 + keysize + spacing;
top5 = top4 + keysize + spacing;

row0left0 = border;
row0left1 = row0left0 + 2 * (keysize + spacing);
row0left2 = row0left1 + keysize + spacing;
row0left3 = row0left2 + keysize + spacing;
row0left4 = row0left3 + keysize + spacing;
row0left5 = row0left4 + keysize + spacing;

row1left0 = border;
row1left1 = row1left0 + keysize + spacing;
row1left2 = row1left1 + keysize + spacing;
row1left3 = row1left2 + keysize + spacing;
row1left4 = row1left3 + keysize + spacing;
row1left5 = row1left4 + keysize + spacing;
row1left6 = row1left5 + keysize + spacing;

row2left0 = border + additional15;
row2left1 = row2left0 + keysize + spacing + additional15;
row2left2 = row2left1 + keysize + spacing;
row2left3 = row2left2 + keysize + spacing;
row2left4 = row2left3 + keysize + spacing;
row2left5 = row2left4 + keysize + spacing;

row3left0 = border + 2 * additional175 + keysize + spacing;
row3left1 = row3left0 + keysize + spacing;
row3left2 = row3left1 + keysize + spacing;
row3left3 = row3left2 + keysize + spacing;
row3left4 = row3left3 + keysize + spacing;

row4left0 = border + additional225;
row4left1 = row4left0 + keysize + spacing + additional225;
row4left2 = row4left1 + keysize + spacing;
row4left3 = row4left2 + keysize + spacing;
row4left4 = row4left3 + keysize + spacing;
row4left5 = row4left4 + keysize + spacing;

row5left0 = border + additional125;
row5left1 = row5left0 + keysize + spacing + 2 * additional125;
row5left2 = row5left1 + keysize + spacing + 2 * additional125;
row5left3 = row3left3 + additionalSpaceBar;

width = row5left3 + keysize + additional225 + border;
height = top5 + keysize + border;

cableWidth = 9 + 2 * 2;
cableLeft = width - cableWidth;
cableHeight = 20 * 2 + 15 + 2 * 2;

teensyWidth = 8 + 31 + 1;
teensyHeight = 18 + 1;
teensyTop = top3 - (teensyHeight - keysize) / 2;

module keySwitch(x, y)
{
	translate([x, y, 0])
	cube([keysize, keysize, boardThickness]);
}

projection()
difference()
{
	cube([width, height, boardThickness]);

	keySwitch(row0left0, top0);
	keySwitch(row0left1, top0);
	keySwitch(row0left2, top0);
	keySwitch(row0left3, top0);
	keySwitch(row0left4, top0);
	keySwitch(row0left5, top0);

	keySwitch(row1left0, top1);
	keySwitch(row1left1, top1);
	keySwitch(row1left2, top1);
	keySwitch(row1left3, top1);
	keySwitch(row1left4, top1);
	keySwitch(row1left5, top1);
	keySwitch(row1left6, top1);

	keySwitch(row2left0, top2);
	keySwitch(row2left1, top2);
	keySwitch(row2left2, top2);
	keySwitch(row2left3, top2);
	keySwitch(row2left4, top2);
	keySwitch(row2left5, top2);

	keySwitch(row3left0, top3);
	keySwitch(row3left1, top3);
	keySwitch(row3left2, top3);
	keySwitch(row3left3, top3);
	keySwitch(row3left4, top3);

	keySwitch(row4left0, top4);
	keySwitch(row4left1, top4);
	keySwitch(row4left2, top4);
	keySwitch(row4left3, top4);
	keySwitch(row4left4, top4);
	keySwitch(row4left5, top4);

	keySwitch(row5left0, top5);
	keySwitch(row5left1, top5);
	keySwitch(row5left2, top5);
	keySwitch(row5left3, top5);

	translate([cableLeft, 0, 0])
	cube([cableWidth, cableHeight, boardThickness]);

	translate([0, teensyTop, 0])
	cube([teensyWidth, teensyHeight, boardThickness]);
}
