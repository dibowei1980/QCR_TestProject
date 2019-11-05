// 2.2 GeodeAndGeoInformation.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"

#include <osg/ref_ptr>
#include <osgDB/Registry>
#include <osgDB/WriteFile>
#include <osg/Notify>

#include <iostream>

using std::endl;

osg::ref_ptr<osg::Node> createSceneGraph();


// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单
int main()
{
	osg::ref_ptr<osg::Node> root = createSceneGraph();
	if (!root.valid())
		osg::notify(osg::FATAL) << "Failed in createSceneGraph()." << endl;

	bool result = osgDB::writeNodeFile(*(root.get()), "Simple.osg");

	if (!result)
		osg::notify(osg::FATAL) << "Failed in osgDB::writeNode()." << endl;
}





