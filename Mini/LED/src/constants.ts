export const ledWidth = 20;
export const ledHeight = 20;

export interface ILedPosition {
	x: number;
	y: number;
	rotation?: number;
}


// enum class LedPosition {
// 	PinkyExtraTop = 0,
// 	PinkyTop,
// 	RingTop,
// 	MiddleTop,
// 	IndexTop,
// 	IndexExtraTop,
// 	IndexExtraHome,
// 	IndexHome,
// 	MiddleHome,
// 	RingHome,
// 	PinkyHome,
// 	PinkyExtraBottom,
// 	PinkyBottom,
// 	RingBottom,
// 	MiddleBottom,
// 	IndexBottom,
// 	IndexExtraBottom,
// 	ThumbGridTopFirst,
// 	ThumbGridTopSecond,
// 	ThumbGridBottomSecond,
// 	ThumbGridBottomFirst,
// 	ThumbInner,
// 	ThumbOuter
// };

export const leftHandPositions: ILedPosition[] = [
	{ x: 0, y: 18 },
	{ x: 20, y: 8 },
	{ x: 40, y: 4 },
	{ x: 60, y: 0 },
	{ x: 80, y: 4 },
	{ x: 100, y: 4 },
	{ x: 100, y: 24 },
	{ x: 80, y: 24 },
	{ x: 60, y: 20 },
	{ x: 40, y: 24 },
	{ x: 20, y: 28 },
	{ x: 0, y: 38 },
	{ x: 20, y: 48 },
	{ x: 40, y: 44 },
	{ x: 60, y: 40 },
	{ x: 80, y: 44 },
	{ x: 100, y: 44 },
	{ x: 93, y: 64 },
	{ x: 73, y: 64 },
	{ x: 73, y: 84 },
	{ x: 93, y: 84 },
	{ x: 116, y: 62, rotation: 45 },
	{ x: 130, y: 76, rotation: 45 }
];

export const width = 310;
export const height = 110;

export const rightHandPositions: ILedPosition[] = [
	{ x: width - ledWidth - 0, y: 18 },
	{ x: width - ledWidth - 20, y: 8 },
	{ x: width - ledWidth - 40, y: 4 },
	{ x: width - ledWidth - 60, y: 0 },
	{ x: width - ledWidth - 80, y: 4 },
	{ x: width - ledWidth - 100, y: 4 },
	{ x: width - ledWidth - 100, y: 24 },
	{ x: width - ledWidth - 80, y: 24 },
	{ x: width - ledWidth - 60, y: 20 },
	{ x: width - ledWidth - 40, y: 24 },
	{ x: width - ledWidth - 20, y: 28 },
	{ x: width - ledWidth - 0, y: 38 },
	{ x: width - ledWidth - 20, y: 48 },
	{ x: width - ledWidth - 40, y: 44 },
	{ x: width - ledWidth - 60, y: 40 },
	{ x: width - ledWidth - 80, y: 44 },
	{ x: width - ledWidth - 100, y: 44 },
	{ x: width - ledWidth - 93, y: 64 },
	{ x: width - ledWidth - 73, y: 64 },
	{ x: width - ledWidth - 73, y: 84 },
	{ x: width - ledWidth - 93, y: 84 },
	{ x: width - ledWidth - 116, y: 62, rotation: 45 },
	{ x: width - ledWidth - 130, y: 76, rotation: 45 }
];

export const distanceFromThumb = [
	13,
	13,
	12,
	11,
	9,
	7,
	6,
	7,
	9,
	10,
	11,
	11,
	9,
	8,
	7,
	6,
	4,
	3,
	5,
	3,
	1,
	2,
	0
];

export const ledCount = leftHandPositions.length;
