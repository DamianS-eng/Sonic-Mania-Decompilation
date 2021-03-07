#ifndef OBJ_ERZGUNNER_H
#define OBJ_ERZGUNNER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectERZGunner : Object {
    int value1[32]; //= { -22, -24, -10, -24, -22, -31, -10, -31, -22, -38, -10, -38, -22, -45, -10, -45, 10, -24, 22, -24, 10, -31, 22, -31, 10, -38, 22, -38, 10, -45, 22, -45 };
    int value2;
    colour value3;
    colour value4;
    colour value5;
    ushort value6;
};

// Entity Class
struct EntityERZGunner : Entity {

};

// Object Struct
extern ObjectERZGunner *ERZGunner;

// Standard Entity Events
void ERZGunner_Update();
void ERZGunner_LateUpdate();
void ERZGunner_StaticUpdate();
void ERZGunner_Draw();
void ERZGunner_Create(void* data);
void ERZGunner_StageLoad();
void ERZGunner_EditorDraw();
void ERZGunner_EditorLoad();
void ERZGunner_Serialize();

// Extra Entity Functions


#endif //!OBJ_ERZGUNNER_H
