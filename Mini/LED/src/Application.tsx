import React, { useState, useEffect } from 'react';
import Keyboard from './Keyboard';
import { ledCount } from './constants';

const allBlack = Array.from({ length: ledCount }).map(index => 'black');
const intervalMs = 20;

export default function Application() {
	const [leftHandColors, setLeftHandColors] = useState(allBlack);
	const [rightHandColors, setRightHandColors] = useState(allBlack);

	useEffect(() => {
		const intervalId = window.setInterval(() => {
			setLeftHandColors(Array.from({ length: ledCount }).map(index => '#' + Math.floor(Math.random() * 256 * 256 * 256).toString(16)));
			setRightHandColors(Array.from({ length: ledCount }).map(index => '#' + Math.floor(Math.random() * 256 * 256 * 256).toString(16)));
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
