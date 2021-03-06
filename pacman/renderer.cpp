internal void 
clearScreen(u32 color) {
	u32* pixel = (u32*)render_state.memory;
	for (int y = 0; y < render_state.height; y++) {
		for (int x = 0; x < render_state.width; x++) {
			*pixel++ = color;
		}
	}
}

internal void 
drawRectInPixels(int x0, int y0, int x1, int y1, u32 color) {

	x0 = clamp(0, x0, render_state.width);
	x1 = clamp(0, x1, render_state.width);
	y0 = clamp(0, y0, render_state.height);
	y1 = clamp(0, y1, render_state.height);

	for (int y = y0; y < y1; y++) {
		u32* pixel = (u32*)render_state.memory + x0 + y*render_state.width;
		for (int x = x0; x < x1; x++) {
			*pixel++ = color;
		}
	}
}

global_variable float render_scale = 0.01f;

internal void
drawRect(float x, float y, float half_size_x, float half_size_y, u32 color) {	

	x *= render_state.height * render_scale;
	y *= render_state.height * render_scale;
	half_size_x *= render_state.height * render_scale;
	half_size_y *= render_state.height * render_scale;

	x += render_state.width / 2.f;
	y += render_state.height / 2.f;

	//change to pixels 
	int x0 = x - half_size_x;
	int x1 = x + half_size_x;
	int y0 = y - half_size_y;
	int y1 = y + half_size_y;

	drawRectInPixels(x0, y0, x1, y1, color);
}

internal void
drawCircleInPixels(int x0, int y0, int radius, u32 color) {
	for (int y = y0 - radius; y < y0 + radius; y++) {
		u32* pixel = (u32*)render_state.memory + x0 - radius + y * render_state.width;
		for (int x = x0 - radius; x < x0 + radius; x++) {
			if (sqrt(pow(x0 - x, 2) + pow(y0 - y, 2)) <= radius) {
				*pixel = color;
			}
			*pixel++;
		}
	}
}

internal void
drawCircle(float x, float y, float radius, u32 color) {
	x *= render_state.height * render_scale;
	y *= render_state.height * render_scale;
	radius *= render_state.height * render_scale;

	x += render_state.width / 2.f;
	y += render_state.height / 2.f;

	drawCircleInPixels(x, y, radius, color);
}

internal void
drawMouthInPixels(int x0, int y0, int radius, int direction, u32 color) {

	switch (direction) {
	case 0: {
		for (int y = y0 - radius; y < y0 + radius; y++) {
			u32* pixel = (u32*)render_state.memory + x0 - radius + y * render_state.width;
			for (int x = x0 - radius; x < x0 + radius; x++) {
				int xt = x - x0;
				int yt = y - y0;
				if (yt > (3*xt) && yt > (-3*xt)) {
					*pixel = color;
				}
				*pixel++;
			}
		}
	} break;

	case 1: {
		for (int y = y0 - radius; y < y0 + radius; y++) {
			u32* pixel = (u32*)render_state.memory + x0 - radius + y * render_state.width;
			for (int x = x0 - radius; x < x0 + radius; x++) {
				int xt = x - x0;
				int yt = y - y0;
				if (yt < (xt/3) && yt > (-xt/3)) {
					*pixel = color;
				}
				*pixel++;
			}
		}
	} break;

	case 2: {
		for (int y = y0 - radius; y < y0 + radius; y++) {
			u32* pixel = (u32*)render_state.memory + x0 - radius + y * render_state.width;
			for (int x = x0 - radius; x < x0 + radius; x++) {
				int xt = x - x0;
				int yt = y - y0;
				if (yt < (3*xt) && yt < (-3*xt)) {
					*pixel = color;
				}
				*pixel++;
			}
		}
	} break;

	case 3: {
		for (int y = y0 - radius; y < y0 + radius; y++) {
			u32* pixel = (u32*)render_state.memory + x0 - radius + y * render_state.width;
			for (int x = x0 - radius; x < x0 + radius; x++) {
				int xt = x - x0;
				int yt = y - y0;
				if (yt < (-xt/3) && yt > (xt/3)) {
					*pixel = color;
				}
				*pixel++;
			}
		}
	} break;
	}
}

internal void
drawMouth(float x, float y, float radius, int direction) {
	x *= render_state.height * render_scale;
	y *= render_state.height * render_scale;
	radius *= render_state.height * render_scale;

	x += render_state.width / 2.f;
	y += render_state.height / 2.f;

	drawMouthInPixels(x, y, radius, direction, 0x000000);
}

internal void
drawNumber(int number, float x, float y, float size, u32 color) {
	float half_size = size * .5f;

	bool drew_number = false;
	while (number || !drew_number) {
		drew_number = true;

		int digit = number % 10;
		number = number / 10;

		switch (digit) {
		case 0: {
			drawRect(x - size, y, half_size, 2.5f * size, color);
			drawRect(x + size, y, half_size, 2.5f * size, color);
			drawRect(x, y + size * 2.f, half_size, half_size, color);
			drawRect(x, y - size * 2.f, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 1: {
			drawRect(x + size, y, half_size, 2.5f * size, color);
			x -= size * 2.f;
		} break;

		case 2: {
			drawRect(x, y + size * 2.f, 1.5f * size, half_size, color);
			drawRect(x, y, 1.5f * size, half_size, color);
			drawRect(x, y - size * 2.f, 1.5f * size, half_size, color);
			drawRect(x + size, y + size, half_size, half_size, color);
			drawRect(x - size, y - size, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 3: {
			drawRect(x - half_size, y + size * 2.f, size, half_size, color);
			drawRect(x - half_size, y, size, half_size, color);
			drawRect(x - half_size, y - size * 2.f, size, half_size, color);
			drawRect(x + size, y, half_size, 2.5f * size, color);
			x -= size * 4.f;
		} break;

		case 4: {
			drawRect(x + size, y, half_size, 2.5f * size, color);
			drawRect(x - size, y + size, half_size, 1.5f * size, color);
			drawRect(x, y, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 5: {
			drawRect(x, y + size * 2.f, 1.5f * size, half_size, color);
			drawRect(x, y, 1.5f * size, half_size, color);
			drawRect(x, y - size * 2.f, 1.5f * size, half_size, color);
			drawRect(x - size, y + size, half_size, half_size, color);
			drawRect(x + size, y - size, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 6: {
			drawRect(x + half_size, y + size * 2.f, size, half_size, color);
			drawRect(x + half_size, y, size, half_size, color);
			drawRect(x + half_size, y - size * 2.f, size, half_size, color);
			drawRect(x - size, y, half_size, 2.5f * size, color);
			drawRect(x + size, y - size, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 7: {
			drawRect(x + size, y, half_size, 2.5f * size, color);
			drawRect(x - half_size, y + size * 2.f, size, half_size, color);
			x -= size * 4.f;
		} break;

		case 8: {
			drawRect(x - size, y, half_size, 2.5f * size, color);
			drawRect(x + size, y, half_size, 2.5f * size, color);
			drawRect(x, y + size * 2.f, half_size, half_size, color);
			drawRect(x, y - size * 2.f, half_size, half_size, color);
			drawRect(x, y, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 9: {
			drawRect(x - half_size, y + size * 2.f, size, half_size, color);
			drawRect(x - half_size, y, size, half_size, color);
			drawRect(x - half_size, y - size * 2.f, size, half_size, color);
			drawRect(x + size, y, half_size, 2.5f * size, color);
			drawRect(x - size, y + size, half_size, half_size, color);
			x -= size * 4.f;
		} break;
		}

	}
}