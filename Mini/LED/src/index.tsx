import React from 'react';
import { render } from 'react-dom';
import Application from './Application';

function initializeApplication() {
	const rootElement = document.getElementById('root');
	render(<Application />, rootElement);
}

initializeApplication();
