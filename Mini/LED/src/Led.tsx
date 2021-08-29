import React from 'react';
import { ledWidth, ledHeight } from './constants';

export interface ILedProps {
	x: number;
	y: number;
	rotation?: number;
	color: string;
}

export default function Led({
	x,
	y,
	rotation,
	color
}: ILedProps) {
	return (
		<rect
			{...{x, y}}
			width={ledWidth}
			height={ledHeight}
			rx={5}
			fill={color}
			transform={rotation === undefined ?
				undefined :
				`rotate(${rotation} ${x + ledWidth / 2} ${y + ledHeight / 2})`}
			/>
	);
}
