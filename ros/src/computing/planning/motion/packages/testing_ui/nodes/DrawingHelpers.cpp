/*
 * DrawingHelpers.cpp
 *
 *  Created on: May 31, 2016
 *      Author: hatem
 */

#include "DrawingHelpers.h"
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <GL/freeglut.h>

using namespace std;


namespace Graphics
{

DrawingHelpers::DrawingHelpers() {
	// TODO Auto-generated constructor stub

}

DrawingHelpers::~DrawingHelpers() {
	// TODO Auto-generated destructor stub
}

void DrawingHelpers::DrawString(float x, float y, GLvoid* font_style, char* format, ...)
{
	glDisable(GL_LIGHTING);

	va_list args;
	char buffer[1000], *s;

	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);
	//GLuint ox = x;
	GLuint oy = y;

	glRasterPos2f(x, y);
	for (s = buffer; *s; s++)
	{
		if(*s == ',')
		{
			x += 220;
			y = oy;
			glRasterPos2f(x, y);
			continue;
		}
		else if(*s == '\n')
		{
			y+=12;
			glRasterPos2f(x, y);
			continue;
		}

		glutBitmapCharacter(font_style, *s);
	}
	glEnable(GL_LIGHTING);
}

void DrawingHelpers::DrawGrid(const double& x, const double& y, const double& w, const double& h, const double& cell_l)
{
	glPushMatrix();
	int nVerticalLisne   = floor(w/cell_l);
	int nHorizontalLines = floor(h/cell_l);

	glBegin(GL_LINES);
	glColor3ub(210,210,210);
	double incr = y;
	for(int r=0; r<= nHorizontalLines; r++)
	{
		glNormal3f(1.0, 1.0, 1.0);
		glVertex3f(x, incr, 0);
		glVertex3f(x+w, incr, 0);
		incr+=cell_l;
	}

	double incc = x;
	for(int r=0; r<= nVerticalLisne; r++)
	{
		glNormal3f(1.0, 1.0, 1.0);
		glVertex3f(incc, y,  0);
		glVertex3f(incc, y + h, 0);
		incc+=cell_l;
	}
	glEnd();

	glPopMatrix();
}

void DrawingHelpers::DrawArrow(const double& x, const double& y, const double& a)
{
	const int nSlicesStacks = 50;
	const double percent = 20.0;
	const double innerPercent = 15.0;
	double half_length = 10/2.0;

	glPushMatrix();
	//Draw one cylender and cone
	glTranslated(x, y, 0.5);
	glRotated(a*RAD2DEG, 0,0,1);

	//X Axis
	glPushMatrix();
	glColor3ub(200,200,200);
	glRotated(90, 0,1,0);
	glutSolidCylinder(half_length/percent, half_length,nSlicesStacks,nSlicesStacks);
	glTranslated(0,0,half_length);
	glColor3f(1,1,0);
	glutSolidCone(half_length/innerPercent, half_length/innerPercent,nSlicesStacks,nSlicesStacks);
	glPopMatrix();

	glPopMatrix();
}

void DrawingHelpers::DrawCustomOrigin(const double& x, const double& y, const double& z, const int& yaw, const int& roll, const int& pitch, const double& length)
{
	const int nSlicesStacks = 50;
	const double percent = 20.0;
	const double innerPercent = 15.0;
	double half_length = length/2.0;

	glPushMatrix();
	//Draw one cylender and cone
	glTranslated(x, y, z);
	glRotated(yaw, 0,0,1);
	glRotated(roll, 1,0,0);
	glRotated(pitch, 0,1,0);

	//Z Axis
	glPushMatrix();
	glColor3f(0.65,0.65,0.65);
	glutSolidCylinder(half_length/percent, half_length,nSlicesStacks,nSlicesStacks);
	glTranslated(0,0,half_length);
	glColor3f(0,0,1);
	glutSolidCone(half_length/innerPercent, half_length/innerPercent,nSlicesStacks,nSlicesStacks);
	glPopMatrix();

	//X Axis
	glPushMatrix();
	glColor3f(0.65,0.65,0.65);
	glRotated(90, 0,1,0);
	glutSolidCylinder(half_length/percent, half_length,nSlicesStacks,nSlicesStacks);
	glTranslated(0,0,half_length);
	glColor3f(1,1,0);
	glutSolidCone(half_length/innerPercent, half_length/innerPercent,nSlicesStacks,nSlicesStacks);
	glPopMatrix();

//	//Y Axis
	glPushMatrix();
	glColor3f(0.65,0.65,0.65);
	glRotated(90, 1,0,0);
	glutSolidCylinder(half_length/percent, half_length, nSlicesStacks, nSlicesStacks);
	glTranslated(0,0,half_length);
	glColor3f(1,0,0);
	glutSolidCone(half_length/innerPercent, half_length/innerPercent, nSlicesStacks,nSlicesStacks);
	glPopMatrix();

	//glDisable(GL_LIGHTING);
	glPopMatrix();

}

GLMmodel* DrawingHelpers::LoadModel(const char* fileName)
{
	GLMmodel* pmodel = glmReadOBJ((char*)fileName);
	if (!pmodel) exit(0);
	glmUnitize(pmodel);
	glmFacetNormals(pmodel);
	glmVertexNormals(pmodel, 90.0);

	return pmodel;
}

void DrawingHelpers::DrawModel(GLMmodel* pmod,double length, double width, double height, double x, double y,double z, double heading, double pitch , double roll )
{
	if (pmod)
	{
		if(!glIsEnabled(GL_LIGHTING))
			  glEnable(GL_LIGHTING);

		glPushMatrix();
		glTranslated(x,y,z);
		glRotated(heading*RAD2DEG,0.0, 0.0, 1.0);
		glRotated(pitch*RAD2DEG,0.0, 1.0, 0.0);
		glRotated(roll*RAD2DEG,1.0, 0.0, 0.0);

		glScaled(length, width, height);
		glmDraw(pmod, GLM_FLAT | GLM_MATERIAL );
		glPopMatrix();

		glDisable(GL_LIGHTING);
	}
}

void DrawingHelpers::DrawFilledEllipse(float x, float y, float z, float width, float height)
{
	glDisable(GL_LIGHTING);
	glBegin(GL_TRIANGLE_FAN);
		//All triangles fan out starting with this point
		glVertex3f (x,y,z);
		for (float i = 0; i <=M_PI*2*RAD2DEG; i+=0.1)
		{
			glVertex3f(x + width*cos(i), y+height*sin(i), z);
		}
	glEnd();
	glEnable(GL_LIGHTING);
}


void DrawingHelpers::DrawSimpleEllipse(float x, float y, float z, float outer_width, float outer_height)
{
	glBegin(GL_LINE_STRIP);
	for (float jj = 0; jj <=M_PI*2.0; jj+=0.1)
	{
		glVertex3f(x + outer_width*cos(jj), y+ outer_height*sin(jj),z);
	}
	glEnd();
}
} /* namespace Graphics */
