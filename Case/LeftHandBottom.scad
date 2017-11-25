board = 160;
edge = 1.5;

depth = 2 * edge;
width = board + 2 * edge;

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

notchCount = 10;
notchGap = board - notchCount * (notchSpacing + notchWidth);
notch1Left = edge + notchSpacing;
notch2Left = notch1Left + notchSpacing + notchWidth;
notch3Left = notch2Left + notchSpacing + notchWidth;
notch4Left = notch3Left + notchSpacing + notchWidth;
notch5Left = notch4Left + notchSpacing + notchWidth;
notch6Left = notch5Left + notchGap + notchWidth;
notch7Left = notch6Left + notchSpacing + notchWidth;
notch8Left = notch7Left + notchSpacing + notchWidth;
notch9Left = notch8Left + notchSpacing + notchWidth;
notch10Left = notch9Left + notchSpacing + notchWidth;

module hole(x, y, z, cx, cy, cz)
{
	translate([x, y, z]) cube([cx, cy, cz]);
}

module grooveHole(y, cx)
{
	hole(edge, y, edge, cx, groove, edge);
}

module notchHole(x, cx)
{
	hole(x, baseTop, edge, cx, notchHeight, edge);
}

module sideNotch(x)
{
	hole(x, pcbTop - edge, 0, edge, 2 * edge + groove, depth);
}

difference()
{
	cube([width, height, depth]);

	grooveHole(faceTop, board);
	grooveHole(mountTop, board);
	grooveHole(pcbTop, board);

	hole(0, baseTop, 0, width, notchHeight, edge);
	notchHole(0, notch1Left);
	notchHole(notch1Left + notchWidth, notchSpacing);
	notchHole(notch2Left + notchWidth, notchSpacing);
	notchHole(notch3Left + notchWidth, notchSpacing);
	notchHole(notch4Left + notchWidth, notchSpacing);
	notchHole(notch5Left + notchWidth, notchGap);
	notchHole(notch6Left + notchWidth, notchSpacing);
	notchHole(notch7Left + notchWidth, notchSpacing);
	notchHole(notch8Left + notchWidth, notchSpacing);
	notchHole(notch9Left + notchWidth, notchSpacing);
	notchHole(notch10Left + notchWidth, notchSpacing + edge);

	sideNotch(0);
	sideNotch(board + edge);
}
