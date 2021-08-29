import React from 'react';
import Led from './Led';

export default function Application() {
	return (
		<>
			<div>Hello World!</div>
			<svg viewbox='0 0 1000 1000' xmlns='http://www.w3.org/2000/svg'>
				<Led x={0} y={0} color='red' />
				<Led x={20} y={0} color='blue' />
				<Led x={0} y={20} color='green' />
				<Led x={20} y={20} color='black' />
			</svg>
		</>
	);
}
