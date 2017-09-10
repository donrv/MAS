/*
 * AlternativeVisualizer.h
 *
 *  Created on: Jun 17, 2016
 *      Author: hatem
 */

#ifndef AlternativeVisualizer_H_
#define AlternativeVisualizer_H_
#include <iostream>
#include "RoadNetwork.h"
#include "DrawObjBase.h"
#include "DrawingHelpers.h"
#include <ros/ros.h>
#include <geometry_msgs/TwistStamped.h>
#include <autoware_msgs/ControlCommandStamped.h>
#include <autoware_msgs/CanInfo.h>
#include "Graph2dBase.h"

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


    void twistCMDCallback(const geometry_msgs::TwistStamped& msg);
    void ctrlCMDCallback(const autoware_msgs::ControlCommandStamped& msg);
    void callbackGetCanInfo(const autoware_msgs::CanInfoConstPtr &msg);

    double m_VehicleTargetStateSpeed;
    double m_VehicleTargetStateSteer;
    double m_VehicleCurrentStateSpeed;
    double m_VehicleCurrentStateSteer;

    ros::Publisher pub_VehicleCommand;
    ros::Subscriber twist_sub;
    ros::Subscriber cmd_sub ;
    ros::Subscriber sub_can_info;
    int m_Velocity;

    Graph2dBase* m_pCurrentVelocityGraph;
    Graph2dBase* m_pTargetVelocityGraph;

private:
	void PrepareVectorMapForDrawing();
	void DrawVectorMap();
	void DrawGPSData();

};

} /* namespace Graphics */

#endif /* AlternativeVisualizer_H_ */
