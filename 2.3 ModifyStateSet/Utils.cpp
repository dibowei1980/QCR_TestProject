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
	//����Group���ڵ�
	osg::ref_ptr<osg::Group> root = new osg::Group;
	{
		//�ڸ��ڵ��StateSet�н�ֹ���ա�
		//ʹ��PROTECTED�Ա�֤��һ�޸Ĳ��ᱻosgviewer����
		osg::StateSet* state = root->getOrCreateStateSet();
		state->setMode(GL_LIGHTING,
						osg::StateAttribute::OFF |
						osg::StateAttribute::PROTECTED);
	}

	//����Geode�нڵ㲢����Drawable��
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(createDrawable().get());
	osg::Matrix m;
	{
		//���Ͻǣ�ʹ��ȱ��������Ⱦ�����塣
		osg::ref_ptr<osg::MatrixTransform> mt =
			new osg::MatrixTransform;
		m.makeTranslate(-2.f, 0.f, 2.f);
		mt->setMatrix(m);
		root->addChild(mt.get());
		mt->addChild(geode.get());
	}
	{
		//���Ͻǣ�������ɫģʽΪFLAT(��ɫ)��
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
		//���½ǣ����������޳���
		osg::ref_ptr<osg::MatrixTransform> mt =
			new osg::MatrixTransform;
		m.makeTranslate(-2.f, 0.f, -2.f);
		mt->setMatrix(m);
		root->addChild(mt.get());
		mt->addChild(geode.get());
		osg::StateSet*  state = mt->getOrCreateStateSet();
		osg::CullFace* cf = new osg::CullFace(); //ȱ��ֵΪBACK
		state->setAttributeAndModes(cf);
	}
	{
		//���½ǣ����ö�������ģʽΪLINE���߿򣩡�
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
		//ͬʱ�������߿�Ϊ3��
		osg::LineWidth* lw = new osg::LineWidth(3.f);
		state->setAttribute(lw);
	}
	return root.get();
}

osg::ref_ptr<osg::Drawable> createDrawable()
{
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;

	osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array;
	//��Ӷ���
	v->push_back(osg::Vec3(-2.0f, 0.0f, 2.0f));
	v->push_back(osg::Vec3(2.0f, 0.0f, 2.0f));
	v->push_back(osg::Vec3(2.0f, 0.0f, -2.0f));
	v->push_back(osg::Vec3(-2.0f, 0.0f, -2.0f));
	//���ö�������
	geom->setVertexArray(v.get());

	//����������������
	geom->setTexCoordArray(0, v.get());


}