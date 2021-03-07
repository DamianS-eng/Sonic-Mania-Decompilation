#ifndef OBJ_CHEMBUBBLE_H
#define OBJ_CHEMBUBBLE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectChemBubble : Object {

};

// Entity Class
struct EntityChemBubble : Entity {

};

// Object Struct
extern ObjectChemBubble *ChemBubble;

// Standard Entity Events
void ChemBubble_Update();
void ChemBubble_LateUpdate();
void ChemBubble_StaticUpdate();
void ChemBubble_Draw();
void ChemBubble_Create(void* data);
void ChemBubble_StageLoad();
void ChemBubble_EditorDraw();
void ChemBubble_EditorLoad();
void ChemBubble_Serialize();

// Extra Entity Functions


#endif //!OBJ_CHEMBUBBLE_H
