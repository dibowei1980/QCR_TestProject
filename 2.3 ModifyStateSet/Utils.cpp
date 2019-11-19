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

	//���ݰ󶨣����ߡ���ɫ���󶨷�ʽΪ��
	//BIND_OFF�������ð�/BIND_OVERALL��ȫ������/BIND_PER_PRIMITIVE_SET������ͼ��Ԫ��/BIND_PER_PRIMITIVE���������Ļ�ͼ��Ԫ��/BIND_PER_VERTIE���������
	//����BIND_PER_PRIMITIVE�󶨷�ʽ����OSG����glBegin()/glEnd()����������Ⱦ����Ϊ�ð󶨷�ʽΪÿ�������ļ���ͼԪ����һ�ְ󶨷�ʽ

	//������ɫ����
	osg::ref_ptr<osg::Vec4Array> vc = new osg::Vec4Array();
	//�������
	vc->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
	vc->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
	vc->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
	vc->push_back(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
	//������ɫ����
	geom->setColorArray(vc.get());
	//������ɫ�İ󶨷�ʽΪ��������
	geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
	//������������
	osg::ref_ptr<osg::Vec3Array> nc = new osg::Vec3Array();
	//��ӷ���
	nc->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));
	//���÷�������
	geom->setNormalArray(nc.get());
	//���÷��ߵİ󶨷�ʽ
	geom->setNormalBinding(osg::Geometry::BIND_OVERALL);
	//���ͼԪ�����ƻ�ԪΪ�ı���
	//���ݽ�������ָ���������ݺͰ󶨷�ʽ��ָ����Ⱦ������ķ�ʽ����ͬ��ʽ��Ⱦ����ͼ�β�ͬ����ʱЧ����ͬ�������������ڲ����Ƶ�Ҳ�����𣬺���Ϊ��
	//bool addPrimitiveSet(PrimitiveSet *primitiveset)����˵����osg::PrimitiveSet���޷���ʼ��������࣬�����Ҫ������������ָ��������Ⱦ�����Ϊosg::DrawArrays
	//osg::DrawArrays(GLenum mode, GLint first, GLsizei count)����Ϊָ���Ļ�ͼ��Ԫ�����Ƽ�����ĵ�һ����������ָ�������λ������ʹ�õĶ��������
	//PrimitiveSet��̳���osg::Object����࣬�����߱�һ��һ�㳡���е����ԣ�PrimitiveSet����Ҫ��װ��OpenGL�Ļ�ͼ��Ԫ��������ͼ��Ԫ����
	//POINTS��/LINES��/LINE_STRIP���߶�/LINE_LOOP�����
	//TRIANGLESһϵ��������(��������)/TRIANGLE_STRIPһϵ��������(���ú�����������)/TRIANGLE_FANһϵ�������Σ�����˳������һ�������Ƶ������β�ͬ
	//QUADS�ı���/QUAD_STRIPһϵ���ı���/POLYGON�����
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));
	return geom;
}