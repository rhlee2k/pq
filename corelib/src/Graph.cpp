/*
Copyright (c) 2010-2014, Mathieu Labbe - IntRoLab - Universite de Sherbrooke
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Universite de Sherbrooke nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "rtabmap/core/Graph.h"

#include <rtabmap/utilite/ULogger.h>
#include <rtabmap/utilite/UStl.h>
#include <rtabmap/utilite/UMath.h>
#include <rtabmap/utilite/UConversion.h>
#include <rtabmap/utilite/UTimer.h>
#include <rtabmap/utilite/UFile.h>
#include <rtabmap/core/GeodeticCoords.h>
#include <rtabmap/core/Memory.h>
#include <rtabmap/core/util3d_filtering.h>
#include <pcl/search/kdtree.h>
#include <pcl/common/eigen.h>
#include <pcl/common/common.h>
#include <set>
#include <queue>
#include <fstream>

#include <rtabmap/core/OptimizerTORO.h>
#include <rtabmap/core/OptimizerG2O.h>

namespace rtabmap {

namespace graph {

bool exportPoses(
		const std::string & filePath,
		int format, // 0=Raw, 1=RGBD-SLAM, 2=KITTI, 3=TORO, 4=g2o
		const std::map<int, Transform> & poses,
		const std::multimap<int, Link> & constraints, // required for formats 3 and 4
		const std::map<int, double> & stamps, // required for format 1
		bool g2oRobust) // optional for format 4
{
	UDEBUG("%s", filePath.c_str());
	std::string tmpPath = filePath;
	if(format==3) // TORO
	{
		if(UFile::getExtension(tmpPath).empty())
		{
			tmpPath+=".graph";
		}
		return OptimizerTORO::saveGraph(tmpPath, poses, constraints);
	}
	else if(format == 4) // g2o
	{
		if(UFile::getExtension(tmpPath).empty())
		{
			tmpPath+=".g2o";
		}
#ifdef WITH_G2O
		return OptimizerG2O::saveGraph(tmpPath, poses, constraints, g2oRobust);
#else
		UERROR("Cannot export in g2o format because RTAB-Map is not built with g2o support!");
		return false;
#endif
	}
	else
	{
		if(UFile::getExtension(tmpPath).empty())
		{
			tmpPath+=".txt";
		}

		if(format == 1)
		{
			if(stamps.size() != poses.size())
			{
				UERROR("When exporting poses to format 1 (RGBD-SLAM), stamps and poses maps should have the same size!");
				return false;
			}
		}

		FILE* fout = 0;
#ifdef _MSC_VER
		fopen_s(&fout, tmpPath.c_str(), "w");
#else
		fout = fopen(tmpPath.c_str(), "w");
#endif
		if(fout)
		{
			for(std::map<int, Transform>::const_iterator iter=poses.begin(); iter!=poses.end(); ++iter)
			{
				if(format == 1) // rgbd-slam format
				{
					// put the pose in rgbd-slam world reference
					Transform t( 0, 0, 1, 0,
								 0, -1, 0, 0,
								 1, 0, 0, 0);
					Transform pose = t.inverse() * iter->second;
					t = Transform( 0, 0, 1, 0,
								  -1, 0, 0, 0,
								   0,-1, 0, 0);
					pose = t.inverse() * pose * t;

					// Format: stamp x y z qx qy qz qw
					Eigen::Quaternionf q = pose.getQuaternionf();

					UASSERT(uContains(stamps, iter->first));
					fprintf(fout, "%f %f %f %f %f %f %f %f\n",
							stamps.at(iter->first),
							pose.x(),
							pose.y(),
							pose.z(),
							q.x(),
							q.y(),
							q.z(),
							q.w());
				}
				else // default / KITTI format
				{
					Transform pose = iter->second;
					if(format == 2)
					{
						// for KITTI, we need to remove optical rotation
						// z pointing front, x left, y down
						Transform t( 0, 0, 1, 0,
									-1, 0, 0, 0,
									 0,-1, 0, 0);
						pose = t.inverse() * pose * t;
					}

					// Format: r11 r12 r13 tx r21 r22 r23 ty r31 r32 r33 tz
					const float * p = (const float *)pose.data();

					fprintf(fout, "%f", p[0]);
					for(int i=1; i<pose.size(); i++)
					{
						fprintf(fout, " %f", p[i]);
					}
					fprintf(fout, "\n");
				}
			}
			fclose(fout);
			return true;
		}
	}
	return false;
}

bool importPoses(
		const std::string & filePath,
		int format, // 0=Raw, 1=RGBD-SLAM, 2=KITTI, 3=TORO, 4=g2o, 5=NewCollege(t,x,y), 6=Malaga Urban GPS, 7=St Lucia INS, 8=Karlsruhe
		std::map<int, Transform> & poses,
		std::multimap<int, Link> * constraints, // optional for formats 3 and 4
		std::map<int, double> * stamps) // optional for format 1
{
	UDEBUG("%s format=%d", filePath.c_str(), format);
	if(format==3) // TORO
	{
		std::multimap<int, Link> constraintsTmp;
		if(OptimizerTORO::loadGraph(filePath, poses, constraintsTmp))
		{
			if(constraints)
			{
				*constraints = constraintsTmp;
			}
			return true;
		}
		return false;
	}
	else if(format == 4) // g2o
	{
		std::multimap<int, Link> constraintsTmp;
		UERROR("Cannot import from g2o format because it is not yet supported!");
		return false;
	}
	else
	{
		std::ifstream file;
		file.open(filePath.c_str(), std::ifstream::in);
		if(!file.good())
		{
			return false;
		}
		int id=1;
		GeodeticCoords origin;
		Transform originPose;
		while(file.good())
		{
			std::string str;
			std::getline(file, str);

			if(str.empty() || str.at(0) == '#' || str.at(0) == '%')
			{
				continue;
			}

			if(format == 8) // Karlsruhe format
			{
				std::vector<std::string> strList = uListToVector(uSplit(str));
				if(strList.size() ==  10)
				{
					// timestamp,lat,lon,alt,x,y,z,roll,pitch,yaw

					// stamp is 1252400096825289728, transform to 1252400096.825289728
					double stamp = uStr2Double(strList[0].insert(10, 1, '.'));
					double x = uStr2Double(strList[4]);
					double y = uStr2Double(strList[5]);
					double z = uStr2Double(strList[6]);
					double roll = uStr2Double(strList[8]);
					double pitch = uStr2Double(strList[7]);
					double yaw = -(uStr2Double(strList[9])-M_PI_2);
					if(stamps)
					{
						stamps->insert(std::make_pair(id, stamp));
					}
					Transform pose = Transform(x,y,z,roll,pitch,yaw);
					if(poses.size() == 0)
					{
						originPose = pose.inverse();
					}
					pose = originPose * pose; // transform in local coordinate where first value is the origin
					poses.insert(std::make_pair(id, pose));
				}
				else
				{
					UERROR("Error parsing \"%s\" with Karlsruhe format (should have 10 values)", str.c_str());
				}
			}
			else if(format == 7) // St Lucia format
			{
				std::vector<std::string> strList = uListToVector(uSplit(str));
				if(strList.size() ==  12)
				{
					// Data Type
					//0=Timestamp (seconds)
					//1=Timestamp (millisec)
					//2=Latitude (deg)
					//3=Longitude (deg)
					//4=Altitude (m)
					//5=Height AMSL (m)
					//6=vENU X
					//7=vENU Y
					//8=vENU Z
					//9=Roll (rad)
					//10=Pitch (rad)
					//11=Yaw (rad)

					// the millisec str needs 0-padding if size < 6
					std::string millisecStr = strList[1];
					while(millisecStr.size() < 6)
					{
						millisecStr = "0" + millisecStr;
					}
					double stamp = uStr2Double(strList[0] + "." + millisecStr);

					// conversion GPS to local coordinate XYZ
					double longitude = uStr2Double(strList[2]);
					double latitude = uStr2Double(strList[3]);
					double altitude = uStr2Double(strList[4]);
					if(poses.empty())
					{
						origin = GeodeticCoords(longitude, latitude, altitude);
					}
					cv::Point3d coordENU = GeodeticCoords(longitude, latitude, altitude).toENU_WGS84(origin);
					double roll = uStr2Double(strList[10]);
					double pitch = uStr2Double(strList[9]);
					double yaw = -(uStr2Double(strList[11])-M_PI_2);
					if(stamps)
					{
						stamps->insert(std::make_pair(id, stamp));
					}
					poses.insert(std::make_pair(id, Transform(coordENU.x,coordENU.y,coordENU.z, roll,pitch,yaw)));
				}
				else
				{
					UERROR("Error parsing \"%s\" with St Lucia format (should have 12 values, e.g., 101215_153851_Ins0.log)", str.c_str());
				}
			}
			else if(format == 6) // MALAGA URBAN format
			{
				std::vector<std::string> strList = uListToVector(uSplit(str));
				if(strList.size() ==  25)
				{
					// 0=Time
					// Lat Lon Alt fix #sats speed dir
					// 8=Local_X
					// 9=Local_Y
					// 10=Local_Z
					// rawlog_ID Geocen_X Geocen_Y Geocen_Z GPS_X GPS_Y GPS_Z GPS_VX GPS_VY GPS_VZ Local_VX Local_VY Local_VZ SAT_Time
					double stamp = uStr2Double(strList[0]);
					double x = uStr2Double(strList[8]);
					double y = uStr2Double(strList[9]);
					double z = uStr2Double(strList[10]);
					if(stamps)
					{
						stamps->insert(std::make_pair(id, stamp));
					}
					float yaw = 0.0f;
					if(uContains(poses, id-1))
					{
						// set yaw depending on successive poses
						Transform & previousPose = poses.at(id-1);
						yaw = atan2(y-previousPose.y(),x-previousPose.x());
						previousPose = Transform(previousPose.x(), previousPose.y(), yaw);
					}
					poses.insert(std::make_pair(id, Transform(x,y,z,0,0,yaw)));
				}
				else
				{
					UERROR("Error parsing \"%s\" with Malaga Urban format (should have 25 values, *_GPS.txt)", str.c_str());
				}
			}
			else if(format == 5) // NewCollege format
			{
				std::vector<std::string> strList = uListToVector(uSplit(str));
				if(strList.size() ==  3)
				{
					if( uIsNumber(uReplaceChar(strList[0], ' ', "")) && 
						uIsNumber(uReplaceChar(strList[1], ' ', "")) && 
						uIsNumber(uReplaceChar(strList[2], ' ', "")) &&
						(strList.size()==3 || uIsNumber(uReplaceChar(strList[3], ' ', ""))))
					{
						double stamp = uStr2Double(uReplaceChar(strList[0], ' ', ""));
						double x = uStr2Double(uReplaceChar(strList[1], ' ', ""));
						double y = uStr2Double(uReplaceChar(strList[2], ' ', ""));

						if(stamps)
						{
							stamps->insert(std::make_pair(id, stamp));
						}
						float yaw = 0.0f; 
						if(uContains(poses, id-1))
						{
							// set yaw depending on successive poses
							Transform & previousPose = poses.at(id-1);
							yaw = atan2(y-previousPose.y(),x-previousPose.x());
							previousPose = Transform(previousPose.x(), previousPose.y(), yaw);
						}
						Transform pose = Transform(x,y,0,0,0,yaw);
						if(poses.size() == 0)
						{
							originPose = pose.inverse();
						}
						pose = originPose * pose; // transform in local coordinate where first value is the origin
						poses.insert(std::make_pair(id, pose));
					}
					else
					{
						UDEBUG("Not valid values detected:  \"%s\"", str.c_str());
					}
				}
				else
				{
					UERROR("Error parsing \"%s\" with NewCollege format (should have 3 values: stamp x y)", str.c_str());
				}
			}
			else if(format == 1) // rgbd-slam format
			{
				std::list<std::string> strList = uSplit(str);
				if(strList.size() ==  8)
				{
					double stamp = uStr2Double(strList.front());
					strList.pop_front();
					str = uJoin(strList, " ");
					Transform pose = Transform::fromString(str);
					if(pose.isNull())
					{
						UWARN("Null transform read!? line parsed: \"%s\"", str.c_str());
					}
					else
					{
						if(stamps)
						{
							stamps->insert(std::make_pair(id, stamp));
						}
						// we need to remove optical rotation
						// z pointing front, x left, y down
						Transform t( 0, 0, 1, 0,
									-1, 0, 0, 0,
									 0,-1, 0, 0);
						pose = t * pose * t.inverse();
						t = Transform( 0, 0, 1, 0,
									   0, -1, 0, 0,
									   1, 0, 0, 0);
						pose = t*pose;
						poses.insert(std::make_pair(id, pose));
					}
				}
				else
				{
					UERROR("Error parsing \"%s\" with RGBD-SLAM format (should have 8 values: stamp x y z qw qx qy qz)", str.c_str());
				}
			}
			else // default / KITTI format
			{
				Transform pose = Transform::fromString(str);
				if(format == 2)
				{
					// for KITTI, we need to remove optical rotation
					// z pointing front, x left, y down
					Transform t( 0, 0, 1, 0,
								-1, 0, 0, 0,
								 0,-1, 0, 0);
					pose = t * pose * t.inverse();
				}
				poses.insert(std::make_pair(id, pose));
			}
			++id;
		}
		file.close();
		return true;
	}
	return false;
}


////////////////////////////////////////////
// Graph utilities
////////////////////////////////////////////
std::multimap<int, Link>::iterator findLink(
		std::multimap<int, Link> & links,
		int from,
		int to,
		bool checkBothWays)
{
	std::multimap<int, Link>::iterator iter = links.find(from);
	while(iter != links.end() && iter->first == from)
	{
		if(iter->second.to() == to)
		{
			return iter;
		}
		++iter;
	}

	if(checkBothWays)
	{
		// let's try to -> from
		iter = links.find(to);
		while(iter != links.end() && iter->first == to)
		{
			if(iter->second.to() == from)
			{
				return iter;
			}
			++iter;
		}
	}
	return links.end();
}

std::multimap<int, int>::iterator findLink(
		std::multimap<int, int> & links,
		int from,
		int to,
		bool checkBothWays)
{
	std::multimap<int, int>::iterator iter = links.find(from);
	while(iter != links.end() && iter->first == from)
	{
		if(iter->second == to)
		{
			return iter;
		}
		++iter;
	}

	if(checkBothWays)
	{
		// let's try to -> from
		iter = links.find(to);
		while(iter != links.end() && iter->first == to)
		{
			if(iter->second == from)
			{
				return iter;
			}
			++iter;
		}
	}
	return links.end();
}
std::multimap<int, Link>::const_iterator findLink(
		const std::multimap<int, Link> & links,
		int from,
		int to,
		bool checkBothWays)
{
	std::multimap<int, Link>::const_iterator iter = links.find(from);
	while(iter != links.end() && iter->first == from)
	{
		if(iter->second.to() == to)
		{
			return iter;
		}
		++iter;
	}

	if(checkBothWays)
	{
		// let's try to -> from
		iter = links.find(to);
		while(iter != links.end() && iter->first == to)
		{
			if(iter->second.to() == from)
			{
				return iter;
			}
			++iter;
		}
	}
	return links.end();
}

std::multimap<int, int>::const_iterator findLink(
		const std::multimap<int, int> & links,
		int from,
		int to,
		bool checkBothWays)
{
	std::multimap<int, int>::const_iterator iter = links.find(from);
	while(iter != links.end() && iter->first == from)
	{
		if(iter->second == to)
		{
			return iter;
		}
		++iter;
	}

	if(checkBothWays)
	{
		// let's try to -> from
		iter = links.find(to);
		while(iter != links.end() && iter->first == to)
		{
			if(iter->second == from)
			{
				return iter;
			}
			++iter;
		}
	}
	return links.end();
}

std::map<int, Transform> frustumPosesFiltering(
		const std::map<int, Transform> & poses,
		const Transform & cameraPose,
		float horizontalFOV, // in degrees, xfov = atan((image_width/2)/fx)*2
		float verticalFOV,   // in degrees, yfov = atan((image_height/2)/fy)*2
		float nearClipPlaneDistance,
		float farClipPlaneDistance,
		bool negative)
{
	std::map<int, Transform> output;

	if(poses.size())
	{
		pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
		std::vector<int> ids(poses.size());

		cloud->resize(poses.size());
		ids.resize(poses.size());
		int oi=0;
		for(std::map<int, rtabmap::Transform>::const_iterator iter=poses.begin(); iter!=poses.end(); ++iter)
		{
			if(!iter->second.isNull())
			{
				(*cloud)[oi] = pcl::PointXYZ(iter->second.x(), iter->second.y(), iter->second.z());
				ids[oi++] = iter->first;
			}
		}
		cloud->resize(oi);
		ids.resize(oi);

		pcl::IndicesPtr indices = util3d::frustumFiltering(
				cloud,
				pcl::IndicesPtr(new std::vector<int>),
				cameraPose,
				horizontalFOV,
				verticalFOV,
				nearClipPlaneDistance,
				farClipPlaneDistance,
				negative);


		for(unsigned int i=0; i<indices->size(); ++i)
		{
			output.insert(*poses.find(ids[indices->at(i)]));
		}
	}
	return output;
}

std::map<int, Transform> radiusPosesFiltering(
		const std::map<int, Transform> & poses,
		float radius,
		float angle,
		bool keepLatest)
{
	if(poses.size() > 2 && radius > 0.0f)
	{
		pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
		cloud->resize(poses.size());
		int i=0;
		for(std::map<int, Transform>::const_iterator iter = poses.begin(); iter!=poses.end(); ++iter, ++i)
		{
			(*cloud)[i] = pcl::PointXYZ(iter->second.x(), iter->second.y(), iter->second.z());
			UASSERT_MSG(pcl::isFinite((*cloud)[i]), uFormat("Invalid pose (%d) %s", iter->first, iter->second.prettyPrint().c_str()).c_str());
		}

		// radius filtering
		std::vector<int> names = uKeys(poses);
		std::vector<Transform> transforms = uValues(poses);

		pcl::search::KdTree<pcl::PointXYZ>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZ> (false));
		tree->setInputCloud(cloud);
		std::set<int> indicesChecked;
		std::set<int> indicesKept;

		for(unsigned int i=0; i<cloud->size(); ++i)
		{
			if(indicesChecked.find(i) == indicesChecked.end())
			{
				std::vector<int> kIndices;
				std::vector<float> kDistances;
				tree->radiusSearch(cloud->at(i), radius, kIndices, kDistances);

				std::set<int> cloudIndices;
				const Transform & currentT = transforms.at(i);
				Eigen::Vector3f vA = currentT.toEigen3f().rotation()*Eigen::Vector3f(1,0,0);
				for(unsigned int j=0; j<kIndices.size(); ++j)
				{
					if(indicesChecked.find(kIndices[j]) == indicesChecked.end())
					{
						if(angle > 0.0f)
						{
							const Transform & checkT = transforms.at(kIndices[j]);
							// same orientation?
							Eigen::Vector3f vB = checkT.toEigen3f().rotation()*Eigen::Vector3f(1,0,0);
							double a = pcl::getAngle3D(Eigen::Vector4f(vA[0], vA[1], vA[2], 0), Eigen::Vector4f(vB[0], vB[1], vB[2], 0));
							if(a <= angle)
							{
								cloudIndices.insert(kIndices[j]);
							}
						}
						else
						{
							cloudIndices.insert(kIndices[j]);
						}
					}
				}

				if(keepLatest)
				{
					bool lastAdded = false;
					for(std::set<int>::reverse_iterator iter = cloudIndices.rbegin(); iter!=cloudIndices.rend(); ++iter)
					{
						if(!lastAdded)
						{
							indicesKept.insert(*iter);
							lastAdded = true;
						}
						indicesChecked.insert(*iter);
					}
				}
				else
				{
					bool firstAdded = false;
					for(std::set<int>::iterator iter = cloudIndices.begin(); iter!=cloudIndices.end(); ++iter)
					{
						if(!firstAdded)
						{
							indicesKept.insert(*iter);
							firstAdded = true;
						}
						indicesChecked.insert(*iter);
					}
				}
			}
		}

		//pcl::IndicesPtr indicesOut(new std::vector<int>);
		//indicesOut->insert(indicesOut->end(), indicesKept.begin(), indicesKept.end());
		UINFO("Cloud filtered In = %d, Out = %d", cloud->size(), indicesKept.size());
		//pcl::io::savePCDFile("duplicateIn.pcd", *cloud);
		//pcl::io::savePCDFile("duplicateOut.pcd", *cloud, *indicesOut);

		std::map<int, Transform> keptPoses;
		for(std::set<int>::iterator iter = indicesKept.begin(); iter!=indicesKept.end(); ++iter)
		{
			keptPoses.insert(std::make_pair(names.at(*iter), transforms.at(*iter)));
		}

		// make sure the first and last poses are still here
		keptPoses.insert(*poses.begin());
		keptPoses.insert(*poses.rbegin());

		return keptPoses;
	}
	else
	{
		return poses;
	}
}

std::multimap<int, int> radiusPosesClustering(const std::map<int, Transform> & poses, float radius, float angle)
{
	std::multimap<int, int> clusters;
	if(poses.size() > 1 && radius > 0.0f)
	{
		pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
		cloud->resize(poses.size());
		int i=0;
		for(std::map<int, Transform>::const_iterator iter = poses.begin(); iter!=poses.end(); ++iter, ++i)
		{
			(*cloud)[i] = pcl::PointXYZ(iter->second.x(), iter->second.y(), iter->second.z());
			UASSERT_MSG(pcl::isFinite((*cloud)[i]), uFormat("Invalid pose (%d) %s", iter->first, iter->second.prettyPrint().c_str()).c_str());
		}

		// radius clustering (nearest neighbors)
		std::vector<int> ids = uKeys(poses);
		std::vector<Transform> transforms = uValues(poses);

		pcl::search::KdTree<pcl::PointXYZ>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZ> (false));
		tree->setInputCloud(cloud);

		for(unsigned int i=0; i<cloud->size(); ++i)
		{
			std::vector<int> kIndices;
			std::vector<float> kDistances;
			tree->radiusSearch(cloud->at(i), radius, kIndices, kDistances);

			std::set<int> cloudIndices;
			const Transform & currentT = transforms.at(i);
			Eigen::Vector3f vA = currentT.toEigen3f().rotation()*Eigen::Vector3f(1,0,0);
			for(unsigned int j=0; j<kIndices.size(); ++j)
			{
				if((int)i != kIndices[j])
				{
					if(angle > 0.0f)
					{
						const Transform & checkT = transforms.at(kIndices[j]);
						// same orientation?
						Eigen::Vector3f vB = checkT.toEigen3f().rotation()*Eigen::Vector3f(1,0,0);
						double a = pcl::getAngle3D(Eigen::Vector4f(vA[0], vA[1], vA[2], 0), Eigen::Vector4f(vB[0], vB[1], vB[2], 0));
						if(a <= angle)
						{
							clusters.insert(std::make_pair(ids[i], ids[kIndices[j]]));
						}
					}
					else
					{
						clusters.insert(std::make_pair(ids[i], ids[kIndices[j]]));
					}
				}
			}
		}
	}
	return clusters;
}

void reduceGraph(
		const std::map<int, Transform> & poses,
		const std::multimap<int, Link> & links,
		std::multimap<int, int> & hyperNodes, //<parent ID, child ID>
		std::multimap<int, Link> & hyperLinks)
{
	UINFO("Input: poses=%d links=%d", (int)poses.size(), (int)links.size());
	UTimer timer;
	std::map<int, int> posesToHyperNodes;
	std::map<int, std::multimap<int, Link> > clusterloopClosureLinks;

	{
		std::multimap<int, Link> bidirectionalLoopClosureLinks;
		for(std::multimap<int, Link>::const_iterator jter=links.begin(); jter!=links.end(); ++jter)
		{
			if(jter->second.type() != Link::kNeighbor &&
			   jter->second.type() != Link::kNeighborMerged &&
			   jter->second.userDataCompressed().empty())
			{
				if(uContains(poses, jter->second.from()) &&
				   uContains(poses, jter->second.to()))
				{
					UASSERT_MSG(graph::findLink(links, jter->second.to(), jter->second.from(), false) == links.end(), "Input links should be unique!");
					bidirectionalLoopClosureLinks.insert(std::make_pair(jter->second.from(), jter->second));
					//bidirectionalLoopClosureLinks.insert(std::make_pair(jter->second.to(), jter->second.inverse()));
				}
			}
		}

		UINFO("Clustering hyper nodes...");
		// largest ID to smallest ID
		for(std::map<int, Transform>::const_reverse_iterator iter=poses.rbegin(); iter!=poses.rend(); ++iter)
		{
			if(posesToHyperNodes.find(iter->first) == posesToHyperNodes.end())
			{
				int hyperNodeId = iter->first;
				std::list<int> loopClosures;
				std::set<int> loopClosuresAdded;
				loopClosures.push_back(iter->first);
				std::multimap<int, Link> clusterLinks;
				while(loopClosures.size())
				{
					int id = loopClosures.front();
					loopClosures.pop_front();

					UASSERT(posesToHyperNodes.find(id) == posesToHyperNodes.end());

					posesToHyperNodes.insert(std::make_pair(id, hyperNodeId));
					hyperNodes.insert(std::make_pair(hyperNodeId, id));

					for(std::multimap<int, Link>::const_iterator jter=bidirectionalLoopClosureLinks.find(id); jter!=bidirectionalLoopClosureLinks.end() && jter->first==id; ++jter)
					{
						if(posesToHyperNodes.find(jter->second.to()) == posesToHyperNodes.end() &&
						   loopClosuresAdded.find(jter->second.to()) == loopClosuresAdded.end())
						{
							loopClosures.push_back(jter->second.to());
							loopClosuresAdded.insert(jter->second.to());
							clusterLinks.insert(*jter);
							clusterLinks.insert(std::make_pair(jter->second.to(), jter->second.inverse()));
							if(jter->second.from() < jter->second.to())
							{
								UWARN("Child to Parent link? %d->%d (type=%d)",
										jter->second.from(),
										jter->second.to(),
										jter->second.type());
							}
						}
					}
				}
				UASSERT(clusterloopClosureLinks.find(hyperNodeId) == clusterloopClosureLinks.end());
				clusterloopClosureLinks.insert(std::make_pair(hyperNodeId, clusterLinks));
				UDEBUG("Created hyper node %d with %d children (%f%%)",
						hyperNodeId, (int)loopClosuresAdded.size(), float(posesToHyperNodes.size())/float(poses.size())*100.0f);
			}
		}
		UINFO("Clustering hyper nodes... done! (%f s)", timer.ticks());
	}

	UINFO("Creating hyper links...");
	int i=0;
	for(std::multimap<int, Link>::const_reverse_iterator jter=links.rbegin(); jter!=links.rend(); ++jter)
	{
		if((jter->second.type() == Link::kNeighbor ||
		   jter->second.type() == Link::kNeighborMerged ||
		   !jter->second.userDataCompressed().empty()) &&
		   uContains(poses, jter->second.from()) &&
		   uContains(poses, jter->second.to()))
		{
			UASSERT_MSG(uContains(posesToHyperNodes, jter->second.from()), uFormat("%d->%d (type=%d)", jter->second.from(), jter->second.to(), jter->second.type()).c_str());
			UASSERT_MSG(uContains(posesToHyperNodes, jter->second.to()), uFormat("%d->%d (type=%d)", jter->second.from(), jter->second.to(), jter->second.type()).c_str());
			int hyperNodeIDFrom = posesToHyperNodes.at(jter->second.from());
			int hyperNodeIDTo = posesToHyperNodes.at(jter->second.to());

			// ignore links inside a hyper node
			if(hyperNodeIDFrom != hyperNodeIDTo)
			{
				std::multimap<int, Link>::iterator tmpIter = graph::findLink(hyperLinks, hyperNodeIDFrom, hyperNodeIDTo);
				if(tmpIter!=hyperLinks.end() &&
					hyperNodeIDFrom == jter->second.from() &&
					hyperNodeIDTo == jter->second.to() &&
					tmpIter->second.type() > Link::kNeighbor &&
					jter->second.type() == Link::kNeighbor)
				{
					// neighbor links have priority, so remove the previously added link
					hyperLinks.erase(tmpIter);
				}

				// only add unique link between two hyper nodes (keeping only the more recent)
				if(graph::findLink(hyperLinks, hyperNodeIDFrom, hyperNodeIDTo) == hyperLinks.end())
				{
					UASSERT(clusterloopClosureLinks.find(hyperNodeIDFrom) != clusterloopClosureLinks.end());
					UASSERT(clusterloopClosureLinks.find(hyperNodeIDTo) != clusterloopClosureLinks.end());
					std::multimap<int, Link> tmpLinks = clusterloopClosureLinks.at(hyperNodeIDFrom);
					tmpLinks.insert(clusterloopClosureLinks.at(hyperNodeIDTo).begin(), clusterloopClosureLinks.at(hyperNodeIDTo).end());
					tmpLinks.insert(std::make_pair(jter->second.from(), jter->second));

					std::list<int> path = computePath(tmpLinks, hyperNodeIDFrom, hyperNodeIDTo, false, true);
					UASSERT_MSG(path.size()>1,
							uFormat("path.size()=%d, hyperNodeIDFrom=%d, hyperNodeIDTo=%d",
									(int)path.size(),
									hyperNodeIDFrom,
									hyperNodeIDTo).c_str());

					if(path.size() > 10)
					{
						UWARN("Large path! %d nodes", (int)path.size());
						std::stringstream stream;
						for(std::list<int>::const_iterator iter=path.begin(); iter!=path.end();++iter)
						{
							if(iter!=path.begin())
							{
								stream << ",";
							}

							stream << *iter;
						}
						UWARN("Path = [%s]", stream.str().c_str());
					}

					// create the hyperlink
					std::list<int>::iterator iter=path.begin();
					int from = *iter;
					++iter;
					int to = *iter;
					std::multimap<int, Link>::const_iterator foundIter = graph::findLink(tmpLinks, from, to, false);
					UASSERT(foundIter != tmpLinks.end());
					Link hyperLink = foundIter->second;
					++iter;
					from = to;
					for(; iter!=path.end(); ++iter)
					{
						to = *iter;
						std::multimap<int, Link>::const_iterator foundIter = graph::findLink(tmpLinks, from, to, false);
						UASSERT(foundIter != tmpLinks.end());
						hyperLink = hyperLink.merge(foundIter->second, jter->second.type());

						from = to;
					}

					UASSERT(hyperLink.from() == hyperNodeIDFrom);
					UASSERT(hyperLink.to() == hyperNodeIDTo);
					hyperLinks.insert(std::make_pair(hyperNodeIDFrom, hyperLink));

					UDEBUG("Created hyper link %d->%d (%f%%)",
							hyperLink.from(), hyperLink.to(), float(i)/float(links.size())*100.0f);
					if(hyperLink.transform().getNorm() > jter->second.transform().getNorm()+1)
					{
						UWARN("Large hyper link %d->%d (%f m)! original %d->%d (%f m)",
								hyperLink.from(),
								hyperLink.to(),
								hyperLink.transform().getNorm(),
								jter->second.from(),
								jter->second.to(),
								jter->second.transform().getNorm());

					}
				}
			}
		}
		++i;
	}
	UINFO("Creating hyper links... done! (%f s)", timer.ticks());

	UINFO("Output: poses=%d links=%d", (int)uUniqueKeys(hyperNodes).size(), (int)links.size());
}


class Node
{
public:
	Node(int id, int fromId, const rtabmap::Transform & pose) :
		id_(id),
		costSoFar_(0.0f),
		distToEnd_(0.0f),
		fromId_(fromId),
		closed_(false),
		pose_(pose)
	{}

	int id() const {return id_;}
	int fromId() const {return fromId_;}
	bool isClosed() const {return closed_;}
	bool isOpened() const {return !closed_;}
	float costSoFar() const {return costSoFar_;} // Dijkstra cost
	float distToEnd() const {return distToEnd_;} // Breath-first cost
	float totalCost() const {return costSoFar_ + distToEnd_;} // A* cost
	rtabmap::Transform pose() const {return pose_;}
	float distFrom(const rtabmap::Transform & pose) const
	{
		return pose_.getDistance(pose); // use sqrt distance
	}

	void setClosed(bool closed) {closed_ = closed;}
	void setFromId(int fromId) {fromId_ = fromId;}
	void setCostSoFar(float costSoFar) {costSoFar_ = costSoFar;}
	void setDistToEnd(float distToEnd) {distToEnd_ = distToEnd;}
	void setPose(const Transform & pose) {pose_ = pose;}

private:
	int id_;
	float costSoFar_;
	float distToEnd_;
	int fromId_;
	bool closed_;
	rtabmap::Transform pose_;
};

typedef std::pair<int, float> Pair; // first is id, second is cost
struct Order
{
    bool operator()(Pair const& a, Pair const& b) const
    {
        return a.second > b.second;
    }
};

// A*
std::list<std::pair<int, Transform> > computePath(
			const std::map<int, rtabmap::Transform> & poses,
			const std::multimap<int, int> & links,
			int from,
			int to,
			bool updateNewCosts)
{
	std::list<std::pair<int, Transform> > path;

	int startNode = from;
	int endNode = to;
	rtabmap::Transform endPose = poses.at(endNode);
	std::map<int, Node> nodes;
	nodes.insert(std::make_pair(startNode, Node(startNode, 0, poses.at(startNode))));
	std::priority_queue<Pair, std::vector<Pair>, Order> pq;
	std::multimap<float, int> pqmap;
	if(updateNewCosts)
	{
		pqmap.insert(std::make_pair(0, startNode));
	}
	else
	{
		pq.push(Pair(startNode, 0));
	}

	while((updateNewCosts && pqmap.size()) || (!updateNewCosts && pq.size()))
	{
		Node * currentNode;
		if(updateNewCosts)
		{
			currentNode = &nodes.find(pqmap.begin()->second)->second;
			pqmap.erase(pqmap.begin());
		}
		else
		{
			currentNode = &nodes.find(pq.top().first)->second;
			pq.pop();
		}

		currentNode->setClosed(true);

		if(currentNode->id() == endNode)
		{
			while(currentNode->id()!=startNode)
			{
				path.push_front(std::make_pair(currentNode->id(), currentNode->pose()));
				currentNode = &nodes.find(currentNode->fromId())->second;
			}
			path.push_front(std::make_pair(startNode, poses.at(startNode)));
			break;
		}

		// lookup neighbors
		for(std::multimap<int, int>::const_iterator iter = links.find(currentNode->id());
			iter!=links.end() && iter->first == currentNode->id();
			++iter)
		{
			std::map<int, Node>::iterator nodeIter = nodes.find(iter->second);
			if(nodeIter == nodes.end())
			{
				std::map<int, rtabmap::Transform>::const_iterator poseIter = poses.find(iter->second);
				if(poseIter == poses.end())
				{
					UERROR("Next pose %d (from %d) should be found in poses! Ignoring it!", iter->second, iter->first);
				}
				else
				{
					Node n(iter->second, currentNode->id(), poseIter->second);
					n.setCostSoFar(currentNode->costSoFar() + currentNode->distFrom(poseIter->second));
					n.setDistToEnd(n.distFrom(endPose));

					nodes.insert(std::make_pair(iter->second, n));
					if(updateNewCosts)
					{
						pqmap.insert(std::make_pair(n.totalCost(), n.id()));
					}
					else
					{
						pq.push(Pair(n.id(), n.totalCost()));
					}
				}
			}
			else if(updateNewCosts && nodeIter->second.isOpened())
			{
				float newCostSoFar = currentNode->costSoFar() + currentNode->distFrom(nodeIter->second.pose());
				if(nodeIter->second.costSoFar() > newCostSoFar)
				{
					// update the cost in the priority queue
					for(std::multimap<float, int>::iterator mapIter=pqmap.begin(); mapIter!=pqmap.end(); ++mapIter)
					{
						if(mapIter->second == nodeIter->first)
						{
							pqmap.erase(mapIter);
							nodeIter->second.setCostSoFar(newCostSoFar);
							pqmap.insert(std::make_pair(nodeIter->second.totalCost(), nodeIter->first));
							break;
						}
					}
				}
			}
		}
	}
	return path;
}

// Dijksta
std::list<int> RTABMAP_EXP computePath(
			const std::multimap<int, Link> & links,
			int from,
			int to,
			bool updateNewCosts,
			bool useSameCostForAllLinks)
{
	std::list<int> path;

	int startNode = from;
	int endNode = to;
	std::map<int, Node> nodes;
	nodes.insert(std::make_pair(startNode, Node(startNode, 0, Transform())));
	std::priority_queue<Pair, std::vector<Pair>, Order> pq;
	std::multimap<float, int> pqmap;
	if(updateNewCosts)
	{
		pqmap.insert(std::make_pair(0, startNode));
	}
	else
	{
		pq.push(Pair(startNode, 0));
	}

	while((updateNewCosts && pqmap.size()) || (!updateNewCosts && pq.size()))
	{
		Node * currentNode;
		if(updateNewCosts)
		{
			currentNode = &nodes.find(pqmap.begin()->second)->second;
			pqmap.erase(pqmap.begin());
		}
		else
		{
			currentNode = &nodes.find(pq.top().first)->second;
			pq.pop();
		}

		currentNode->setClosed(true);

		if(currentNode->id() == endNode)
		{
			while(currentNode->id()!=startNode)
			{
				path.push_front(currentNode->id());
				currentNode = &nodes.find(currentNode->fromId())->second;
			}
			path.push_front(startNode);
			break;
		}

		// lookup neighbors
		for(std::multimap<int, Link>::const_iterator iter = links.find(currentNode->id());
			iter!=links.end() && iter->first == currentNode->id();
			++iter)
		{
			std::map<int, Node>::iterator nodeIter = nodes.find(iter->second.to());
			float cost = 1;
			if(!useSameCostForAllLinks)
			{
				cost = iter->second.transform().getNorm();
			}
			if(nodeIter == nodes.end())
			{
				Node n(iter->second.to(), currentNode->id(), Transform());

				n.setCostSoFar(currentNode->costSoFar() + cost);
				nodes.insert(std::make_pair(iter->second.to(), n));
				if(updateNewCosts)
				{
					pqmap.insert(std::make_pair(n.totalCost(), n.id()));
				}
				else
				{
					pq.push(Pair(n.id(), n.totalCost()));
				}
			}
			else if(!useSameCostForAllLinks && updateNewCosts && nodeIter->second.isOpened())
			{
				float newCostSoFar = currentNode->costSoFar() + cost;
				if(nodeIter->second.costSoFar() > newCostSoFar)
				{
					// update the cost in the priority queue
					for(std::multimap<float, int>::iterator mapIter=pqmap.begin(); mapIter!=pqmap.end(); ++mapIter)
					{
						if(mapIter->second == nodeIter->first)
						{
							pqmap.erase(mapIter);
							nodeIter->second.setCostSoFar(newCostSoFar);
							pqmap.insert(std::make_pair(nodeIter->second.totalCost(), nodeIter->first));
							break;
						}
					}
				}
			}
		}
	}
	return path;
}


// return path starting from "fromId" (Identity pose for the first node)
std::list<std::pair<int, Transform> > computePath(
		int fromId,
		int toId,
		const Memory * memory,
		bool lookInDatabase,
		bool updateNewCosts,
		float linearVelocity,  // m/sec
		float angularVelocity) // rad/sec
{
	UASSERT(memory!=0);
	UASSERT(fromId>=0);
	UASSERT(toId>=0);
	std::list<std::pair<int, Transform> > path;
	UDEBUG("fromId=%d, toId=%d, lookInDatabase=%d, updateNewCosts=%d, linearVelocity=%f, angularVelocity=%f",
			fromId,
			toId,
			lookInDatabase?1:0,
			updateNewCosts?1:0,
			linearVelocity,
			angularVelocity);

	std::multimap<int, Link> allLinks;
	if(lookInDatabase)
	{
		// Faster to load all links in one query
		UTimer t;
		allLinks = memory->getAllLinks(lookInDatabase);
		UINFO("getting all %d links time = %f s", (int)allLinks.size(), t.ticks());
	}

	//dijkstra
	int startNode = fromId;
	int endNode = toId;
	std::map<int, Node> nodes;
	nodes.insert(std::make_pair(startNode, Node(startNode, 0, Transform::getIdentity())));
	std::priority_queue<Pair, std::vector<Pair>, Order> pq;
	std::multimap<float, int> pqmap;
	if(updateNewCosts)
	{
		pqmap.insert(std::make_pair(0, startNode));
	}
	else
	{
		pq.push(Pair(startNode, 0));
	}

	while((updateNewCosts && pqmap.size()) || (!updateNewCosts && pq.size()))
	{
		Node * currentNode;
		if(updateNewCosts)
		{
			currentNode = &nodes.find(pqmap.begin()->second)->second;
			pqmap.erase(pqmap.begin());
		}
		else
		{
			currentNode = &nodes.find(pq.top().first)->second;
			pq.pop();
		}

		currentNode->setClosed(true);

		if(currentNode->id() == endNode)
		{
			while(currentNode->id()!=startNode)
			{
				path.push_front(std::make_pair(currentNode->id(), currentNode->pose()));
				currentNode = &nodes.find(currentNode->fromId())->second;
			}
			path.push_front(std::make_pair(startNode, currentNode->pose()));
			break;
		}

		// lookup neighbors
		std::map<int, Link> links;
		if(allLinks.size() == 0)
		{
			links = memory->getLinks(currentNode->id(), lookInDatabase);
		}
		else
		{
			for(std::multimap<int, Link>::const_iterator iter = allLinks.lower_bound(currentNode->id());
				iter!=allLinks.end() && iter->first == currentNode->id();
				++iter)
			{
				links.insert(std::make_pair(iter->second.to(), iter->second));
			}
		}
		for(std::map<int, Link>::const_iterator iter = links.begin(); iter!=links.end(); ++iter)
		{
			Transform nextPose = currentNode->pose()*iter->second.transform();
			float cost = 0.0f;
			if(linearVelocity <= 0.0f && angularVelocity <= 0.0f)
			{
				// use distance only
				cost = iter->second.transform().getNorm();
			}
			else // use time
			{
				if(linearVelocity > 0.0f)
				{
					cost += iter->second.transform().getNorm()/linearVelocity;
				}
				if(angularVelocity > 0.0f)
				{
					Eigen::Vector4f v1 = Eigen::Vector4f(nextPose.x()-currentNode->pose().x(), nextPose.y()-currentNode->pose().y(), nextPose.z()-currentNode->pose().z(), 1.0f);
					Eigen::Vector4f v2 = nextPose.rotation().toEigen4f()*Eigen::Vector4f(1,0,0,1);
					float angle = pcl::getAngle3D(v1, v2);
					cost += angle / angularVelocity;
				}
			}

			std::map<int, Node>::iterator nodeIter = nodes.find(iter->first);
			if(nodeIter == nodes.end())
			{
				Node n(iter->second.to(), currentNode->id(), nextPose);

				n.setCostSoFar(currentNode->costSoFar() + cost);
				nodes.insert(std::make_pair(iter->second.to(), n));
				if(updateNewCosts)
				{
					pqmap.insert(std::make_pair(n.totalCost(), n.id()));
				}
				else
				{
					pq.push(Pair(n.id(), n.totalCost()));
				}
			}
			else if(updateNewCosts && nodeIter->second.isOpened())
			{
				float newCostSoFar = currentNode->costSoFar() + cost;
				if(nodeIter->second.costSoFar() > newCostSoFar)
				{
					// update pose with new link
					nodeIter->second.setPose(nextPose);

					// update the cost in the priority queue
					for(std::multimap<float, int>::iterator mapIter=pqmap.begin(); mapIter!=pqmap.end(); ++mapIter)
					{
						if(mapIter->second == nodeIter->first)
						{
							pqmap.erase(mapIter);
							nodeIter->second.setCostSoFar(newCostSoFar);
							pqmap.insert(std::make_pair(nodeIter->second.totalCost(), nodeIter->first));
							break;
						}
					}
				}
			}
		}
	}

	// Debugging stuff
	if(ULogger::level() == ULogger::kDebug)
	{
		std::stringstream stream;
		std::vector<int> linkTypes(Link::kUndef, 0);
		std::list<std::pair<int, Transform> >::const_iterator previousIter = path.end();
		float length = 0.0f;
		for(std::list<std::pair<int, Transform> >::const_iterator iter=path.begin(); iter!=path.end();++iter)
		{
			if(iter!=path.begin())
			{
				stream << ",";
			}

			if(previousIter!=path.end())
			{
				//UDEBUG("current  %d = %s", iter->first, iter->second.prettyPrint().c_str());
				if(allLinks.size())
				{
					std::multimap<int, Link>::iterator jter = graph::findLink(allLinks, previousIter->first, iter->first);
					if(jter != allLinks.end())
					{
						//Transform nextPose = iter->second;
						//Eigen::Vector4f v1 = Eigen::Vector4f(nextPose.x()-previousIter->second.x(), nextPose.y()-previousIter->second.y(), nextPose.z()-previousIter->second.z(), 1.0f);
						//Eigen::Vector4f v2 = nextPose.rotation().toEigen4f()*Eigen::Vector4f(1,0,0,1);
						//float angle = pcl::getAngle3D(v1, v2);
						//float cost = angle ;
						//UDEBUG("v1=%f,%f,%f v2=%f,%f,%f a=%f", v1[0], v1[1], v1[2], v2[0], v2[1], v2[2], cost);

						UASSERT(jter->second.type() >= Link::kNeighbor && jter->second.type()<Link::kUndef);
						++linkTypes[jter->second.type()];
						stream << "[" << jter->second.type() << "]";
						length += jter->second.transform().getNorm();
					}
				}
			}

			stream << iter->first;

			previousIter=iter;
		}
		UDEBUG("Path (%f m) = [%s]", length, stream.str().c_str());
		std::stringstream streamB;
		for(unsigned int i=0; i<linkTypes.size(); ++i)
		{
			if(i > 0)
			{
				streamB << " ";
			}
			streamB << i << "=" << linkTypes[i];
		}
		UDEBUG("Link types = %s", streamB.str().c_str());
	}

	return path;
}

int findNearestNode(
		const std::map<int, rtabmap::Transform> & nodes,
		const rtabmap::Transform & targetPose)
{
	int id = 0;
	if(nodes.size() && !targetPose.isNull())
	{
		pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
		cloud->resize(nodes.size());
		std::vector<int> ids(nodes.size());
		int oi = 0;
		for(std::map<int, Transform>::const_iterator iter = nodes.begin(); iter!=nodes.end(); ++iter)
		{
			(*cloud)[oi] = pcl::PointXYZ(iter->second.x(), iter->second.y(), iter->second.z());
			ids[oi++] = iter->first;
		}

		std::map<int, float> foundNodes;
		if(cloud->size())
		{
			pcl::search::KdTree<pcl::PointXYZ>::Ptr kdTree(new pcl::search::KdTree<pcl::PointXYZ>);
			kdTree->setInputCloud(cloud);
			std::vector<int> ind;
			std::vector<float> dist;
			pcl::PointXYZ pt(targetPose.x(), targetPose.y(), targetPose.z());
			kdTree->nearestKSearch(pt, 1, ind, dist);
			if(ind.size() && dist.size() && ind[0] >= 0)
			{
				UDEBUG("Nearest node = %d: %f", ids[ind[0]], dist[0]);
				id = ids[ind[0]];
			}
		}
	}
	return id;
}

// return <id, sqrd distance>, excluding query
std::map<int, float> getNodesInRadius(
		int nodeId,
		const std::map<int, Transform> & nodes,
		float radius)
{
	UASSERT(uContains(nodes, nodeId));
	std::map<int, float> foundNodes;
	if(nodes.size() <= 1)
	{
		return foundNodes;
	}

	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
	cloud->resize(nodes.size());
	std::vector<int> ids(nodes.size());
	int oi = 0;
	for(std::map<int, Transform>::const_iterator iter = nodes.begin(); iter!=nodes.end(); ++iter)
	{
		if(iter->first != nodeId)
		{
			(*cloud)[oi] = pcl::PointXYZ(iter->second.x(), iter->second.y(), iter->second.z());
			UASSERT_MSG(pcl::isFinite((*cloud)[oi]), uFormat("Invalid pose (%d) %s", iter->first, iter->second.prettyPrint().c_str()).c_str());
			ids[oi] = iter->first;
			++oi;
		}
	}
	cloud->resize(oi);
	ids.resize(oi);

	Transform fromT = nodes.at(nodeId);

	if(cloud->size())
	{
		pcl::search::KdTree<pcl::PointXYZ>::Ptr kdTree(new pcl::search::KdTree<pcl::PointXYZ>);
		kdTree->setInputCloud(cloud);
		std::vector<int> ind;
		std::vector<float> sqrdDist;
		pcl::PointXYZ pt(fromT.x(), fromT.y(), fromT.z());
		kdTree->radiusSearch(pt, radius, ind, sqrdDist, 0);
		for(unsigned int i=0; i<ind.size(); ++i)
		{
			if(ind[i] >=0)
			{
				foundNodes.insert(std::make_pair(ids[ind[i]], sqrdDist[i]));
			}
		}
	}
	UDEBUG("found nodes=%d", (int)foundNodes.size());
	return foundNodes;
}

// return <id, Transform>, excluding query
std::map<int, Transform> getPosesInRadius(
		int nodeId,
		const std::map<int, Transform> & nodes,
		float radius,
		float angle)
{
	UASSERT(uContains(nodes, nodeId));
	std::map<int, Transform> foundNodes;
	if(nodes.size() <= 1)
	{
		return foundNodes;
	}

	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
	cloud->resize(nodes.size());
	std::vector<int> ids(nodes.size());
	int oi = 0;
	for(std::map<int, Transform>::const_iterator iter = nodes.begin(); iter!=nodes.end(); ++iter)
	{
		if(iter->first != nodeId)
		{
			(*cloud)[oi] = pcl::PointXYZ(iter->second.x(), iter->second.y(), iter->second.z());
			UASSERT_MSG(pcl::isFinite((*cloud)[oi]), uFormat("Invalid pose (%d) %s", iter->first, iter->second.prettyPrint().c_str()).c_str());
			ids[oi] = iter->first;
			++oi;
		}
	}
	cloud->resize(oi);
	ids.resize(oi);

	Transform fromT = nodes.at(nodeId);

	if(cloud->size())
	{
		pcl::search::KdTree<pcl::PointXYZ>::Ptr kdTree(new pcl::search::KdTree<pcl::PointXYZ>);
		kdTree->setInputCloud(cloud);
		std::vector<int> ind;
		std::vector<float> sqrdDist;
		pcl::PointXYZ pt(fromT.x(), fromT.y(), fromT.z());
		kdTree->radiusSearch(pt, radius, ind, sqrdDist, 0);

		Eigen::Vector3f vA = fromT.toEigen3f().rotation()*Eigen::Vector3f(1,0,0);

		for(unsigned int i=0; i<ind.size(); ++i)
		{
			if(ind[i] >=0)
			{
				if(angle > 0.0f)
				{
					const Transform & checkT = nodes.at(ids[ind[i]]);
					// same orientation?
					Eigen::Vector3f vB = checkT.toEigen3f().rotation()*Eigen::Vector3f(1,0,0);
					double a = pcl::getAngle3D(Eigen::Vector4f(vA[0], vA[1], vA[2], 0), Eigen::Vector4f(vB[0], vB[1], vB[2], 0));
					if(a <= angle)
					{
						foundNodes.insert(std::make_pair(ids[ind[i]], nodes.at(ids[ind[i]])));
					}
				}
				else
				{
					foundNodes.insert(std::make_pair(ids[ind[i]], nodes.at(ids[ind[i]])));
				}
			}
		}
	}
	UDEBUG("found nodes=%d", (int)foundNodes.size());
	return foundNodes;
}

float computePathLength(
		const std::vector<std::pair<int, Transform> > & path,
		unsigned int fromIndex,
		unsigned int toIndex)
{
	float length = 0.0f;
	if(path.size() > 1)
	{
		UASSERT(fromIndex  < path.size() && toIndex < path.size() && fromIndex <= toIndex);
		if(fromIndex >= toIndex)
		{
			toIndex = (unsigned int)path.size()-1;
		}
		float x=0, y=0, z=0;
		for(unsigned int i=fromIndex; i<toIndex-1; ++i)
		{
			x += fabs(path[i].second.x() - path[i+1].second.x());
			y += fabs(path[i].second.y() - path[i+1].second.y());
			z += fabs(path[i].second.z() - path[i+1].second.z());
		}
		length = sqrt(x*x + y*y + z*z);
	}
	return length;
}

// return all paths linked only by neighbor links
std::list<std::map<int, Transform> > getPaths(
		std::map<int, Transform> poses,
		const std::multimap<int, Link> & links)
{
	std::list<std::map<int, Transform> > paths;
	if(poses.size() && links.size())
	{
		// Segment poses connected only by neighbor links
		while(poses.size())
		{
			std::map<int, Transform> path;
			for(std::map<int, Transform>::iterator iter=poses.begin(); iter!=poses.end();)
			{
				std::multimap<int, Link>::const_iterator jter = findLink(links, path.rbegin()->first, iter->first);
				if(path.size() == 0 || (jter != links.end() && (jter->second.type() == Link::kNeighbor || jter->second.type() == Link::kNeighborMerged)))
				{
					path.insert(*iter);
					poses.erase(iter++);
				}
				else
				{
					break;
				}
			}
			UASSERT(path.size());
			paths.push_back(path);
		}

	}
	return paths;
}

} /* namespace graph */

} /* namespace rtabmap */
