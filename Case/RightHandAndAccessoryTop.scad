rightHandBoard = 188;
accessoryBoard = 153;
edge = 1.5;

depth = 2 * edge;
width = rightHandBoard + accessoryBoard + 3 * edge;

groove = 1.5;
faceSpace = 5;
mountSpace = 3;
pcbSpace = 5;

notchSpacing = 5;
notchWidth = 10;
notchHeight = 3;

height =
	edge +
	groove +
	faceSpace +
	groove +
	mountSpace +
	groove +
	pcbSpace +
	notchHeight;

faceTop = edge;
mountTop = faceTop + groove + faceSpace;
pcbTop = mountTop + groove + mountSpace;
baseTop = pcbTop + groove + pcbSpace;

rightHandNotchCount = 12;
accessoryNotchCount = 10;
rightHandNotchGap = rightHandBoard - rightHandNotchCount * (notchSpacing + notchWidth);
accessoryNotchGap = accessoryBoard - accessoryNotchCount * (notchSpacing + notchWidth);
notch1Left = edge + notchSpacing;
notch2Left = notch1Left + notchSpacing + notchWidth;
notch3Left = notch2Left + notchSpacing + notchWidth;
notch4Left = notch3Left + notchSpacing + notchWidth;
notch5Left = notch4Left + notchSpacing + notchWidth;
notch6Left = notch5Left + notchSpacing + notchWidth;
notch7Left = notch6Left + rightHandNotchGap + notchWidth;
notch8Left = notch7Left + notchSpacing + notchWidth;
notch9Left = notch8Left + notchSpacing + notchWidth;
notch10Left = notch9Left + notchSpacing + notchWidth;
notch11Left = notch10Left + notchSpacing + notchWidth;
notch12Left = notch11Left + notchSpacing + notchWidth;

notch13Left = notch12Left + notchSpacing + notchWidth + edge + notchSpacing;
notch14Left = notch13Left + notchSpacing + notchWidth;
notch15Left = notch14Left + notchSpacing + notchWidth;
notch16Left = notch15Left + notchSpacing + notchWidth;
notch17Left = notch16Left + notchSpacing + notchWidth;
notch18Left = notch17Left + accessoryNotchGap + notchWidth;
notch19Left = notch18Left + notchSpacing + notchWidth;
notch20Left = notch19Left + notchSpacing + notchWidth;
notch21Left = notch20Left + notchSpacing + notchWidth;
notch22Left = notch21Left + notchSpacing + notchWidth;

module hole(x, y, z, cx, cy, cz)
{
	translate([x, y, z]) cube([cx, cy, cz]);
}

module grooveHole(x, y, cx)
{
	hole(edge + x, y, 0, rightHandBoard - x, groove, edge);
	hole(2 * edge + rightHandBoard, y, 0, cx, groove, edge);
}

module notchHole(x, cx)
{
	hole(x, baseTop, 0, cx, notchHeight, edge);
}

module sideNotch(x)
{
	hole(x, pcbTop - edge, 0, edge, 2 * edge + groove, depth);
}

difference()
{
	cube([width, height, depth]);

	grooveHole(39, faceTop, accessoryBoard - 39);
	grooveHole(39, mountTop, accessoryBoard - 39);
	grooveHole(0, pcbTop, accessoryBoard);

	hole(0, baseTop, edge, width, notchHeight, edge);
	notchHole(0, notch1Left);
	notchHole(notch1Left + notchWidth, notchSpacing);
	notchHole(notch2Left + notchWidth, notchSpacing);
	notchHole(notch3Left + notchWidth, notchSpacing);
	notchHole(notch4Left + notchWidth, notchSpacing);
	notchHole(notch5Left + notchWidth, notchSpacing);
	notchHole(notch6Left + notchWidth, rightHandNotchGap);
	notchHole(notch7Left + notchWidth, notchSpacing);
	notchHole(notch8Left + notchWidth, notchSpacing);
	notchHole(notch9Left + notchWidth, notchSpacing);
	notchHole(notch10Left + notchWidth, notchSpacing);
	notchHole(notch11Left + notchWidth, notchSpacing);
	notchHole(notch12Left + notchWidth, 2 * notchSpacing + edge);
	notchHole(notch13Left + notchWidth, notchSpacing);
	notchHole(notch14Left + notchWidth, notchSpacing);
	notchHole(notch15Left + notchWidth, notchSpacing);
	notchHole(notch16Left + notchWidth, notchSpacing);
	notchHole(notch17Left + notchWidth, accessoryNotchGap);
	notchHole(notch18Left + notchWidth, notchSpacing);
	notchHole(notch19Left + notchWidth, notchSpacing);
	notchHole(notch20Left + notchWidth, notchSpacing);
	notchHole(notch21Left + notchWidth, notchSpacing);
	notchHole(notch22Left + notchWidth, notchSpacing + edge);

	sideNotch(0);
	sideNotch(width - edge);

	hole(0, 0, 0, 39, pcbTop - edge, depth);
	hole(width - 39, 0, 0, 39, pcbTop - edge, depth);

	hole(edge + rightHandBoard, 5, 0, edge, 9, depth);
}
