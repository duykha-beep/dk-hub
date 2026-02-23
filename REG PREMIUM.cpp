#include <iostream>
JNIEXPORT jobjectArray  JNICALL
Java_uk_lgl_modmenu_FloatingModMenuService_getFeatureList(
        JNIEnv *env,
        jobject activityObject) {
    jobjectArray ret;

    const char *features[] = {
            "Toggle_AIMBOT AUTO", 
            "Toggle_AUTO HEADSHOT", 
            "Toggle_AIM INVISIBLE", // 2
            "Toggle_AIM WHEN FIRE", 
            "Toggle_AIM WHEN AIM",
            "Toggle_AIM WHEN CROUCH",
            "SeekBarSpot_HEADSHOT PERCENT_0_3",// 6
            "SeekBar_SENSTIVITY_0_5", // 8
            "Toggle_AIM + MOVEMENT", // 21

       case 2:
            MT.hs70 = !MT.hs70;
            break;

        case 3:
            MT.aimFire = !MT.aimFire;
           
            break;

        case 5:
            MT.aimAgachado = !MT.aimAgachado;
           
            break;
            
        case 6:
            if (Value == 0) {
                MT.hs100 = false;
                MT.hs70 = false;
                MT.aimHead = false;
            } else if (Value == 1) {
                MT.hs70 = false;
                MT.hs100 = false;
                MT.aimHeaf = !MT.aimHead;
                MT.aimHead = true;
            } else if (Value == 2) {
                MT.hs100 = false;
                MT.aimHead = false;
                MT.hs70 = !MT.hs70;
                MT.hs70 = true;
            } else if (Value == 2) {
                MT.hs70 = false;
                MT.aimHead = false;
                MT.hs100 = !MT.hs100;
                MT.hs100 = true;
            }
            break;
            case 8:
            if (Value == 0) {
                hexPatches.Sensi1.Restore();
                hexPatches.Sensi2.Restore();
                hexPatches.Sensi3.Restore();
                hexPatches.Sensi4.Restore();
                hexPatches.Sensi5.Restore();
            } else if (Value == 1) {
                hexPatches.Sensi1.Modify();
            } else if (Value == 2) {
                hexPatches.Sensi2.Modify();
            } else if (Value == 3) {
                hexPatches.Sensi3.Modify();
            } else if (Value == 4) {
                hexPatches.Sensi4.Modify();
            } else if (Value == 5) {
                hexPatches.Sensi5.Modify();
            }
            break;
     }
  }
}
Vector3 GetHeadPosition(void* player){
    return Transform_INTERNAL_GetPosition(*(void**) ((uint64_t) player + 0x1B0)); // atualizado 1.56
}

Vector3 GetHipPosition(void* player){
    return Transform_INTERNAL_GetPosition(*(void**) ((uint64_t) player + 0x1B0)); // atualizado 1.56
}

struct EnemyData {
    Vector2 Localation;
    Vector2 HeadLocation;
    Vector3 Enemy;
    Vector3 Player;
    float Distance;
    float Distancee;
    string PlayerName;
};

struct Response {
    int PlayerCount;
    EnemyData Players[maxplayerCount];
} response;

bool isValidPlayer(EnemyData data){
    return (data.Localation != Vector2::Zero() && data.HeadLocation != Vector2::Zero());
}

void *playerlate = NULL;
void *get_Player(void *player) {
    playerlate = player;
    return playerlate;
}

void (*LateUpdate)(void *componentPlayer);
            void AimBot(void *local_player, void *enemy_player) {

                int pose = GetPhysXPose(enemy_player);
                bool alive = get_isAlive(enemy_player);
                bool visible = get_isVisible(enemy_player);
                bool visi = get_AttackableEntity_IsVisible(enemy_player);
                bool visir = get_AttackableEntity_GetIsDead(enemy_player);
                bool sameteam = get_isLocalTeam(enemy_player);
                void *HeadTF = *(void **)((uintptr_t)enemy_player + Global.HeadTF);
                void *HipTF = *(void **)((uintptr_t)enemy_player + Global.HipTF);
                void *Main_Camera = *(void **)((uintptr_t)local_player + Global.MainCameraTransform);

                if (alive && pose != 8 && visible && !sameteam && HeadTF != NULL && Main_Camera != NULL && HipTF != NULL) {
                    Vector3 EnemyLocation = Transform_INTERNAL_GetPosition(HeadTF);
                    Vector3 CenterWS = GetAttackableCenterWS(local_player);
                    bool scope = get_IsSighting(local_player);
                    bool agachado = get_IsCrouching(local_player);
                    float distance = Vector3::Distance(CenterWS, EnemyLocation);

                    Vector3 PlayerLocation = Transform_INTERNAL_GetPosition(Main_Camera);
                    Quaternion PlayerLook = GetRotationToLocation(EnemyLocation, 0.1f, PlayerLocation);
                    Quaternion PlayerLook2 = GetRotationToLocation(Transform_INTERNAL_GetPosition(HipTF), 0.1f, PlayerLocation);
                    Vector3 fwd = GetForward(Main_Camera);

                    Vector3 nrml = Vector3::Normalized(EnemyLocation - PlayerLocation);
                    float PlayerDot = Vector3::Dot(fwd, nrml);
                    
                    if (MT.aimHead) {
                            set_aim(local_player, PlayerLook2);
                        }
                        if (MT.hs100) {
                            set_aim(local_player, PlayerLook);
                        }
                        if (MT.hs70) {
                            if (MT.aimbotauto)
                            {
                                set_aim(local_player, PlayerLook);
                                ++MT.semihs;
                            } else {
                                set_aim(local_player, PlayerLook2);
                                --MT.semihs;
                            }

                            if (MT.semihs == 6)
                            {
                                MT.aimbotauto = false;
                            } else if (MT.semihs == 0) {
                                MT.aimbotauto = true;
                            }
                            if (MT.semihs > 6 || MT.semihs < 0)
                            {
                                MT.semihs = 3;
                                MT.aimbotauto = true;
                            }
                        }
                        if(!MT.UseFOV && ((!MT.UseMxD) || (MT.UseMxD && distance < MT.MxDis)) && ((!MT.UseVisible) || (MT.UseVisible && get_isAlive))) {
                            if(feature0) {
                                set_aim(local_player, PlayerLook);
                            } else if(scope && MT.UseScope) {
                                set_aim(local_player, PlayerLook);
                            } else if (firing && MT.UseFiring)  {
                                set_aim(local_player, PlayerLook);
                            }
                        } else if(MT.UseFOV && PlayerDot > MT.FovCount2 && ((!MT.UseMxD) || (MT.UseMxD && distance < MT.MxDis)) && ((!MT.UseVisible) || (MT.UseVisible && get_isAlive))){
                            if(feature0) {
                                set_aim(local_player, PlayerLook);
                            } else if(scope && MT.UseScope) {
                                set_aim(local_player, PlayerLook);
                            } else if (firing && MT.UseFiring)  {
                                set_aim(local_player, PlayerLook);
                            }
                        }
                    }
                }
                int isOutsideSafezone(Vector2 pos, Vector2 screen) {
    Vector2 mSafezoneTopLeft(screen.X * 0.04f, screen.Y * 0.04f);
    Vector2 mSafezoneBottomRight(screen.X * 0.96f, screen.Y * 0.96f);

    int result = 0;
    if (pos.Y < mSafezoneTopLeft.Y) {
        result |= 1;
    }
    if (pos.X > mSafezoneBottomRight.X) {
        result |= 2;
    }
    if (pos.Y > mSafezoneBottomRight.Y) {
        result |= 4;
    }
    if (pos.X < mSafezoneTopLeft.X) {
        result |= 8;
    }
    return result;
}

Vector2 pushToScreenBorder(Vector2 Pos, Vector2 screen, int borders, int offset) {
    int X = (int) Pos.X;
    int Y = (int) Pos.Y;
    if ((borders & 1) == 1) {
        Y = 0 - offset;
    }
    if ((borders & 2) == 2) {
        X = (int) screen.X + offset;
    }
    if ((borders & 4) == 4) {
        Y = (int) screen.Y + offset;
    }
    if ((borders & 8) == 8) {
        X = 0 - offset;
    }
    return Vector2(X, Y);
}
void *hack_thread(void *) {
    LOGI("Loading...");

    ProcMap il2cppMap;
    do {
        il2cppMap = KittyMemory::getLibraryMap("libil2cpp.so");
        sleep(1);
    } while (!il2cppMap.isValid());
    hexPatches.Sensi1 = MemoryPatch("libil2cpp.so", 0xB8EB28, "\x00\x00\x96\x43", 4);
    hexPatches.Sensi2 = MemoryPatch("libil2cpp.so", 0xB8EB28, "\x00\x00\x7A\x43", 4);
    hexPatches.Sensi3 = MemoryPatch("libil2cpp.so", 0xB8EB28, "\x00\x00\x48\x43", 4);
    hexPatches.Sensi4 = MemoryPatch("libil2cpp.so", 0xB8EB28, "\x00\x00\x32\x43", 4);
    hexPatches.Sensi5 = MemoryPatch("libil2cpp.so", 0xB8EB28, "\x00\x00\x16\x43", 4);
    return NULL;
}

JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *globalEnv;
    vm->GetEnv((void **) &globalEnv, JNI_VERSION_1_6);

    pthread_t ptid;
    pthread_create(&ptid, NULL, hack_thread, NULL);
    return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL
JNI_OnUnload(JavaVM *vm, void *reserved) {}


__attribute__((constructor))
void initializer() {
    pthread_t ptid;
    pthread_create(&ptid, NULL, Super_thread, NULL);
}
