#include "SonicMania.h"

ObjectEggPrison *EggPrison;

void EggPrison_Update(void)
{
    RSDK_THIS(EggPrison);
    StateMachine_Run(entity->state);

    if (entity->type == 1) {
        RSDK.ProcessAnimation(&entity->data2);
        EggPrison_HandleTileCollisions();
        if (!entity->activated) {
            foreach_active(Player, player)
            {
                if (Player_CheckCollisionBox(player, entity, &entity->hitbox1) == 1) {
                    player->position.x += entity->velocity.x;
                    player->position.y += 0x10000;
                }

                if (entity->state == EggPrison_Unknown2) {
                    if (Player_CheckCollisionBox(player, entity, &entity->hitbox2) == 4) {
                        entity->velocity.x = 0;
                        entity->active     = ACTIVE_NORMAL;
                        entity->state      = EggPrison_Activated;
                        entity->offsetY    = -0x80000;
                    }
                    else {
                        if (!Player_CheckCollisionTouch(player, entity, &entity->hitbox3)) {
                            if (entity->offsetY < 0)
                                entity->offsetY += 0x10000;
                        }
                        else {
                            Hitbox *playerHitbox = Player_GetHitbox(player);
                            entity->offsetY      = ((playerHitbox->top - 48) << 16) - entity->position.y + player->position.y;
                            if (entity->offsetY >= -0x80000) {
                                if (entity->offsetY > 0)
                                    entity->offsetY = 0;
                                entity->offsetY &= 0xFFFF0000;
                            }
                            else {
                                entity->offsetY = -0x80000;
                                entity->offsetY &= 0xFFFF0000;
                            }
                        }
                    }
                }
                else {
                    Player_CheckCollisionBox(player, entity, &entity->hitbox2);
                }
            }
        }
    }
    else if (!entity->activated) {
        foreach_active(Player, player)
        {
            Player_CheckCollisionBox(player, entity, &entity->hitbox1);
            if (entity->state == EggPrison_Unknown2) {
                if (Player_CheckCollisionBox(player, entity, &entity->hitbox2) == 1) {
                    entity->offsetY = 0x80000;
                    if (entity->type < 2)
                        RSDK_sceneInfo->timeEnabled = false;
                    if (entity->type == 5) {
                        int anim = player->playerAnimator.animationID;
                        if (anim == ANI_WALK || (anim > ANI_AIRWALK && anim <= ANI_DASH))
                            player->storedAnim = player->playerAnimator.animationID;
                        else
                            player->storedAnim = ANI_WALK;
                        player->state      = Player_State_Air;
                        player->onGround   = false;
                        player->velocity.y = -0xA0000;
                        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_SPRINGTWIRL, &player->playerAnimator, true, 0);
                        RSDK.PlaySFX(EggPrison->sfxSpring, 0, 255);
                    }
                    else {
                        entity->active = ACTIVE_NORMAL;
                        entity->state  = EggPrison_Activated;
                    }
                }
                else {
                    if (!Player_CheckCollisionTouch(player, entity, &entity->hitbox3)) {
                        if (entity->offsetY > 0)
                            entity->offsetY -= 0x10000;
                    }
                    else {
                        Hitbox *playerHitbox = Player_GetHitbox(player);
                        entity->offsetY      = ((playerHitbox->bottom + 48) << 16) - entity->position.y + player->position.y;
                        if (entity->offsetY <= 0x80000) {
                            if (entity->offsetY < 0)
                                entity->offsetY = 0;
                        }
                        else {
                            entity->offsetY = 0x80000;
                        }
                        entity->offsetY &= 0xFFFF0000;
                    }
                }
            }
            else {
                Player_CheckCollisionBox(player, entity, &entity->hitbox2);
            }
        }
    }
}

void EggPrison_LateUpdate(void) {}

void EggPrison_StaticUpdate(void) {}

void EggPrison_Draw(void)
{
    RSDK_THIS(EggPrison);
    Vector2 drawPos;
    if (entity->type == 1) {
        entity->direction = FLIP_Y;
        drawPos.x         = entity->position.x;
        drawPos.y         = entity->position.y;
        drawPos.y += entity->offsetY;
        RSDK.DrawSprite(&entity->data4, &drawPos, 0);
        entity->direction = FLIP_NONE;
        RSDK.DrawSprite(&entity->data1, NULL, false);
        RSDK.DrawSprite(&entity->data3, NULL, false);
        RSDK.DrawSprite(&entity->data2, NULL, false);
        entity->direction = FLIP_X;
        RSDK.DrawSprite(&entity->data2, NULL, false);
        entity->direction = FLIP_NONE;
    }
    else {
        drawPos.x = entity->position.x;
        drawPos.y = entity->position.y;
        drawPos.y += entity->offsetY;
        RSDK.DrawSprite(&entity->data4, &drawPos, false);
        RSDK.DrawSprite(&entity->data1, NULL, false);
        RSDK.DrawSprite(&entity->data3, NULL, false);
    }
}

void EggPrison_Create(void *data)
{
    RSDK_THIS(EggPrison);
    if (globals->gameMode < MODE_TIMEATTACK || entity->type >= 1) {
        entity->drawFX = FX_FLIP;
        if (!RSDK_sceneInfo->inEditor) {
            if (data)
                entity->type = voidToInt(data);
            if (entity->type == 1) {
                RSDK.SetSpriteAnimation(EggPrison->spriteIndex, 0, &entity->data1, true, 1);
                RSDK.SetSpriteAnimation(EggPrison->spriteIndex, 1, &entity->data4, true, 0);
                RSDK.SetSpriteAnimation(EggPrison->spriteIndex, 2, &entity->data3, true, 1);
                RSDK.SetSpriteAnimation(EggPrison->spriteIndex, 3, &entity->data2, true, 0);
                entity->hitbox2.left   = -16;
                entity->hitbox2.top    = 24;
                entity->hitbox2.right  = 16;
                entity->hitbox2.bottom = 38;

                entity->hitbox3.left   = -15;
                entity->hitbox3.top    = 24;
                entity->hitbox3.right  = 15;
                entity->hitbox3.bottom = 48;
                entity->velocity.x     = 0x10000;
                entity->velocity.y     = 0x4000;
            }
            else {
                RSDK.SetSpriteAnimation(EggPrison->spriteIndex, 0, &entity->data1, true, 0);
                RSDK.SetSpriteAnimation(EggPrison->spriteIndex, 1, &entity->data4, true, 0);
                RSDK.SetSpriteAnimation(EggPrison->spriteIndex, 2, &entity->data3, true, 0);
                entity->hitbox2.left   = -16;
                entity->hitbox2.top    = -37;
                entity->hitbox2.right  = 16;
                entity->hitbox2.bottom = -23;

                entity->hitbox3.left  = -15;
                entity->hitbox3.top   = -47;
                entity->hitbox3.right = -15;
                entity->hitbox3.top   = -23;
            }
            entity->hitbox1.left   = -32;
            entity->hitbox1.top    = -23;
            entity->hitbox1.right  = 32;
            entity->hitbox1.bottom = 32;
            entity->state          = EggPrison_Unknown1;
            entity->active         = ACTIVE_BOUNDS;
            entity->updateRange.x  = 0x800000;
            entity->updateRange.y  = 0x800000;
            entity->visible        = true;
            entity->drawOrder      = Zone->drawOrderLow;
        }
    }
    else {
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL);
    }
}

void EggPrison_StageLoad(void)
{
    EggPrison->spriteIndex = RSDK.LoadSpriteAnimation("Global/EggPrison.bin", SCOPE_STAGE);
    EggPrison->sfxDestroy  = RSDK.GetSFX("Global/Destroy.wav");
    EggPrison->sfxSpring   = RSDK.GetSFX("Global/Spring.wav");
}

void EggPrison_HandleTileCollisions(void)
{
    RSDK_THIS(EggPrison);

    bool32 flag = false;
    if (entity->field_70) {
        if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, 0, 0, -0x300000, 0x900000, false)
            || RSDK.ObjectTileCollision(entity, Zone->fgLayers, 0, 0, 0x300000, 0x900000, false)) {
            entity->angleY -= entity->velocity.y;
            flag = true;
        }
    }

    if (!flag) {
        if (entity->angleY < (RSDK_screens->position.y + 64) << 16)
            entity->angleY += entity->velocity.y;
    }

    if (entity->state != EggPrison_Unknown5) {
        if (entity->velocity.x > 0) {
            if (entity->position.x <= (RSDK_screens->position.x + RSDK_screens->width - 48) << 16) {
                if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_LWALL, 0, 0x400000, 0, true))
                    entity->velocity.x = -entity->velocity.x;
            }
            else {
                entity->velocity.x = -entity->velocity.x;
            }
        }
        else if (entity->position.x < (RSDK_screens->position.x + 48) << 16) {
            entity->velocity.x = -entity->velocity.x;
        }
        else {
            if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_RWALL, 0, -0x400000, 0, true))
                entity->velocity.x = -entity->velocity.x;
        }
    }
    entity->position.x += entity->velocity.x;
    entity->position.y = (RSDK.Sin256(entity->angle) << 10) + entity->angleY;
    entity->angle += 4;
    entity->angle &= 0xFF;
}

void EggPrison_Activated(void)
{
    RSDK_THIS(EggPrison);
    RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->data3, true, 0);
    entity->state = EggPrison_Unknown3;

    switch (entity->type) {
        default:
        case 0:
        case 1:
        case 2:
            for (int i = 0; i < 10; ++i) {
                EntityAnimals *animals =
                    (EntityAnimals *)RSDK.CreateEntity(Animals->objectID, intToVoid(Animals->animalTypes[i & 1] + 1),
                                                       (((RSDK.Rand(0, 48) & -4) - 24) << 16) + entity->position.x, entity->position.y + 0x40000);
                animals->delay          = 4 * i;
                animals->state          = Animals_State_BounceAround;
                animals->behaviour      = 1;
                animals->direction      = (i ^ (i >> 1)) & 1;
            }
            break;
        case 3: {
            int angle = 144;
            for (int r = 0; r < 6; ++r) {
                EntityRing *ring = (EntityRing *)RSDK.CreateEntity(
                    Ring->objectID, entity, (((RSDK.Rand(0, 48) & 0xFFFFFFFC) - 24) << 16) + entity->position.x, entity->position.y);
                ring->velocity.x              = RSDK.Cos256(angle) << 9;
                ring->velocity.y              = RSDK.Sin256(angle) << 9;
                ring->animator.animationSpeed = 512;
                ring->collisionPlane          = 0;
                ring->inkEffect               = INK_ALPHA;
                if (angle != 176)
                    angle += 8;
                else
                    angle = 208;
                ring->alpha     = 0x100;
                ring->state     = Ring_State_Bounce;
                ring->stateDraw = Ring_StateDraw_Normal;
            }
            break;
        }
        case 4: {
            EntityTechnosqueek *technoSqueak = CREATE_ENTITY(Technosqueek, NULL, entity->position.x - 0x80000, entity->position.y);
            technoSqueak->velocity.x = -0x30000;
            technoSqueak->velocity.y = -0x40000;
            technoSqueak->active     = ACTIVE_NORMAL;
            // technoSqueak[1].position.x = (int)TechnoSqueak_Unknown7;

            technoSqueak             = CREATE_ENTITY(Technosqueek, NULL, entity->position.x + 0x80000, entity->position.y);
            technoSqueak->velocity.x = 0x30000;
            technoSqueak->velocity.y = -0x40000;
            technoSqueak->active     = ACTIVE_NORMAL;
            technoSqueak->direction  = 1;
            // technoSqueak[1].position.x = (int)TechnoSqueak_Unknown7;

            EntityBlaster *blaster = CREATE_ENTITY(Blaster, NULL, entity->position.x - 0x180000, entity->position.y);
            blaster->velocity.x = -0x30000;
            blaster->velocity.y = -0x30000;
            blaster->active     = ACTIVE_NORMAL;
            blaster->state         = Blaster_State_Unknown7;

            blaster             = CREATE_ENTITY(Blaster, NULL, entity->position.x + 0x180000, entity->position.y);
            blaster->velocity.x = 0x30000;
            blaster->velocity.y = -0x30000;
            blaster->active     = ACTIVE_NORMAL;
            blaster->direction  = 1;
            blaster->state      = Blaster_State_Unknown7;
            break;
        }
        case 5:
            for (int a = 0; a < 10; ++a) {
                EntityAnimals *animals = (EntityAnimals *)RSDK.CreateEntity(Animals->objectID, intToVoid(Animals->animalTypes[a & 1] + 1),
                                                                            (((RSDK.Rand(0, 48) & 0xFFFFFFFC) - 24) << 16) + entity->position.x,
                                                                            entity->position.y + 0x40000);
                animals->delay         = 4 * a;
                animals->state         = Animals_State_BounceAround;
                animals->behaviour     = 1;
                animals->direction     = (a ^ (a >> 1)) & 1;
            }
            break;
    }

    EntityDebris *debris =
        (EntityDebris *)RSDK.CreateEntity(Debris->objectID, Debris_State_Unknkown, entity->position.x - 0x160000, entity->position.y);
    RSDK.SetSpriteAnimation(EggPrison->spriteIndex, 2, &debris->animator, true, 2);
    debris->velocity.x    = -0x20000;
    debris->velocity.y    = -0x20000;
    debris->gravity       = 0x3800;
    debris->rotSpeed      = -4;
    debris->drawFX        = FX_ROTATE;
    debris->drawOrder     = Zone->drawOrderHigh;
    debris->updateRange.x = 0x800000;
    debris->updateRange.y = 0x800000;

    debris = (EntityDebris *)RSDK.CreateEntity(Debris->objectID, Debris_State_Unknkown, entity->position.x, entity->position.y);
    RSDK.SetSpriteAnimation(EggPrison->spriteIndex, 2, &debris->animator, true, 3);
    debris->velocity.x    = 0;
    debris->velocity.y    = -0x30000;
    debris->gravity       = 0x3800;
    debris->rotSpeed      = 8;
    debris->drawFX        = FX_ROTATE;
    debris->drawOrder     = Zone->drawOrderHigh;
    debris->updateRange.x = 0x800000;
    debris->updateRange.y = 0x800000;

    debris = (EntityDebris *)RSDK.CreateEntity(Debris->objectID, Debris_State_Unknkown, entity->position.x + 0x160000, entity->position.y);
    RSDK.SetSpriteAnimation(EggPrison->spriteIndex, 2, &debris->animator, true, 4);
    debris->velocity.x    = 0x20000;
    debris->velocity.y    = -0x20000;
    debris->gravity       = 0x3800;
    debris->rotSpeed      = 4;
    debris->drawFX        = FX_ROTATE;
    debris->drawOrder     = Zone->drawOrderHigh;
    debris->updateRange.x = 0x800000;
    debris->updateRange.y = 0x800000;
}

void EggPrison_Unknown1(void)
{
    RSDK_THIS(EggPrison);
    entity->angleY = entity->position.y;
    entity->state  = EggPrison_Unknown2;
}

void EggPrison_Unknown2(void)
{
    RSDK_THIS(EggPrison);
    for (int p = 0; p < Player->playerCount && entity->type < 2; ++p) {
        EntityPlayer *player = RSDK_GET_ENTITY(p, Player);
        if (!player->sidekick) {
            if (abs(entity->position.x - player->position.x) < 0x1000000) {
                if (abs(entity->position.y - player->position.y) < 0x1000000 && entity->position.x - (Zone->screenBoundsR1[p] << 16) < 0x1000000) {
                    Zone->playerBoundActiveL[p] = true;
                    Zone->playerBoundActiveR[p] = true;
                    if (!entity->type) {
                        Zone->screenBoundsL1[p] = (entity->position.x >> 0x10) - RSDK_screens[p].centerX;
                        Zone->screenBoundsR1[p] = (entity->position.x >> 0x10) + RSDK_screens[p].centerX;
                    }
                }
            }
        }
    }
}

void EggPrison_Unknown3(void)
{
    RSDK_THIS(EggPrison);
    if (!(entity->timer % 3)) {
        Entity *explosion    = RSDK.CreateEntity(Explosion->objectID, intToVoid(2 * (RSDK.Rand(0, 256) > 192) + 1),
                                              (RSDK.Rand(-24, 24) << 16) + entity->position.x, (RSDK.Rand(-24, 24) << 16) + entity->position.y);
        explosion->drawOrder = Zone->drawOrderHigh;
        RSDK.PlaySFX(EggPrison->sfxDestroy, 0, 255);
    }
    if (++entity->timer == 20) {
        entity->timer = 0;
        if (entity->type < 2) {
            entity->state = EggPrison_Unknown4;
            Music_FadeOut(0.025);
        }
        else {
            entity->state = StateMachine_None;
        }
    }
}

void EggPrison_Unknown4(void)
{
    RSDK_THIS(EggPrison);
    if (++entity->timer == 60) {
        entity->timer                 = 0;
        entity->state                 = StateMachine_None;
        Zone->forcePlayerOnScreenFlag = false;
        Music_PlayTrack(TRACK_ACTCLEAR);
        RSDK.ResetEntitySlot(SLOT_ACTCLEAR, ActClear->objectID, 0);
    }
}

void EggPrison_Unknown5(void)
{
    RSDK_THIS(EggPrison);
    if (entity->velocity.x > -0x30000)
        entity->velocity.x -= 0x1000;

    if (!RSDK.CheckOnScreen(entity, NULL))
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, 0);
}

void EggPrison_EditorDraw(void) {}

void EggPrison_EditorLoad(void) {}

void EggPrison_Serialize(void) { RSDK_EDITABLE_VAR(EggPrison, VAR_ENUM, type); }
