/*
 * WireframeDemo.h
 *
 *  Created on: Jun 9, 2022
 *      Author: ZornCo
 */

#ifndef WIREFRAMEDEMO_H_
#define WIREFRAMEDEMO_H_
#define ZORNHEAD 1
#include "CommonIncludes.h"
#include "Screen.h"
namespace ZTD {

/*

 Example sketch for TFT_eSPI library.

 No fonts are needed.

 Draws a 3d rotating cube on the TFT screen.

 Original code was found at http://forum.freetronics.com/viewtopic.php?f=37&t=5495

 */

#define ZORN_WHITE 0xF79E
#define ZORN_GREEN 0x052B
struct Point3d {
	int x;
	int y;
	int z;
};

struct Point2d {
	int x;
	int y;
};

struct Line3d {
	Point3d p0;
	Point3d p1;
	uint16_t c;
};

struct Line2d {
	Point2d p0;
	Point2d p1;
	uint16_t c;
};
class WireframeDemo {
	int16_t h;
	int16_t w;

	int inc = -2;
	int xinc = 1;

	float xx, xy, xz;
	float yx, yy, yz;
	float zx, zy, zz;

	float fact;

	int Xan, Yan;

	int Xoff;
	int Yoff;
	int Zoff;

	int LinestoRender; // lines to render.
	int OldLinestoRender; // lines to render just in case it changes. this makes sure the old lines all get erased.
	Line3d Lines[20];
	Line2d Render[20];
	Line2d ORender[20];
public:
	void setupObj();
	void processLine(struct Line2d *ret, struct Line3d vec);
	void setVars();
	void renderImage();
	void loopDemo();
	void setupDemo();
};
}
#endif /* WIREFRAMEDEMO_H_ */
