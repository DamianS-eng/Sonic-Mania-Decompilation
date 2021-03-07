#ifndef OBJ_TMZBARRIER_H
#define OBJ_TMZBARRIER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectTMZBarrier : Object {

};

// Entity Class
struct EntityTMZBarrier : Entity {

};

// Object Struct
extern ObjectTMZBarrier *TMZBarrier;

// Standard Entity Events
void TMZBarrier_Update();
void TMZBarrier_LateUpdate();
void TMZBarrier_StaticUpdate();
void TMZBarrier_Draw();
void TMZBarrier_Create(void* data);
void TMZBarrier_StageLoad();
void TMZBarrier_EditorDraw();
void TMZBarrier_EditorLoad();
void TMZBarrier_Serialize();

// Extra Entity Functions


#endif //!OBJ_TMZBARRIER_H
