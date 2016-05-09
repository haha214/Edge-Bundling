#ifndef TIXML_USE_STL
#define TIXML_USE_STL
#endif

#include "tinystr.h"
#include "tinyxml.h"
#include <iostream>
#include <vector>

#define BOX 0
#define CAPPEDCYLINDER 1
#define CONE 2
#define CYLINDER 3
#define PLANE 4
#define RAY 5
#define SPHERE 6
#define TRIMESH 7
#define MASS_SHAPE 8
#define MASS_STRUCT 9
#define NO_INFO 10
#define AMOTOR 11
#define BALL 12
#define FIXED 13
#define HINGE 14
#define HINGE2 15
#define SLIDER 16
#define UNIVERSAL 17
#define PISTON 18

namespace TinyXode
{

class Joint;
class Geom;
class Body;
class Group;
class JointGroup;


/************************************************************************/
/* 类名：  XodeElement                                                   */
/* 功能：  所有元素类的基类                                             */
/* 说明：  只有一个TiXmlElement指针类型成员变量，保存了一个节点的指针 */
/* AttributeToVar    ：  重载函数，将从xml中提取的字符串类型变量转化为三种其他类型 */
/* PrintElement      ：  虚函数，将这个节点的所有信息输出                 */
/* PrintChildElement :   将一个子节点的所有数据输出                      */
/* PrintElementName  ：  将当前节点的名称输出                                */
/************************************************************************/

class XodeElement
{
protected:
	TiXmlElement* pEle;
public:
	XodeElement(TiXmlElement* pEle=NULL);
	~XodeElement();
	void AttributeToVar(const char* str,int &num);       //将属性值转化为整数
	void AttributeToVar(const char* str,float &num);      //将属性值转化为浮点数
	void AttributeToVar(const char* cha,std::string &str); //将属性值转化为字符串
	virtual void PrintElement(int level=0,std::string eleName="");
	void PrintChildElement(XodeElement* childEle,int level=0,std::string eleName="");
	void PrintElementName(int level=0,std::string eleName="");
};


/************************************************************************/
/* 类名：  Colour                                                   */
/* 功能：  保存一个颜色的RGB三个分量值，通过构造函数对三个值赋值    */     
/************************************************************************/
class Colour:public XodeElement
{
public:
	float r;
	float g;
	float b;
public:
	Colour(TiXmlElement* pEle=NULL);
	virtual void PrintElement(int level=0,std::string eleName="");
};


class Ext:public XodeElement
{
public:
	std::string ename;
	bool isLocation;
	std::string location;
	Colour* colour;
public:
	Ext(TiXmlElement* pEle=NULL);
	~Ext();
	virtual void PrintElement(int level=0,std::string eleName="");
};


/************************************************************************/
/* 类名：  ElementWithExt                                                  */
/* 功能：  作为所有具有Ext类成员变量的类的基类                               */
/* 说明：  具有Ext指针类型的智能指针，构造函数用来初始化它               */
/*          因此所以继承于它的类就不用再初始化ext */
/************************************************************************/

class ElementWithExt:public XodeElement
{
public:
	std::vector<Ext*> ext;                   //Ext指针类型的可变长度数组，表示可容纳0到无穷多个Ext指针
public:
	ElementWithExt(TiXmlElement* pEle=NULL);
	~ElementWithExt();
	virtual void PrintElement(int level=0,std::string eleName="");
};


/************************************************************************/
/* 类名：  Position                                                      */
/* 父类：  XodeElement                                                 */
/* 功能：  代表一个位置信息                                            */
/* 说明：  在构造函数中通过参数传递的指针对xyz值进行赋值               */
/************************************************************************/

class Position:public XodeElement
{
public:
	float x;        //位置的x坐标
	float y;        //位置的y坐标
	float z;        //位置的z坐标
public:
	Position(TiXmlElement* pEle=NULL);   //构造函数，在其中通过pEle指针对xyz赋值
	virtual void PrintElement(int level=0,std::string eleName="");
};

/************************************************************************/
/* 类名：  Matrix4f                                                     */
/* 父类：  XodeElement                                                 */
/* 功能：  代表一个4行4列变换矩阵                                       */
/* 说明：  在构造函数中通过参数传递的指针对矩阵内数据进行赋值           */
/************************************************************************/

class Matrix4f:public XodeElement
{
public:
	float m00;            //矩阵的第0行0列数据
	float m01;
	float m02;
	float m03;
	float m10;
	float m11;
	float m12;
	float m13;
	float m20;
	float m21;
	float m22;
	float m23;
	float m30;
	float m31;
	float m32;
	float m33;
public:
	Matrix4f(TiXmlElement* pEle=NULL);   //构造函数
	 
};

/************************************************************************/
/* 类名：  Euler                                                     */
/* 父类：  XodeElement                                                 */
/* 功能：  代表一个变换中需要的欧拉角                                 */
/* 说明：  在构造函数中通过参数传递的指针对矩阵内数据进行赋值           */
/************************************************************************/
class Euler:public XodeElement
{
public:
	float x;
	float y;
	float z;
	std::string aformat;
public:
	Euler(TiXmlElement* pEle=NULL);
	virtual void PrintElement(int level=0,std::string eleName="");

};

class Axisangle:public Euler
{
public:
	float angle;
public:
	Axisangle(TiXmlElement* pEle=NULL);
	virtual void PrintElement(int level=0,std::string eleName="");
};

class Quaternion:public Euler
{
public:
	float w;
public:
	Quaternion(TiXmlElement* pEle=NULL);
	virtual void PrintElement(int level=0,std::string eleName="");
};

/************************************************************************/
/* 类名：  Rotation                                                     */
/* 父类：  XodeElement                                                 */
/* 功能：  代表一个变换中需要的旋转信息，包括欧拉角，偏轴角和一个四元组   */
/* 说明：  在构造函数中通过参数传递的指针对矩阵内数据进行赋值           */
/************************************************************************/
class Rotation:public XodeElement
{
public:
	Euler* euler;                      //欧拉角
	Axisangle* axisangle;               //偏轴角
	Quaternion* quaternion;           
public:
	Rotation(TiXmlElement* pEle=NULL);
	~Rotation();
	virtual void PrintElement(int level=0,std::string eleName="");
};

/************************************************************************/
/* 类名：  Transform                                                      */
/* 父类：  ElementWithExt                                                 */
/* 功能：  代表一个变换信息，包含变换所需要的各种数据                      */
/* 说明：  在构造函数中通过参数传递的指针对所有成员变量进行赋值               */
/************************************************************************/
class Transform:public ElementWithExt
{
public:
	bool absolute;                       //表示是否采取绝对坐标
	std::string transformstyle;          //变换的类型
	float scale;                         //变换的尺度
	bool isMatrix;                       //表示是否采用矩阵表示变换，true时使用矩阵
	Position* position;                  //变换后的位置，仅在isMatrix为false时有效
	Rotation* rotation;                  //变换时的旋转量，在isMatrix为false时有效
	Matrix4f* matrix4f;                  //变换矩阵，仅在isMatrix为true时有效
public:
	Transform(TiXmlElement* pEle=NULL);  //构造函数，根据pEle指针初始化成员变量
	~Transform();
	virtual void PrintElement(int level=0,std::string eleName="");
};

/************************************************************************/
/* 类名：  SpaceBase                                                      */
/* 父类：  ElementWithExt                                                 */
/* 说明：  这是space中一些类的基类，他们含有五个共同的成员变量               */
/************************************************************************/
class SpaceBase:public ElementWithExt
{
public:
	std::vector<Joint*> joint;
	std::vector<Geom*> geom;
	std::vector<Body*> body;
	std::vector<Group*> group;
	std::vector<JointGroup*> jointgroup;
public:
	SpaceBase(TiXmlElement* pEle=NULL);
	~SpaceBase();
	int GetBody(std::string bodyName);
	virtual void PrintElement(int level=0,std::string eleName="");
};

class JointGroup:public SpaceBase
{
public:
	JointGroup(TiXmlElement* pEle=NULL);
};


class Box:public ElementWithExt
{
public:
	float sizex;
	float sizey;
	float sizez;
public:
	Box(TiXmlElement* pEle=NULL);
	virtual void PrintElement(int level=0,std::string eleName="");
};

//圆柱体类
class CappedCylinder:public ElementWithExt
{
public:
	float radius;    //截面半径
	float length;    //长度
public:
	CappedCylinder(TiXmlElement* pEle=NULL);
	virtual void PrintElement(int level=0,std::string eleName="");
};

//椎体类，是一种特殊的圆柱体，所以继承于CappedCylinder类
class Cone:public CappedCylinder
{
public:
	Cone(TiXmlElement* pEle=NULL);
};

//非封闭圆柱体类，因与圆柱体的参数相同，继承于CappedCylinder类
class Cylinder:public CappedCylinder
{
public:
	Cylinder(TiXmlElement* pEle=NULL);
};

//平面类
class Plane:public ElementWithExt
{
public:
	float a;
	float b;
	float c;
	float d;
public:
	Plane(TiXmlElement* pEle=NULL);
	virtual void PrintElement(int level=0,std::string eleName="");
};

//射线类
class Ray:public ElementWithExt
{
public:
	float length;        //射线长度
public:
	Ray(TiXmlElement* pEle=NULL);
	virtual void PrintElement(int level=0,std::string eleName="");
};


//球体类
class Sphere:public ElementWithExt
{
public:
	float radius;        //半径大小
public:
	Sphere(TiXmlElement* pEle=NULL);
	virtual void PrintElement(int level=0,std::string eleName="");
};

//点类
class V:public XodeElement
{
public:
	float x;         
	float y;
	float z;
	float e;
public:
	V(TiXmlElement* pEle=NULL);
	virtual void PrintElement(int level=0,std::string eleName="");
};

//三角形类
class T:public XodeElement
{
public:
	float ia;            //a边长
	float ib;
	float ic;
public:
	T(TiXmlElement* pEle=NULL);
	virtual void PrintElement(int level=0,std::string eleName="");
};

/************************************************************************/
/* 类名：  Vertices                                                                   */
/* 父类：  XodeElement
/* 说明：  点集，包含0个到多个点对象的指针
/************************************************************************/
class Vertices:public XodeElement
{
public:
	std::vector<V*> v;
public:
	Vertices(TiXmlElement* pEle=NULL);
	virtual void PrintElement(int level=0,std::string eleName="");
};

/************************************************************************/
/* 类名：  Triangles                                                                  */
/* 父类：  XodeElement
/* 说明：  三角形集，包含0个到多个三角形对象的指针
/************************************************************************/
class Triangles:public XodeElement
{
public:
	std::vector<T*> t;
public:
	Triangles(TiXmlElement* pEle=NULL);
	virtual void PrintElement(int level=0,std::string eleName="");
};

class Trimesh:public ElementWithExt
{
public:
	Vertices* vertices;
	Triangles* triangles;
public:
	Trimesh(TiXmlElement* pEle=NULL);
	virtual void PrintElement(int level=0,std::string eleName="");
};

class Geom:public SpaceBase
{
public:
	std::string name;
	Transform* transform;
	ElementWithExt* shape;
	int shapeKind;
public:
	Geom(TiXmlElement* pEle=NULL);
	~Geom();
	virtual void PrintElement(int level=0,std::string eleName="");
};

class Adjust:public XodeElement
{
public:
	float density;
	int total;
public:
	Adjust(TiXmlElement* pEle = NULL);
	virtual void PrintElement(int level=0,std::string eleName="");
};

/************************************************************************/
/* 类名：  MassShape                                                      */
/* 父类：  Adjust                                                 */
/* 功能：  代表用形状来表示的质点                                            */
/* 说明：  shapeKind表示质点形状，可选的CYLINDER、SPHERE、BOX、CAPPENCYLINDER  */
/*         shape为这些形状的父类指针，根据shapeKind的值初始化为相应的子类对象
/************************************************************************/

class MassShape:public Adjust
{
public:
	int shapeKind;
	ElementWithExt* shape;
public:
	MassShape(TiXmlElement* pEle = NULL);
	~MassShape();
	virtual void PrintElement(int level=0,std::string eleName="");
};

class Imatrix:public XodeElement
{
public:
	float m00;
	float m01;
	float m02;
	float m10;
	float m11;
	float m12;
	float m20;
	float m21;
	float m22;
public:
	Imatrix(TiXmlElement* pEle = NULL);
	virtual void PrintElement(int level=0,std::string eleName="");
};


class MassStruct:public Adjust
{
public:
	Position* centerGravity;
	Imatrix* imatrix;
public:
	MassStruct(TiXmlElement* pEle = NULL);
	~MassStruct();
	virtual void PrintElement(int level=0,std::string eleName="");
};

class Torque:public XodeElement
{
public:
	float x;
	float y;
	float z;
public:
	Torque(TiXmlElement* pEle=NULL);
	virtual void PrintElement(int level=0,std::string eleName="");
};

class Force:public Torque
{
public:
	Force(TiXmlElement* pEle=NULL);
};

class LinearVel:public Torque
{
public:
	LinearVel(TiXmlElement* pEle=NULL);
};

class AngularVel:public Torque
{
public:
	AngularVel(TiXmlElement* pEle=NULL);
};

class FiniteRotation:public XodeElement
{
public:
	std::string mode;
	float xaxis;
	float yaxis;
	float zaxis;
public:
	FiniteRotation(TiXmlElement* pEle=NULL);
	virtual void PrintElement(int level=0,std::string eleName="");
};

/************************************************************************/
/* 类名：  Mass                                                      */
/* 父类：  XodeElement                                                 */
/* 功能：  代表一个质点                                            */
/* 说明：  massKind表示质点的表示形式，当为MASS_SHAPE时用形状来表示          */
/*         massForm初始化为MassShape类的对象，当为MASS_STRUCT时表示用结构*/
/*         来表示质点，massForm初始化为MassStruct类的对象;当为NO_INFO时表示 */
/*         massForm为空                                                  */
/************************************************************************/
class Mass:public XodeElement
{
public:
	int massKind;
	XodeElement* massForm;
	Transform* transform;           
	Adjust* adjust;
	Mass* mass;                     //子质点对象指针
public:
	Mass(TiXmlElement* pEle=NULL);
	~Mass();
	virtual void PrintElement(int level=0,std::string eleName="");
};

class Body:public SpaceBase
{
public:
	bool enabled;
	std::string gravitymode;
	std::string name;
	Transform* transform;
	Torque* torque;
	Force* force;
	LinearVel* linearVel;
	AngularVel* angularVel;
	FiniteRotation* finiteRotation;
	Mass* mass;
public:
	Body(TiXmlElement* pEle=NULL);
	~Body();
	virtual void PrintElement(int level=0,std::string eleName="");
};

class Link:public XodeElement
{
public:
	std::string body;
	Transform* transform;
public:
	Link(TiXmlElement* pEle=NULL);
	~Link();
	virtual void PrintElement(int level=0,std::string eleName="");
};

class Link1:public Link
{
public:
	Link1(TiXmlElement* pEle=NULL);
	//virtual void PrintElement(int level=0,std::string eleName="");
};

class Link2:public Link
{
public:
	Link2(TiXmlElement* pEle=NULL);
	//virtual void PrintElement(int level=0,std::string eleName="");
};

class Anchor:public Position
{
public:
	Anchor(TiXmlElement* pEle=NULL);
};

class Axis:public XodeElement
{
public:
	float x;
	float y;
	float z;
	float LowStop;
	float HiStop;
	float Vel;
	float FMax;
	float FudgeFactor;
	float Bounce;
	float CFM;
	float StopERP;
	float StopCFM;
	float SuspensionERP;
	float SuspensionCFM;
public:
	Axis(TiXmlElement* pEle=NULL);
	virtual void PrintElement(int level=0,std::string eleName="");
};

/************************************************************************/
/* 类名：JointKindBase1                                                                     */
/* 父类：XodeElement
/* 说明：所有约束类型的第一种基类，仅有axis，如Fixed、Slider都是这个类型的约束
/************************************************************************/
class JointKindBase1:public XodeElement
{
public:
	std::vector<Axis*> axis;
public:
	JointKindBase1(TiXmlElement* pEle=NULL);
	~JointKindBase1();
	virtual void PrintElement(int level=0,std::string eleName="");
};

/************************************************************************/
/* 类名：JointKindBase2                                                                     */
/* 父类：JointKindBase1
/* 说明：所有约束类型的第二种基类，继承于JointKindBase1
/*       仅有axis，还有anchor，如Ball、Hinge都是这个类型的约束
/************************************************************************/
class JointKindBase2:public JointKindBase1
{
public:
	Anchor* anchor;
public:
	JointKindBase2(TiXmlElement* pEle=NULL);
	~JointKindBase2();
	virtual void PrintElement(int level=0,std::string eleName="");
};

class Amotor:public JointKindBase2
{
public:
	Amotor(TiXmlElement* pEle=NULL);
};

class Ball:public JointKindBase2
{
public:
	Ball(TiXmlElement* pEle=NULL);
};

class Fixed:public JointKindBase1
{
public:
	Fixed(TiXmlElement* pEle=NULL);
};

class Hinge:public JointKindBase2
{
public:
	Hinge(TiXmlElement* pEle=NULL);
};

class Hinge2:public JointKindBase2
{
public:
	Hinge2(TiXmlElement* pEle=NULL);
};

class Slider:public JointKindBase2
{
public:
	Slider(TiXmlElement* pEle=NULL);
};

class Piston:public JointKindBase2
{
public:
	Piston(TiXmlElement* pEle=NULL);
};
class Universal:public JointKindBase2
{
public:
	Universal(TiXmlElement* pEle=NULL);
};

class Joint:public ElementWithExt
{
public:
	std::string name;
	Link1* link1;
	Link2* link2;
	int jointKind;
	JointKindBase1* jointObject;
public:
	Joint(TiXmlElement* pEle=NULL);
	~Joint();
	virtual void PrintElement(int level=0,std::string eleName="");
};


class Space:public SpaceBase
{
public:
	Transform* transform;
public:
	Space(TiXmlElement* pEle=NULL);
	~Space();
	virtual void PrintElement(int level=0,std::string eleName="");
};

class Group:public SpaceBase
{
public:
	std::string name;
	Transform* transform;
public:
	Group(TiXmlElement* pEle=NULL);
	~Group();
};

class World:public ElementWithExt
{
public:
	Transform* transform;
	std::vector<Space*> space;
public:
	World(TiXmlElement* pEle=NULL);
	~World();
	virtual void PrintElement(int level=0,std::string eleName="");
};

/************************************************************************/
/* 类名： Xode 
/* 父类： XodeElement
/* 说明： 是xml文档中所有节点中的根节点，构造函数仅有无参数一种，
/*        因此不能通过构造函数对其初始化，仅能通过bool型函数LoadFile
/*        进行初始化，方法是传入一个xml文档的绝对或相对路径名
/************************************************************************/
class Xode:public XodeElement
{
public:
	std::string name;
	std::string version;
	std::vector<World*> world;
	std::vector<Ext*> ext;
	TiXmlDocument* pDoc;
public:
	Xode();
	~Xode();
	bool LoadFile(std::string fileName);
	virtual void PrintElement(std::string eleName="",int level=0);
	/*void CreatElement(TiXmlElement* pEle);*/
};
}