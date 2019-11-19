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

	//数据绑定：法线、颜色，绑定方式为：
	//BIND_OFF不启动用绑定/BIND_OVERALL绑定全部顶点/BIND_PER_PRIMITIVE_SET单个绘图基元绑定/BIND_PER_PRIMITIVE单个独立的绘图基元绑定/BIND_PER_VERTIE单个顶点绑定
	//采用BIND_PER_PRIMITIVE绑定方式，则OSG采用glBegin()/glEnd()函数进行渲染，因为该绑定方式为每个独立的几何图元设置一种绑定方式

	//创建颜色数组
	osg::ref_ptr<osg::Vec4Array> vc = new osg::Vec4Array();
	//添加数据
	vc->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
	vc->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
	vc->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
	vc->push_back(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
	//设置颜色数组
	geom->setColorArray(vc.get());
	//设置颜色的绑定方式为单个顶点
	geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
	//创建法线数组
	osg::ref_ptr<osg::Vec3Array> nc = new osg::Vec3Array();
	//添加法线
	nc->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));
	//设置法线数组
	geom->setNormalArray(nc.get());
	//设置法线的绑定方式
	geom->setNormalBinding(osg::Geometry::BIND_OVERALL);
	//添加图元，绘制基元为四边形
	//数据解析，即指定向量数据和绑定方式后，指定渲染几何体的方式，不同方式渲染出的图形不同，即时效果相同，可能面数或内部机制等也有区别，函数为：
	//bool addPrimitiveSet(PrimitiveSet *primitiveset)参数说明：osg::PrimitiveSet是无法初始化的虚基类，因此主要调用它的子类指定数据渲染，最常用为osg::DrawArrays
	//osg::DrawArrays(GLenum mode, GLint first, GLsizei count)参数为指定的绘图基元、绘制几何体的第一个顶点数在指定顶点的位置数、使用的顶点的总数
	//PrimitiveSet类继承自osg::Object虚基类，但不具备一般一般场景中的特性，PrimitiveSet类主要封装了OpenGL的绘图基元，常见绘图基元如下
	//POINTS点/LINES线/LINE_STRIP多线段/LINE_LOOP封闭线
	//TRIANGLES一系列三角形(不共顶点)/TRIANGLE_STRIP一系列三角形(共用后面两个顶点)/TRIANGLE_FAN一系列三角形，顶点顺序与上一条语句绘制的三角形不同
	//QUADS四边形/QUAD_STRIP一系列四边形/POLYGON多边形
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));
	return geom;
}