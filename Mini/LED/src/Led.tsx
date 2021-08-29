import React from 'react';
import { ILedPosition, ledWidth, ledHeight } from './constants';

export interface ILedProps {
	position: ILedPosition;
	color: string;
}

export default function Led({
	position,
	color
}: ILedProps) {
	const { x, y, rotation } = position;
	return (
		<rect
			{...{x, y}}
			width={ledWidth}
			height={ledHeight}
			rx={5}
			fill={color}
			transform={rotation === undefined ?
				undefined :
				`rotate(${rotation},${x + ledWidth / 2},${y + ledHeight / 2})`}
			/>
	);
}
