function hueToRgb(hue: number) {
	if (hue < 60) {
		return Math.floor(6 * hue * 255 / 360);
	} else if (hue < 180) {
		return 255;
	} else if (hue < 240) {
		return Math.floor((240 - hue) * 6 * 255 / 360);
	} else {
		return 0;
	}
}

function toHex(component: number) {
	const value = `0${component.toString(16)}`;
	return value.substr(value.length - 2);
}

export function hslToRgb(hue: number) {
	const red = hueToRgb((hue + 120) % 360);
	const blue = hueToRgb(hue);
	const green = hueToRgb((hue + 240) % 360);
	return `#${toHex(red)}${toHex(blue)}${toHex(green)}`;
}
