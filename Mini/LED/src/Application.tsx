import React, { useState, useEffect, useRef } from 'react';
import Keyboard from './Keyboard';
import { ledCount } from './constants';
import { hslToRgb } from './colors';

const allBlack = Array.from({ length: ledCount }).map(index => 'black');
const intervalMs = 20;

export default function Application() {
	const [leftHandColors, setLeftHandColors] = useState(allBlack);
	const [rightHandColors, setRightHandColors] = useState(allBlack);
	const hueRef = useRef(0);

	useEffect(() => {
		const intervalId = window.setInterval(() => {
			hueRef.current = (hueRef.current + 1) % 360;
			const color = hslToRgb(hueRef.current);
			console.log(color);
			setLeftHandColors(Array.from({ length: ledCount }).map(index => color));
			setRightHandColors(Array.from({ length: ledCount }).map(index => color));
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
