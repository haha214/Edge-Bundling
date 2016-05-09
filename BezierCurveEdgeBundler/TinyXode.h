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
/* ������  XodeElement                                                   */
/* ���ܣ�  ����Ԫ����Ļ���                                             */
/* ˵����  ֻ��һ��TiXmlElementָ�����ͳ�Ա������������һ���ڵ��ָ�� */
/* AttributeToVar    ��  ���غ���������xml����ȡ���ַ������ͱ���ת��Ϊ������������ */
/* PrintElement      ��  �麯����������ڵ��������Ϣ���                 */
/* PrintChildElement :   ��һ���ӽڵ�������������                      */
/* PrintElementName  ��  ����ǰ�ڵ���������                                */
/************************************************************************/

class XodeElement
{
protected:
	TiXmlElement* pEle;
public:
	XodeElement(TiXmlElement* pEle=NULL);
	~XodeElement();
	void AttributeToVar(const char* str,int &num);       //������ֵת��Ϊ����
	void AttributeToVar(const char* str,float &num);      //������ֵת��Ϊ������
	void AttributeToVar(const char* cha,std::string &str); //������ֵת��Ϊ�ַ���
	virtual void PrintElement(int level=0,std::string eleName="");
	void PrintChildElement(XodeElement* childEle,int level=0,std::string eleName="");
	void PrintElementName(int level=0,std::string eleName="");
};


/************************************************************************/
/* ������  Colour                                                   */
/* ���ܣ�  ����һ����ɫ��RGB��������ֵ��ͨ�����캯��������ֵ��ֵ    */     
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
/* ������  ElementWithExt                                                  */
/* ���ܣ�  ��Ϊ���о���Ext���Ա��������Ļ���                               */
/* ˵����  ����Extָ�����͵�����ָ�룬���캯��������ʼ����               */
/*          ������Լ̳���������Ͳ����ٳ�ʼ��ext */
/************************************************************************/

class ElementWithExt:public XodeElement
{
public:
	std::vector<Ext*> ext;                   //Extָ�����͵Ŀɱ䳤�����飬��ʾ������0��������Extָ��
public:
	ElementWithExt(TiXmlElement* pEle=NULL);
	~ElementWithExt();
	virtual void PrintElement(int level=0,std::string eleName="");
};


/************************************************************************/
/* ������  Position                                                      */
/* ���ࣺ  XodeElement                                                 */
/* ���ܣ�  ����һ��λ����Ϣ                                            */
/* ˵����  �ڹ��캯����ͨ���������ݵ�ָ���xyzֵ���и�ֵ               */
/************************************************************************/

class Position:public XodeElement
{
public:
	float x;        //λ�õ�x����
	float y;        //λ�õ�y����
	float z;        //λ�õ�z����
public:
	Position(TiXmlElement* pEle=NULL);   //���캯����������ͨ��pEleָ���xyz��ֵ
	virtual void PrintElement(int level=0,std::string eleName="");
};

/************************************************************************/
/* ������  Matrix4f                                                     */
/* ���ࣺ  XodeElement                                                 */
/* ���ܣ�  ����һ��4��4�б任����                                       */
/* ˵����  �ڹ��캯����ͨ���������ݵ�ָ��Ծ��������ݽ��и�ֵ           */
/************************************************************************/

class Matrix4f:public XodeElement
{
public:
	float m00;            //����ĵ�0��0������
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
	Matrix4f(TiXmlElement* pEle=NULL);   //���캯��
	 
};

/************************************************************************/
/* ������  Euler                                                     */
/* ���ࣺ  XodeElement                                                 */
/* ���ܣ�  ����һ���任����Ҫ��ŷ����                                 */
/* ˵����  �ڹ��캯����ͨ���������ݵ�ָ��Ծ��������ݽ��и�ֵ           */
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
/* ������  Rotation                                                     */
/* ���ࣺ  XodeElement                                                 */
/* ���ܣ�  ����һ���任����Ҫ����ת��Ϣ������ŷ���ǣ�ƫ��Ǻ�һ����Ԫ��   */
/* ˵����  �ڹ��캯����ͨ���������ݵ�ָ��Ծ��������ݽ��и�ֵ           */
/************************************************************************/
class Rotation:public XodeElement
{
public:
	Euler* euler;                      //ŷ����
	Axisangle* axisangle;               //ƫ���
	Quaternion* quaternion;           
public:
	Rotation(TiXmlElement* pEle=NULL);
	~Rotation();
	virtual void PrintElement(int level=0,std::string eleName="");
};

/************************************************************************/
/* ������  Transform                                                      */
/* ���ࣺ  ElementWithExt                                                 */
/* ���ܣ�  ����һ���任��Ϣ�������任����Ҫ�ĸ�������                      */
/* ˵����  �ڹ��캯����ͨ���������ݵ�ָ������г�Ա�������и�ֵ               */
/************************************************************************/
class Transform:public ElementWithExt
{
public:
	bool absolute;                       //��ʾ�Ƿ��ȡ��������
	std::string transformstyle;          //�任������
	float scale;                         //�任�ĳ߶�
	bool isMatrix;                       //��ʾ�Ƿ���þ����ʾ�任��trueʱʹ�þ���
	Position* position;                  //�任���λ�ã�����isMatrixΪfalseʱ��Ч
	Rotation* rotation;                  //�任ʱ����ת������isMatrixΪfalseʱ��Ч
	Matrix4f* matrix4f;                  //�任���󣬽���isMatrixΪtrueʱ��Ч
public:
	Transform(TiXmlElement* pEle=NULL);  //���캯��������pEleָ���ʼ����Ա����
	~Transform();
	virtual void PrintElement(int level=0,std::string eleName="");
};

/************************************************************************/
/* ������  SpaceBase                                                      */
/* ���ࣺ  ElementWithExt                                                 */
/* ˵����  ����space��һЩ��Ļ��࣬���Ǻ��������ͬ�ĳ�Ա����               */
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

//Բ������
class CappedCylinder:public ElementWithExt
{
public:
	float radius;    //����뾶
	float length;    //����
public:
	CappedCylinder(TiXmlElement* pEle=NULL);
	virtual void PrintElement(int level=0,std::string eleName="");
};

//׵���࣬��һ�������Բ���壬���Լ̳���CappedCylinder��
class Cone:public CappedCylinder
{
public:
	Cone(TiXmlElement* pEle=NULL);
};

//�Ƿ��Բ�����࣬����Բ����Ĳ�����ͬ���̳���CappedCylinder��
class Cylinder:public CappedCylinder
{
public:
	Cylinder(TiXmlElement* pEle=NULL);
};

//ƽ����
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

//������
class Ray:public ElementWithExt
{
public:
	float length;        //���߳���
public:
	Ray(TiXmlElement* pEle=NULL);
	virtual void PrintElement(int level=0,std::string eleName="");
};


//������
class Sphere:public ElementWithExt
{
public:
	float radius;        //�뾶��С
public:
	Sphere(TiXmlElement* pEle=NULL);
	virtual void PrintElement(int level=0,std::string eleName="");
};

//����
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

//��������
class T:public XodeElement
{
public:
	float ia;            //a�߳�
	float ib;
	float ic;
public:
	T(TiXmlElement* pEle=NULL);
	virtual void PrintElement(int level=0,std::string eleName="");
};

/************************************************************************/
/* ������  Vertices                                                                   */
/* ���ࣺ  XodeElement
/* ˵����  �㼯������0�������������ָ��
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
/* ������  Triangles                                                                  */
/* ���ࣺ  XodeElement
/* ˵����  �����μ�������0������������ζ����ָ��
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
/* ������  MassShape                                                      */
/* ���ࣺ  Adjust                                                 */
/* ���ܣ�  ��������״����ʾ���ʵ�                                            */
/* ˵����  shapeKind��ʾ�ʵ���״����ѡ��CYLINDER��SPHERE��BOX��CAPPENCYLINDER  */
/*         shapeΪ��Щ��״�ĸ���ָ�룬����shapeKind��ֵ��ʼ��Ϊ��Ӧ���������
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
/* ������  Mass                                                      */
/* ���ࣺ  XodeElement                                                 */
/* ���ܣ�  ����һ���ʵ�                                            */
/* ˵����  massKind��ʾ�ʵ�ı�ʾ��ʽ����ΪMASS_SHAPEʱ����״����ʾ          */
/*         massForm��ʼ��ΪMassShape��Ķ��󣬵�ΪMASS_STRUCTʱ��ʾ�ýṹ*/
/*         ����ʾ�ʵ㣬massForm��ʼ��ΪMassStruct��Ķ���;��ΪNO_INFOʱ��ʾ */
/*         massFormΪ��                                                  */
/************************************************************************/
class Mass:public XodeElement
{
public:
	int massKind;
	XodeElement* massForm;
	Transform* transform;           
	Adjust* adjust;
	Mass* mass;                     //���ʵ����ָ��
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
/* ������JointKindBase1                                                                     */
/* ���ࣺXodeElement
/* ˵��������Լ�����͵ĵ�һ�ֻ��࣬����axis����Fixed��Slider����������͵�Լ��
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
/* ������JointKindBase2                                                                     */
/* ���ࣺJointKindBase1
/* ˵��������Լ�����͵ĵڶ��ֻ��࣬�̳���JointKindBase1
/*       ����axis������anchor����Ball��Hinge����������͵�Լ��
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
/* ������ Xode 
/* ���ࣺ XodeElement
/* ˵���� ��xml�ĵ������нڵ��еĸ��ڵ㣬���캯�������޲���һ�֣�
/*        ��˲���ͨ�����캯�������ʼ��������ͨ��bool�ͺ���LoadFile
/*        ���г�ʼ���������Ǵ���һ��xml�ĵ��ľ��Ի����·����
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