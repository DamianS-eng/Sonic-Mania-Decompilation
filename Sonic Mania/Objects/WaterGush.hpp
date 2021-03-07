#ifndef OBJ_WATERGUSH_H
#define OBJ_WATERGUSH_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectWaterGush : Object {

};

// Entity Class
struct EntityWaterGush : Entity {

};

// Object Struct
extern ObjectWaterGush *WaterGush;

// Standard Entity Events
void WaterGush_Update();
void WaterGush_LateUpdate();
void WaterGush_StaticUpdate();
void WaterGush_Draw();
void WaterGush_Create(void* data);
void WaterGush_StageLoad();
void WaterGush_EditorDraw();
void WaterGush_EditorLoad();
void WaterGush_Serialize();

// Extra Entity Functions


#endif //!OBJ_WATERGUSH_H
