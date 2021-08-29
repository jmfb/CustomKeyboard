import React from 'react';
import Led from './Led';
import { leftHandPositions, rightHandPositions, width, height } from './constants';

export interface IKeyboardProps {
	leftHandColors: string[];
	rightHandColors: string[];
	scaleFactor: number;
}

export default function Keyboard({
	leftHandColors,
	rightHandColors,
	scaleFactor
}: IKeyboardProps) {
	return (
		<svg
			viewBox={`0 0 ${width} ${height}`}
			style={{
				width: `${scaleFactor * width}px`,
				height: `${scaleFactor * height}px`
			}}
			xmlns='http://www.w3.org/2000/svg'>
			{leftHandPositions.map((position, index) =>
				<Led
					key={index}
					{...{position}}
					color={leftHandColors[index]}
					/>
			)}
			{rightHandPositions.map((position, index) =>
				<Led
					key={index}
					{...{position}}
					color={rightHandColors[index]}
					/>
			)}
		</svg>
	);
}
