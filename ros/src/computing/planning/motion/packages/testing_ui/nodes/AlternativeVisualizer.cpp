/*
 * AlternativeVisualizer.cpp
 *
 *  Created on: Jun 17, 2016
 *      Author: hatem
 */

#include "AlternativeVisualizer.h"
#include <sstream>
#include <algorithm>
#include "UtilityH.h"

using namespace std;


namespace Graphics
{

AlternativeVisualizer::AlternativeVisualizer()
{
	ros::NodeHandle nh;
	pub_VehicleCommand	= nh.advertise<geometry_msgs::TwistStamped>("twist_cmd", 100);

	twist_sub = nh.subscribe("/twist_cmd", 1, &AlternativeVisualizer::twistCMDCallback, this);
	cmd_sub = nh.subscribe("/ctrl_cmd", 1, &AlternativeVisualizer::ctrlCMDCallback, this);
	sub_can_info  = nh.subscribe("/can_info",		1,	&AlternativeVisualizer::callbackGetCanInfo, this);

	m_VehicleTargetStateSpeed = 0;
	m_VehicleTargetStateSteer = 0;
	m_VehicleCurrentStateSpeed = 0;
	m_VehicleCurrentStateSteer = 0;

	double axes_color[3] = {0.1, 0.1, 0.8};
	double graph_color[3] = {0.9, 0.2, 0.1};
	m_pCurrentVelocityGraph = new Graph2dBase(20, 200,1000, 20, 0, "Car Velocity", "T s", "V km/h", axes_color, graph_color );
	m_pTargetVelocityGraph = new Graph2dBase(20, 200,1000, 20, 0, "Target Velocity", "T s", "V km/h", axes_color, graph_color );
	m_Velocity = 0;
}

void AlternativeVisualizer::twistCMDCallback(const geometry_msgs::TwistStamped& msg)
{
	cout << "Recieve Twist Data " << endl;
	m_VehicleTargetStateSpeed = msg.twist.linear.x;
	//m_VehicleTargetStateSteer = msg.twist.angular.z;
}

void AlternativeVisualizer::ctrlCMDCallback(const autoware_msgs::ControlCommandStamped& msg)
{

	//m_VehicleTargetStateSpeed = msg.cmd.linear_velocity;
	m_VehicleTargetStateSteer = msg.cmd.steering_angle;

	cout << "Recieve command Data " << endl;
}

void AlternativeVisualizer::callbackGetCanInfo(const autoware_msgs::CanInfoConstPtr &msg)
{
	m_VehicleCurrentStateSpeed = msg->speed/3.6;
	m_VehicleCurrentStateSteer = msg->angle * 0.45 / 600;
	cout << "Recieve Can Data " << endl;
}

void AlternativeVisualizer::LoadMaterials()
{
}

AlternativeVisualizer::~AlternativeVisualizer()
{
	if(m_pCurrentVelocityGraph)
		delete m_pCurrentVelocityGraph;
	if(m_pTargetVelocityGraph)
		delete m_pTargetVelocityGraph;
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

		//pub_VehicleCommand.publish(twist);

		ros::spinOnce();
		loop_rate.sleep();
	}
}

void AlternativeVisualizer::DrawInfo(const int& centerX, const int& centerY, const int& maxX, const int& maxY)
{
	double left_shift = 150;
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glTranslated(centerX-left_shift, 70, 0);
	glRotated(-1*m_VehicleTargetStateSteer*RAD2DEG*16, 0,0,1);
	glTranslated(-(centerX-left_shift), -70, 0);

	float wheel_color[3] = {0.6, 0.7, 0.8};
	DrawingHelpers::DrawWideEllipse(centerX-left_shift, 70, 0.5, 60, 55, 54, wheel_color);

	glColor3f(0.5,0.4, 0.3);
	PlannerHNS::GPSPoint p1(centerX-left_shift, 70, 0.52, 0), p2(centerX-left_shift+38, 70-38, 0.52, 0);
	DrawingHelpers::DrawLinePoygonline(p1, p2, 5);

	PlannerHNS::GPSPoint p11(centerX-left_shift, 70, 0.52, 0), p22(centerX-left_shift-38, 70-38, 0.52, 0);
	DrawingHelpers::DrawLinePoygonline(p11, p22, 5);

	PlannerHNS::GPSPoint p111(centerX-left_shift, 70, 0.52, 0), p222(centerX-left_shift, 70+52, 0.52, 0);
	DrawingHelpers::DrawLinePoygonline(p111, p222, 5);
	glPopMatrix();

	double speed = m_VehicleTargetStateSpeed*3.6;
	float pedal_color[3] = {0.5,0.4, 0.3};
	glColor3f(wheel_color[0],wheel_color[1],wheel_color[2]);
	DrawingHelpers::DrawPedal(centerX + 100 - left_shift, 70, 0, 30.0, 100.0, speed*5.5,pedal_color );

	glPushMatrix();
	glTranslated(centerX-left_shift-15, 70+85, 0);
	glColor3f(0.8, 0.1, 0.7);
	std::ostringstream str_out ;
	str_out.precision(2);
	str_out <<  m_VehicleTargetStateSteer*RAD2DEG;
	DrawingHelpers::DrawString(0, 0, GLUT_BITMAP_TIMES_ROMAN_24, (char*)str_out.str().c_str());
	glPopMatrix();

	glPushMatrix();
	glTranslated(centerX+90 - left_shift, 70+85, 0);
	glColor3f(0.8, 0.1, 0.7);
	std::ostringstream v_out ;
	v_out.precision(2);
	v_out <<  speed;
	DrawingHelpers::DrawString(0, 0, GLUT_BITMAP_TIMES_ROMAN_24, (char*)v_out.str().c_str());
	glPopMatrix();

	glPushMatrix();
	double verticalTranslation = 200;
	glTranslated(10, verticalTranslation, 0);
	double axes_color[3] = {0.1, 0.1, 0.8};
	double graph_color[3] = {0.9, 0.2, 0.1};
	m_pTargetVelocityGraph->ReInitGraphResolution(maxX-20, 200,1000, axes_color, graph_color );
	struct timespec tStamp;
	UtilityHNS::UtilityH::GetTickCount(tStamp);
	m_pTargetVelocityGraph->InsertPointTimeStamp(tStamp, m_VehicleTargetStateSpeed*3.6);
	m_pTargetVelocityGraph->DrawGraph();
	glPopMatrix();


	left_shift = -150;
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glTranslated(centerX-left_shift, 70, 0);
	glRotated(-1*m_VehicleCurrentStateSteer*RAD2DEG*16, 0,0,1);
	glTranslated(-(centerX-left_shift), -70, 0);

	//wheel_color[3] = {0.6, 0.7, 0.8};
	DrawingHelpers::DrawWideEllipse(centerX-left_shift, 70, 0.5, 60, 55, 54, wheel_color);

	glColor3f(0.5,0.4, 0.3);
	p1 = PlannerHNS::GPSPoint(centerX-left_shift, 70, 0.52, 0);
	p2 = PlannerHNS::GPSPoint(centerX-left_shift+38, 70-38, 0.52, 0);
	DrawingHelpers::DrawLinePoygonline(p1, p2, 5);

	p11 = PlannerHNS::GPSPoint(centerX-left_shift, 70, 0.52, 0);
	p22 = PlannerHNS::GPSPoint(centerX-left_shift-38, 70-38, 0.52, 0);
	DrawingHelpers::DrawLinePoygonline(p11, p22, 5);

	p111 = PlannerHNS::GPSPoint(centerX-left_shift, 70, 0.52, 0);
	p222 = PlannerHNS::GPSPoint(centerX-left_shift, 70+52, 0.52, 0);
	DrawingHelpers::DrawLinePoygonline(p111, p222, 5);
	glPopMatrix();

	speed = m_VehicleCurrentStateSpeed*3.6;
	//float pedal_color[3] = {0.5,0.4, 0.3};
	glColor3f(wheel_color[0],wheel_color[1],wheel_color[2]);
	DrawingHelpers::DrawPedal(centerX + 100 - left_shift, 70, 0, 30.0, 100.0, speed*5.5,pedal_color );

	glPushMatrix();
	glTranslated(centerX-left_shift-15, 70+85, 0);
	glColor3f(0.8, 0.1, 0.7);
	std::ostringstream str_out_curr ;
	str_out_curr.precision(2);
	str_out_curr <<  m_VehicleCurrentStateSteer*RAD2DEG;
	DrawingHelpers::DrawString(0, 0, GLUT_BITMAP_TIMES_ROMAN_24, (char*)str_out_curr.str().c_str());
	glPopMatrix();

	glPushMatrix();
	glTranslated(centerX+90 - left_shift, 70+85, 0);
	glColor3f(0.8, 0.1, 0.7);
	std::ostringstream v_out_curr ;
	v_out_curr.precision(2);
	v_out_curr <<  speed;
	DrawingHelpers::DrawString(0, 0, GLUT_BITMAP_TIMES_ROMAN_24, (char*)v_out_curr.str().c_str());
	glPopMatrix();

	glPushMatrix();
	verticalTranslation+=250;
	glTranslated(10, verticalTranslation, 0);
	//double axes_color[3] = {0.1, 0.1, 0.8};
	//double graph_color[3] = {0.9, 0.2, 0.1};
	m_pCurrentVelocityGraph->ReInitGraphResolution(maxX-20, 200,1000, axes_color, graph_color );
	m_pCurrentVelocityGraph->InsertPointTimeStamp(tStamp, m_VehicleCurrentStateSpeed*3.6);
	m_pCurrentVelocityGraph->DrawGraph();
	glPopMatrix();
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
