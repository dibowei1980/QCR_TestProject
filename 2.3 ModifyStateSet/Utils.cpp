#include "pch.h"

#include <osg/Geode>
#include <osg/Group>
#include <osg/MatrixTransform>
#include <osg/Geometry>
#include <osg/StateSet>
#include <osg/StateAttribute>
#include <osg/ShadeModel>
#include <osg/ShadeModel>
#include <osg/CullFace>
#include <osg/PolygonMode>
#include <osg/LineWidth>

osg::ref_ptr<osg::Node> createSenceGraph()
{
	//创建Group根节点
	osg::ref_ptr<osg::Group> root = new osg::Group;
	{
		//在根节点的StateSet中禁止光照。
		//使用PROTECTED以保证这一修改不会被osgviewer覆盖
		osg::StateSet* state = root->getOrCreateStateSet();
		state->setMode(GL_LIGHTING,
						osg::StateAttribute::OFF |
						osg::StateAttribute::PROTECTED);
	}

	//创建Geode中节点并关联Drawable。
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(createDrawable().get());
	osg::Matrix m;
	{
		//左上角：使用缺少属性渲染几何体。
		osg::ref_ptr<osg::MatrixTransform> mt =
			new osg::MatrixTransform;
		m.makeTranslate(-2.f, 0.f, 2.f);
		mt->setMatrix(m);
		root->addChild(mt.get());
		mt->addChild(geode.get());
	}
	{
		//右上角：设置着色模式为FLAT(单色)。
		osg::ref_ptr<osg::MatrixTransform> mt =
			new osg::MatrixTransform;
		m.makeTranslate(2.f, 0.f, 2.f);
		mt->setMatrix(m);
		root->addChild(mt.get());
		mt->addChild(geode.get());
		osg::StateSet* state = mt->getOrCreateStateSet();
		osg::ShadeModel* sm = new osg::ShadeModel();
		sm->setMode(osg::ShadeModel::FLAT);
		state->setAttribute(sm);
	}
	{
		//左下角：开启背面剔除。
		osg::ref_ptr<osg::MatrixTransform> mt =
			new osg::MatrixTransform;
		m.makeTranslate(-2.f, 0.f, -2.f);
		mt->setMatrix(m);
		root->addChild(mt.get());
		mt->addChild(geode.get());
		osg::StateSet*  state = mt->getOrCreateStateSet();
		osg::CullFace* cf = new osg::CullFace(); //缺少值为BACK
		state->setAttributeAndModes(cf);
	}
	{
		//右下角：设置多边形填充模式为LINE（线框）。
		osg::ref_ptr<osg::MatrixTransform> mt =
			new osg::MatrixTransform;
		m.makeTranslate(2.f, 0.f, -2.f);
		mt->setMatrix(m);
		root->addChild(mt.get());
		mt->addChild(geode.get());
		osg::StateSet* state = mt->getOrCreateStateSet();
		osg::PolygonMode* pm = new osg::PolygonMode(
			osg::PolygonMode::FRONT_AND_BACK,
			osg::PolygonMode::LINE);
		state->setAttributeAndModes(pm);
		//同时还设置线宽为3。
		osg::LineWidth* lw = new osg::LineWidth(3.f);
		state->setAttribute(lw);
	}
	return root.get();
}

osg::ref_ptr<osg::Drawable> createDrawable()
{
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;

	osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array;
	//添加顶点
	v->push_back(osg::Vec3(-2.0f, 0.0f, 2.0f));
	v->push_back(osg::Vec3(2.0f, 0.0f, 2.0f));
	v->push_back(osg::Vec3(2.0f, 0.0f, -2.0f));
	v->push_back(osg::Vec3(-2.0f, 0.0f, -2.0f));
	//设置顶点数据
	geom->setVertexArray(v.get());

	//创建纹理坐标数组
	geom->setTexCoordArray(0, v.get());


}