/*
 * AlternativeVisualizer.h
 *
 *  Created on: Jun 17, 2016
 *      Author: hatem
 */

#ifndef AlternativeVisualizer_H_
#define AlternativeVisualizer_H_
#include <iostream>
#include "DrawObjBase.h"
#include "DrawingHelpers.h"
#include <ros/ros.h>
#include <geometry_msgs/TwistStamped.h>

namespace Graphics
{

class AlternativeVisualizer : public DrawObjBase
{
public:
	AlternativeVisualizer();
	virtual ~AlternativeVisualizer();

	void DrawSimu();
	void DrawInfo(const int& centerX, const int& centerY, const int& maxX, const int& maxY);
	void OnLeftClick(const double& x, const double& y);
	void OnRightClick(const double& x, const double& y);
	void OnKeyboardPress(const SPECIAL_KEYS_TYPE& sKey, const unsigned char& key);
	void LoadMaterials();
	void Reset();
    bool IsInitState();
    void UpdatePlaneStartGoal(const double& x1,const double& y1, const double& a1, const double& x2,const double& y2, const double& a2);
    void AddSimulatedCarPos(const double& x,const double& y, const double& a);


    ros::Publisher pub_VehicleCommand;
    int m_Velocity;

private:
	void PrepareVectorMapForDrawing();
	void DrawVectorMap();
	void DrawGPSData();

};

} /* namespace Graphics */

#endif /* AlternativeVisualizer_H_ */
