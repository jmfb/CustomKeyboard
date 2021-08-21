import React from 'react';
import { render } from 'react-dom';
import Application from './Application';

function initializeApplication() {
	const rootElement = document.getElementById('root');
	const application = <Application />;
	render(application, rootElement);
}

initializeApplication();
