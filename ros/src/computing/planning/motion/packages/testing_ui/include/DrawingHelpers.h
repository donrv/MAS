/*
 * DrawingHelpers.h
 *
 *  Created on: May 31, 2016
 *      Author: hatem
 */

#ifndef DRAWINGHELPERS_H_
#define DRAWINGHELPERS_H_

#include "glm.h"


namespace Graphics {


class DrawingHelpers {
public:
	DrawingHelpers();
	virtual ~DrawingHelpers();
	static void DrawString(float x, float y, GLvoid *font_style, char* format, ...);
	static void DrawGrid(const double& x, const double& y, const double& w, const double& h, const double& cell_l);
	static void DrawCustomOrigin(const double& x, const double& y, const double& z,
			const int& yaw, const int& roll, const int& pitch, const double& length);
	static void DrawArrow(const double& x, const double& y, const double& a);

	static GLMmodel* LoadModel(const char* fileName);

	static void DrawModel(GLMmodel* pmod,double length, double width, double height, double x, double y,double z, double heading, double pitch , double roll );

	static void DrawFilledEllipse(float x, float y, float z, float width, float height);

	static void DrawSimpleEllipse(float x, float y, float z, float outer_width, float outer_height);



};

} /* namespace Graphics */


#endif
