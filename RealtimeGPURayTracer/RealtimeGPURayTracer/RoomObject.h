#ifndef _ROOM_OBJECT_H_
#define _ROOM_OBJECT_H_

#include "SceneObject.h"
#include <tiny_obj_loader.h>

class CRoomObject :
	public CSceneObject
{
protected:
	tinyobj::material_t mat;
public:
	CRoomObject(void);
	~CRoomObject(void);

	virtual void Render();

	tinyobj::material_t &GetMaterial() { return mat; }
};


#endif

