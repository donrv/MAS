/*
 * AlternativeVisualizer.cpp
 *
 *  Created on: Jun 17, 2016
 *      Author: hatem
 */

#include "AlternativeVisualizer.h"
#include <sstream>
#include <algorithm>


using namespace std;


namespace Graphics
{

AlternativeVisualizer::AlternativeVisualizer()
{
	ros::NodeHandle nh;
	pub_VehicleCommand	= nh.advertise<geometry_msgs::TwistStamped>("twist_cmd", 100);
	m_Velocity = 0;
}

void AlternativeVisualizer::LoadMaterials()
{
}

AlternativeVisualizer::~AlternativeVisualizer()
{

}

bool AlternativeVisualizer::IsInitState()
{
	return false;
}

void AlternativeVisualizer::UpdatePlaneStartGoal(const double& x1,const double& y1, const double& a1, const double& x2,const double& y2, const double& a2)
{
}

void AlternativeVisualizer::AddSimulatedCarPos(const double& x,const double& y, const double& a)
{
}

void AlternativeVisualizer::Reset()
{
}

void AlternativeVisualizer::PrepareVectorMapForDrawing()
{

}

void AlternativeVisualizer::DrawGPSData()
{
}

void AlternativeVisualizer::DrawVectorMap()
{

}

void AlternativeVisualizer::DrawSimu()
{

	glPushMatrix();
	std::ostringstream str_out ;
	str_out <<  m_Velocity;
	DrawingHelpers::DrawString(0, 0, GLUT_BITMAP_TIMES_ROMAN_24, (char*)str_out.str().c_str());
	glPopMatrix();


//		glTranslated(centerX-left_shift-15, 70+85, 0);
//		glColor3f(0.8, 0.1, 0.7);
//		std::ostringstream str_out ;
//		str_out.precision(2);
//		str_out <<  m_VehicleCurrentState.steer*RAD2DEG;
//		DrawingHelpers::DrawString(0, 0, GLUT_BITMAP_TIMES_ROMAN_24, (char*)str_out.str().c_str());


	ros::Rate loop_rate(20);

	if(ros::ok())
	{
		geometry_msgs::Twist t;
		geometry_msgs::TwistStamped twist;
		t.linear.x = m_Velocity;
		t.angular.z = 0;
		twist.twist = t;
		twist.header.stamp = ros::Time::now();

		pub_VehicleCommand.publish(twist);

		ros::spinOnce();
		loop_rate.sleep();
	}
}

void AlternativeVisualizer::DrawInfo(const int& centerX, const int& centerY, const int& maxX, const int& maxY)
{
}

void AlternativeVisualizer::OnLeftClick(const double& x, const double& y)
{}

void AlternativeVisualizer::OnRightClick(const double& x, const double& y)
{}

void AlternativeVisualizer::OnKeyboardPress(const SPECIAL_KEYS_TYPE& sKey, const unsigned char& key)
{
	//std::cout << "key" << std::endl;

	switch(key)
	{
	case 's':
	break;
	case 'v':
	{
	}
	break;
	case 'l':
	{
	}
	break;
	case 'n':
	{
	}
	break;
	case 'g':
	{
	}
	break;
	case '+':
	{
		if(m_Velocity < 10)
			m_Velocity++;
		else
			m_Velocity = 10;
	}
	break;
	case '-':
	{
		if(m_Velocity > 0)
			m_Velocity--;
		else
			m_Velocity = 0;
	}
	break;

	default:
		break;

	}
}

} /* namespace Graphics */
