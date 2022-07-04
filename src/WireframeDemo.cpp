/*
 * WireframeDemo.cpp
 *
 *  Created on: Jun 9, 2022
 *      Author: ZornCo
 */

#include "WireframeDemo.h"

namespace ZTD {
/***********************************************************************************************************************************/
	void WireframeDemo::setupDemo() {

		setupObj();

		fact = 180 / 3.14159259; // conversion from degrees to radians.

		Xoff = 240; // Position the centre of the 3d conversion space into the centre of the TFT screen.
		Yoff = 160;
		Zoff = 550; // Z offset in 3D space (smaller = closer and bigger rendering)
	}

	/***********************************************************************************************************************************/
	void WireframeDemo::loopDemo() {

		// Rotate around x and y axes in 1 degree increments

		Yan++;

		Yan = Yan % 360;

		//Xan = Xan % 360; // prevents overflow.
		Xan += xinc;
		if (Xan > 30)
			xinc = -3;     // Switch to zoom in
		else if (Xan < -30)
			xinc = 3; // Switch to zoom out

		setVars(); //sets up the global vars to do the 3D conversion.

		// Zoom in and out on Z axis within limits
		// the cube intersects with the screen for values < 160
		Zoff += inc;
		if (Zoff > 500)
			inc = -1;     // Switch to zoom in
		else if (Zoff < 160)
			inc = 1; // Switch to zoom out

		for (int i = 0; i < LinestoRender; i++) {
			ORender[i] = Render[i]; // stores the old line segment so we can delete it later.
			processLine(&Render[i], Lines[i]); // converts the 3d line segments to 2d.
		}
		renderImage(); // go draw it!

		delay(14); // Delay to reduce loop rate (reduces flicker caused by aliasing with TFT screen refresh rate)
	}

	/***********************************************************************************************************************************/
	void WireframeDemo::renderImage() {
		// renders all the lines after erasing the old ones.
		// in here is the only code actually interfacing with the OLED. so if you use a different lib, this is where to change it.

		for (int i = 0; i < LinestoRender; i++) {
			Screen::getTFT()->drawLine(ORender[i].p0.x, ORender[i].p0.y, ORender[i].p1.x, ORender[i].p1.y, TFT_BLACK); // erase the old lines.
			//uint16_t color = TFT_BLUE;
			//if (i < 4) color = TFT_RED;
			//if (i > 7) color = TFT_GREEN;
			//if (i > 11) color = TFT_WHITE;
			Screen::getTFT()->drawLine(Render[i].p0.x, Render[i].p0.y, Render[i].p1.x, Render[i].p1.y, Render[i].c);
		}
		OldLinestoRender = LinestoRender;
	}

	/***********************************************************************************************************************************/
// Sets the global vars for the 3d transform. Any points sent through "process" will be transformed using these figures.
// only needs to be called if Xan or Yan are changed.
	void WireframeDemo::setVars() {
		float Xan2, Yan2, Zan2;
		float s1, s2, s3, c1, c2, c3;

		Xan2 = Xan / fact; // convert degrees to radians.
		Yan2 = Yan / fact;

		// Zan is assumed to be zero

		s1 = sin(Yan2);
		s2 = sin(Xan2);

		c1 = cos(Yan2);
		c2 = cos(Xan2);

		xx = c1;
		xy = 0;
		xz = -s1;

		yx = (s1 * s2);
		yy = c2;
		yz = (c1 * s2);

		zx = (s1 * c2);
		zy = -s2;
		zz = (c1 * c2);
	}

	/***********************************************************************************************************************************/
// processes x1,y1,z1 and returns rx1,ry1 transformed by the variables set in SetVars()
// fairly heavy on floating point here.
// uses a bunch of global vars. Could be rewritten with a struct but not worth the effort.
	void WireframeDemo::processLine(struct Line2d *ret, struct Line3d vec) {
		float zvt1;
		int xv1, yv1, zv1;

		float zvt2;
		int xv2, yv2, zv2;

		int rx1, ry1;
		int rx2, ry2;

		int x1;
		int y1;
		int z1;

		int x2;
		int y2;
		int z2;

		int Ok;

		x1 = vec.p0.x;
		y1 = vec.p0.y;
		z1 = vec.p0.z;

		x2 = vec.p1.x;
		y2 = vec.p1.y;
		z2 = vec.p1.z;

		Ok = 0; // defaults to not OK

		xv1 = (x1 * xx) + (y1 * xy) + (z1 * xz);
		yv1 = (x1 * yx) + (y1 * yy) + (z1 * yz);
		zv1 = (x1 * zx) + (y1 * zy) + (z1 * zz);

		zvt1 = zv1 - Zoff;

		if (zvt1 < -5) {
			rx1 = 256 * (xv1 / zvt1) + Xoff;
			ry1 = 256 * (yv1 / zvt1) + Yoff;
			Ok = 1; // ok we are alright for point 1.
		}

		xv2 = (x2 * xx) + (y2 * xy) + (z2 * xz);
		yv2 = (x2 * yx) + (y2 * yy) + (z2 * yz);
		zv2 = (x2 * zx) + (y2 * zy) + (z2 * zz);

		zvt2 = zv2 - Zoff;

		if (zvt2 < -5) {
			rx2 = 256 * (xv2 / zvt2) + Xoff;
			ry2 = 256 * (yv2 / zvt2) + Yoff;
		} else {
			Ok = 0;
		}

		if (Ok == 1) {

			ret->p0.x = rx1;
			ret->p0.y = ry1;

			ret->p1.x = rx2;
			ret->p1.y = ry2;
		}
		// The ifs here are checks for out of bounds. needs a bit more code here to "safe" lines that will be way out of whack, so they don't get drawn and cause screen garbage.
		ret->c = vec.c;
	}

	/***********************************************************************************************************************************/
// line segments to draw a cube. basically p0 to p1. p1 to p2. p2 to p3 so on.
	void WireframeDemo::setupObj() {
		Point3d u = { .x = 0, .y = 50, .z = 0 };
		Point3d d = { .x = 0, .y = -50, .z = 0 };
		Point3d l = { .x = -50, .y = 0, .z = 0 };
		Point3d r = { .x = 50, .y = 0, .z = 0 };
		Point3d f = { .x = 0, .y = 0, .z = 75 };
		Point3d b = { .x = 0, .y = 0, .z = -25 };
		Point3d el = { .x = -40, .y = 75, .z = 0 };
		Point3d er = { .x = 40, .y = 75, .z = 0 };
		// Front Face.

		Lines[0].p0 = f;
		Lines[0].p1 = l;
		Lines[0].c = ZORN_GREEN;

		Lines[1].p0 = f;
		Lines[1].p1 = r;
		Lines[1].c = ZORN_GREEN;

		Lines[2].p0 = f;
		Lines[2].p1 = u;
		Lines[2].c = ZORN_GREEN;

		Lines[3].p0 = f;
		Lines[3].p1 = d;
		Lines[3].c = ZORN_WHITE;

		//back face.

		Lines[4].p0 = b;
		Lines[4].p1 = l;
		Lines[4].c = ZORN_GREEN;

		Lines[5].p0 = b;
		Lines[5].p1 = r;
		Lines[5].c = ZORN_GREEN;

		Lines[6].p0 = b;
		Lines[6].p1 = u;
		Lines[6].c = ZORN_GREEN;

		Lines[7].p0 = b;
		Lines[7].p1 = d;
		Lines[7].c = ZORN_GREEN;

		// now the 4 edge lines.

		Lines[8].p0 = u;
		Lines[8].p1 = r;
		Lines[8].c = ZORN_GREEN;

		Lines[9].p0 = r;
		Lines[9].p1 = d;
		Lines[9].c = ZORN_WHITE;

		Lines[10].p0 = d;
		Lines[10].p1 = l;
		Lines[10].c = ZORN_WHITE;

		Lines[11].p0 = l;
		Lines[11].p1 = u;
		Lines[11].c = ZORN_GREEN;

		// el
		Lines[12].p0 = el;
		Lines[12].p1 = b;
		Lines[12].c = ZORN_GREEN;

		Lines[13].p0 = el;
		Lines[13].p1 = u;
		Lines[13].c = ZORN_GREEN;

		Lines[14].p0 = el;
		Lines[14].p1 = l;
		Lines[14].c = ZORN_GREEN;

		// er
		Lines[15].p0 = er;
		Lines[15].p1 = b;
		Lines[15].c = ZORN_GREEN;

		Lines[16].p0 = er;
		Lines[16].p1 = u;
		Lines[16].c = ZORN_GREEN;

		Lines[17].p0 = er;
		Lines[17].p1 = r;
		Lines[17].c = ZORN_GREEN;

		LinestoRender = 18;
		OldLinestoRender = LinestoRender;

	}
} /* namespace ZTD */
