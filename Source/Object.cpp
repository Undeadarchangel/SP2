#include "Object.h"

Object::Object()
{
	meshType = nullptr;
	parent = nullptr;
	pos.SetZero();
	translation.SetZero();
	angle.SetZero();
	scale.Set(1, 1, 1);
	render = true;
	clockwise = false;
	parentRotation = false;
	parentScale = false;
	interactable = false;
}

Object::~Object()
{
}

void Object::setPos(float x, float y, float z)
{
	pos.Set(x, y, z);
}
void Object::setTranslation(float x, float y, float z)
{
	translation.Set(x, y, z);
	if (parent != nullptr)
	{
		pos = parent->getPos()+ translation;
	}
	else
		pos = translation;
}
void Object::setRotation(float angle, char axis)
{
	Mtx44 rotation;
	if (axis == 'x')
	{
		this->angle.x = angle;
		//rotation.SetToRotation(angle, 1, 0, 0);
	}
	else
		if (axis == 'y')
		{
			this->angle.y = angle;
			//rotation.SetToRotation(angle, 0, 1, 0);
		}
		else
			if (axis == 'z')
			{
				this->angle.z = angle;
				//rotation.SetToRotation(angle, 0, 0, 1);
			}
	//for (int i = 0; i < 8; ++i)
	//{
	//	rotation * vertex[i];
	//}
	//for (int i = 0; i < 3; ++i)
	//{
	//	rotation* normal[i];
	//}
}
void Object::addRotation(float angle, char axis)
{
	Mtx44 rotation;
	if (axis == 'x')
	{
		this->angle.x += angle;
		//rotation.SetToRotation(this->angle.x, 1, 0, 0);
	}
	else
		if (axis == 'y')
		{
			this->angle.y += angle;
			//rotation.SetToRotation(this->angle.y, 0, 1, 0);
		}
		else
			if (axis == 'z')
			{
				this->angle.z += angle;
				//rotation.SetToRotation(this->angle.z, 0, 0, 1);
			}
	//for (int i = 0; i < 8; ++i)
	//{
	//	rotation* vertex[i];
	//}
	//for (int i = 0; i < 3; ++i)
	//{
	//	rotation* normal[i];
	//}
}
void Object::setScale(float x, float y, float z)
{
	if (parent == nullptr)
	scale.Set(x, y, z);
	else
	scale.Set(x, y, z);
}
void Object::setScale(float scale)
{
	if (parent == nullptr)
	{
		this->scale.Set(scale, scale, scale);
	}
	else
	this->scale.Set(scale, scale, scale);
}
void Object::setParent(Object* parent)
{
	this->parent = parent;
}
void Object::setChild(Object* child)
{
	this->child.push_back(child);
}
void Object::setMesh(Mesh* mesh)
{
	meshType = mesh;
}

void Object::setMesh(Mesh* mesh, Object parent, Vector3 translation)
{
	meshType = mesh;
	this->parent = &parent;
	pos = parent.pos + translation;
	this->translation = translation;
}

void Object::setMesh(Mesh* mesh, Object* parent)
{
	meshType = mesh;
	this->parent = parent;
	pos = parent->getPos();
}

void Object::setMesh(Mesh* mesh, Vector3 translation)
{
	meshType = mesh;
	this->translation = translation;
}
void Object::setDimension(float x, float y, float z)
{
	//vertex[0] = pos - Vector3(-x / 2, y / 2, -z / 2);
	//vertex[1] = pos - Vector3(x / 2, y / 2, -z / 2);
	//vertex[2] = pos - Vector3(-x / 2, y / 2, z / 2);
	//vertex[3] = pos - Vector3(x / 2, y / 2, z / 2);

	//vertex[4] = pos - Vector3(-x / 2, -y / 2, -z / 2);
	//vertex[5] = pos - Vector3(x / 2, -y / 2, -z / 2);
	//vertex[6] = pos - Vector3(-x / 2, -y / 2, z / 2);
	//vertex[7] = pos - Vector3(x / 2, -y / 2, z / 2);

	//normal[0] = Vector3(pos.x + x, pos.y, pos.z).Normalize();
	//normal[1] = Vector3(pos.x, pos.y + y, pos.z).Normalize();
	//normal[2] = Vector3(pos.x, pos.y, pos.z + z).Normalize();
	dimension = Vector3(x, y, z);
}
void Object::setIsClockwise(bool boolean)
{
	clockwise = boolean;
}

void Object::setRender(bool render)//set if is object rendered in the scene
{
	this->render = render;
}
bool Object::isClockwise()
{
	return clockwise;
}
bool Object::followParentRotation()
{
	return parentRotation;
}
bool Object::followParentScale()
{
	return parentScale;

}
bool Object::isInteractable()
{
	return interactable;
}
Vector3 Object::getScale()
{
	return scale;
}
Object* Object::getParent()
{
	return parent;
}
std::vector<Object*> Object::getChild()
{
	return child;
}
Vector3 Object::getPos()
{
	return pos;
}
Vector3 Object::getTranslation()
{
	return translation;
}
Vector3 Object::getAngle()
{
	return angle;
}
Mesh* Object::getMesh()
{
	return meshType;
}
Vector3 Object::getDimension()
{
	return dimension;
}
bool Object::isRender()
{
	return render;
}
void Object::setInteractable(bool canInteract)
{
	interactable = canInteract;
}
void Object::updateCollision(Object* b)
{
	Vector3 T = pos - b->pos;
	float Wa, Ha, Da;// half dimensions of A (Width, Height, Depth)
	float Wb, Hb, Db;// half dimensions of B (Width, Height, Depth)
	Vector3 Ax, Ay, Az;// unit vector of the axes of A
	Vector3 Bx, By, Bz;// unit vector of the axes of B
	Mtx44 rAx,rAy,rAz,rBx,rBy,rBz;
	if (angle != Vector3(0, 0, 0))
	{
		rAx.SetToRotation(angle.x, 1, 0, 0);
		rAy.SetToRotation(angle.y, 0, 1, 0);
		rAz.SetToRotation(angle.z, 0, 0, 1);
	}
	if (b->angle != Vector3(0, 0, 0))
	{
		rBx.SetToRotation(b->angle.x, 1, 0, 0);
		rBy.SetToRotation(b->angle.y, 0, 1, 0);
		rBz.SetToRotation(b->angle.z, 0, 0, 1);
	}
	Wa = dimension.x / 2;
	Ha = dimension.y / 2;
	Da = dimension.z / 2;

	Wb = b->dimension.x / 2;
	Hb = b->dimension.y / 2;
	Db = b->dimension.z / 2;

	Ax = Vector3(1, 0, 0);
	Ay = Vector3(0, 1, 0);
	Az = Vector3(0, 0, 1);

	Bx = Vector3(1, 0, 0);
	By = Vector3(0, 1, 0);
	Bz = Vector3(0, 0, 1);
	if (angle != Vector3(0, 0, 0))
	{
		Ax = rAx * rAy * rAz * Ax;
		Ay = rAx * rAy * rAz * Ay;
		Az = rAx * rAy * rAz * Az;
	}
	if (b->angle != Vector3(0, 0, 0))
	{
		Bx = rBx * rBy * rBz * Bx;
		By = rBx * rBy * rBz * By;
		Bz = rBx * rBy * rBz * Bz;
	}

	//Checking by A

	float LHS = projPlane(T,Ax).Length(); //Projection of T onto plane with normal Ax
	float RHS = projPlane(Ax * Wa, Ax).Length() + projPlane(Ay * Ha, Ax).Length() + projPlane(Az * Da, Ax).Length() + 
				projPlane(Bx * Wb, Ax).Length() + projPlane(By * Hb, Ax).Length() + projPlane(Bz * Db, Ax).Length();
	if (LHS <= RHS)//Collision
	{
		//std::cout << "AX COLLISION"<<std::endl;
	}
	else
	{
		return;
	}

	 LHS = projPlane(T, Ay).Length(); //Projection of T onto plane with normal Ay
	 RHS = projPlane(Ax * Wa, Ay).Length() + projPlane(Ay * Ha, Ay).Length() + projPlane(Az * Da, Ay).Length() +
		projPlane(Bx * Wb, Ay).Length() + projPlane(By * Hb, Ay).Length() + projPlane(Bz * Db, Ay).Length();
	if (LHS <= RHS)//Collision
	{
		//std::cout << "AY COLLISION" << std::endl;
	}
	else
	{
		return;
	}

	 LHS = projPlane(T, Az).Length(); //Projection of T onto plane with normal Az
	 RHS = projPlane(Ax * Wa, Ay).Length() + projPlane(Ay * Ha, Ay).Length() + projPlane(Az * Da, Ay).Length() +
		projPlane(Bx * Wb, Ay).Length() + projPlane(By * Hb, Ay).Length() + projPlane(Bz * Db, Ay).Length();
	if (LHS <= RHS)//Collision
	{
		//std::cout << "AZ COLLISION" << std::endl;
	}
	else
	{
		return;
	}

	//Checking by B

	 LHS = projPlane(T, Bx).Length(); //Projection of T onto plane with normal Ax
	 RHS = projPlane(Ax * Wa, Bx).Length() + projPlane(Ay * Ha, Bx).Length() + projPlane(Az * Da, Bx).Length() +
		projPlane(Bx * Wb, Bx).Length() + projPlane(By * Hb, Bx).Length() + projPlane(Bz * Db, Bx).Length();
	if (LHS <= RHS)//Collision
	{
		//std::cout << "BX COLLISION" << std::endl;
	}
	else
	{
		return;
	}

	 LHS = projPlane(T, By).Length(); //Projection of T onto plane with normal Ax
	 RHS = projPlane(Ax * Wa, By).Length() + projPlane(By * Ha, By).Length() + projPlane(Az * Da, By).Length() +
		projPlane(Bx * Wb, By).Length() + projPlane(By * Hb, By).Length() + projPlane(Bz * Db, By).Length();
	if (LHS <= RHS)//Collision
	{
		//std::cout << "BY COLLISION" << std::endl;
	}
	else
	{
		return;
	}

	 LHS = projPlane(T, Bz).Length(); //Projection of T onto plane with normal Ax
	 RHS = projPlane(Ax * Wa, Ay).Length() + projPlane(Ay * Ha, Ay).Length() + projPlane(Bz * Da, Ay).Length() +
		projPlane(Bx * Wb, Ay).Length() + projPlane(By * Hb, Ay).Length() + projPlane(Bz * Db, Ay).Length();
	if (LHS <= RHS)//Collision
	{
		//std::cout << "BZ COLLISION" << std::endl;
	}
	else
	{
		return;
	}
	//Edges
	Vector3 L = Ax.Cross(Bx); //Normal of separating plane
	LHS = projPlane(T, L).Length();
	RHS = projPlane(Wa * Ax, L).Length() + projPlane(Ha * Ay, L).Length() + projPlane(Da * Az, L).Length() +
		projPlane(Wb * Bx, L).Length() + projPlane(Hb * By, L).Length() + projPlane(Db * Bz, L).Length();
	if (LHS <= RHS)//Collision
	{
	}
	else
	{
		return;
	}

	L = Ax.Cross(By);
	LHS = projPlane(T, L).Length();
	RHS = projPlane(Wa * Ax, L).Length() + projPlane(Ha * Ay, L).Length() + projPlane(Da * Az, L).Length() +
		projPlane(Wb * Bx, L).Length() + projPlane(Hb * By, L).Length() + projPlane(Db * Bz, L).Length();
	if (LHS <= RHS)//Collision
	{
	}
	else
	{
		return;
	}

	L = Ax.Cross(Bz);
	LHS = projPlane(T, L).Length();
	RHS = projPlane(Wa * Ax, L).Length() + projPlane(Ha * Ay, L).Length() + projPlane(Da * Az, L).Length() +
		projPlane(Wb * Bx, L).Length() + projPlane(Hb * By, L).Length() + projPlane(Db * Bz, L).Length();
	if (LHS <= RHS)//Collision
	{
	}
	else
	{
		return;
	}

	L = Ay.Cross(Bx); //Normal of separating plane
	LHS = projPlane(T, L).Length();
	RHS = projPlane(Wa * Ax, L).Length() + projPlane(Ha * Ay, L).Length() + projPlane(Da * Az, L).Length() +
		projPlane(Wb * Bx, L).Length() + projPlane(Hb * By, L).Length() + projPlane(Db * Bz, L).Length();
	if (LHS <= RHS)//Collision
	{
	}
	else
	{
		return;
	}

	L = Ay.Cross(By);
	LHS = projPlane(T, L).Length();
	RHS = projPlane(Wa * Ax, L).Length() + projPlane(Ha * Ay, L).Length() + projPlane(Da * Az, L).Length() +
		projPlane(Wb * Bx, L).Length() + projPlane(Hb * By, L).Length() + projPlane(Db * Bz, L).Length();
	if (LHS <= RHS)//Collision
	{
	}
	else
	{
		return;
	}

	L = Ay.Cross(Bz);
	LHS = projPlane(T, L).Length();
	RHS = projPlane(Wa * Ax, L).Length() + projPlane(Ha * Ay, L).Length() + projPlane(Da * Az, L).Length() +
		projPlane(Wb * Bx, L).Length() + projPlane(Hb * By, L).Length() + projPlane(Db * Bz, L).Length();
	if (LHS <= RHS)//Collision
	{
	}
	else
	{
		return;
	}

	L = Az.Cross(Bx); //Normal of separating plane
	LHS = projPlane(T, L).Length();
	RHS = projPlane(Wa * Ax, L).Length() + projPlane(Ha * Ay, L).Length() + projPlane(Da * Az, L).Length() +
		projPlane(Wb * Bx, L).Length() + projPlane(Hb * By, L).Length() + projPlane(Db * Bz, L).Length();
	if (LHS <= RHS)//Collision
	{
	}
	else
	{
		return;
	}

	L = Az.Cross(By);
	LHS = projPlane(T, L).Length();
	RHS = projPlane(Wa * Ax, L).Length() + projPlane(Ha * Ay, L).Length() + projPlane(Da * Az, L).Length() +
		projPlane(Wb * Bx, L).Length() + projPlane(Hb * By, L).Length() + projPlane(Db * Bz, L).Length();
	if (LHS <= RHS)//Collision
	{
	}
	else
	{
		return;
	}

	L = Az.Cross(Bz);
	LHS = projPlane(T, L).Length();
	RHS = projPlane(Wa * Ax, L).Length() + projPlane(Ha * Ay, L).Length() + projPlane(Da * Az, L).Length() +
		projPlane(Wb * Bx, L).Length() + projPlane(Hb * By, L).Length() + projPlane(Db * Bz, L).Length();
	if (LHS <= RHS)//Collision
	{
	}
	else
	{
		return;
	}
	std::cout << "Collision\nCollision\nCollision\nCollision\nCollision\nCollision\nCollision\nCollision\nCollision\nCollision\nCollision\nCollision\nCollision\nCollision\nCollision\nCollision\nCollision\nCollision\nCollision\nCollision\nCollision\nCollision\nCollision\n";
}

Vector3 Object::projPlane(Vector3 vector, Vector3 planeNormal)
{
	return vector - vector.Dot(planeNormal) * (planeNormal);
}
void Object::unbindChild(Object* child)
{
	for (int i = 0; i < this->child.size(); ++i)//scans through all parent's children
	{
		if (this->child[i] == child)
		{
			this->child.erase(this->child.begin() + i);//unbinds child
			break;
		}
	}
}
void Object::bind(Object* parent, Object* child,bool followParentRotation,bool followParentScale)
{
	parent->setChild(child);
	child->setParent(parent);
	child->parentRotation = followParentRotation;
	child->parentScale = followParentRotation;
}

void Object::unbind(Object* child)
{
	if (child->getParent() != nullptr)
	{
	child->getParent()->unbindChild(child);
	child->setParent(nullptr);
	}
}

float Object::checkDist(Vector3 playerpos)
{
	float x = pow(pos.x - playerpos.x, 2.0);
	float z = pow(pos.z - playerpos.z, 2.0);
	return sqrt(x + z);
}
