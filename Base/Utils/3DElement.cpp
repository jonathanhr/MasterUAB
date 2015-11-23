#include "3DElement.h"

C3DElement::C3DElement(){}
C3DElement::C3DElement(const Vect3f &Position) : m_Position(Position){}
C3DElement::C3DElement(const Vect3f &Position, float Yaw, float Pitch, float Roll) : m_Position(Position), m_Yaw(Yaw), m_Pitch(Pitch), m_Roll(Roll){}
C3DElement::C3DElement(float Yaw, float Pitch, float Roll) : m_Yaw(Yaw), m_Pitch(Pitch), m_Roll(Roll){}
C3DElement::C3DElement(const CXMLTreeNode &XMLTreeNode){
	// pos, yaw, pitch, roll, scale

	m_Position = XMLTreeNode.GetVect3fProperty("pos",Vect3f(0,0,0),false);
	m_Yaw = XMLTreeNode.GetFloatProperty("yaw");
	m_Pitch = XMLTreeNode.GetFloatProperty("pitch");
	m_Roll = XMLTreeNode.GetFloatProperty("roll");
	m_Scale = XMLTreeNode.GetVect3fProperty("scale",Vect3f(0,0,0),false);
}

void C3DElement::SetPosition(const Vect3f &Position){
	m_PrevPos = m_Position;
	m_Position = Position;
}

inline const Vect3f & C3DElement::GetPosition() const{
	return m_Position;
}

inline const Vect3f & C3DElement::GetPrevPosition() const{
	return m_PrevPos;
}

float C3DElement::GetYaw() const{
	return m_Yaw;
}

float C3DElement::GetPitch() const{
	return m_Pitch;
}

float C3DElement::GetRoll() const{
	return m_Roll;
}

void C3DElement::SetYaw(float Yaw){
	m_Yaw = Yaw;
}

void C3DElement::SetPitch(float Pitch){
	m_Pitch = Pitch;
}

void C3DElement::SetRoll(float Roll){
	m_Roll = Roll;
}

void C3DElement::SetYawPitchRoll(float Yaw, float Pitch, float Roll){
	m_Yaw = Yaw;
	m_Pitch = Pitch;
	m_Roll = Roll;
}

void C3DElement::SetScale(const Vect3f &Scale){
	m_Scale = Scale;
}

inline const Vect3f & C3DElement::GetScale() const{
	return m_Scale;
}

//void C3DElement::Render(CKGRenderManager *RenderManager){}

const Mat44f & C3DElement::GetTransform(){
	return m_TransformMatrix;
}