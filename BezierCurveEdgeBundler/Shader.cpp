#include "Graph.h"
#include <osg/Geometry>



void Graph::ShaderCreate()
{
	
	//设置着色器
	osg::Shader* vertexShader = new osg::Shader(osg::Shader::VERTEX);
	osg::Shader* fragmentShader = new osg::Shader(osg::Shader::FRAGMENT);
	vertexShader->loadShaderSourceFromFile("shader/shader.vert");
	fragmentShader->loadShaderSourceFromFile("shader/shader.frag");
	osg::Program* program = new osg::Program;
	program->addShader(vertexShader);
	program->addShader(fragmentShader);
	//根目录的渲染状态
	osg::Group* rot = new osg::Group;
	addChild(rot);
	rot->addChild(mBezierCurve);
	//mBezierCurve->setNodeMask(0);
	rot->addChild(mDrawEdgePoints);
	rot->addChild(mRoadCurve);
	rot->addChild(mColorBezierCurve);
	
	

	osg::StateSet* stateset = rot->getOrCreateStateSet();
	stateset->setAttributeAndModes(program,osg::StateAttribute::ON);

	


	
}