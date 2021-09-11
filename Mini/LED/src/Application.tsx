import React, { useState, useEffect, useRef } from 'react';
import Keyboard from './Keyboard';
import { ledCount, distanceFromThumb } from './constants';
import { hslToRgb } from './colors';

const allBlack = Array.from({ length: ledCount }).map(index => 'black');
const intervalMs = 40;

export default function Application() {
	const [leftHandColors, setLeftHandColors] = useState(allBlack);
	const [rightHandColors, setRightHandColors] = useState(allBlack);
	const hueRef = useRef(0);

	useEffect(() => {
		const intervalId = window.setInterval(() => {
			hueRef.current = (hueRef.current + 1) % 360;
			const colors = Array.from({ length: ledCount }).map((_, index) => hslToRgb((hueRef.current + 2 * distanceFromThumb[index]) % 360));
			setLeftHandColors(colors);
			setRightHandColors(colors);
		}, intervalMs);
		return () => window.clearInterval(intervalId);
	}, []);

	return (
		<Keyboard
			scaleFactor={3}
			{...{leftHandColors, rightHandColors}}
			/>
	);
}
