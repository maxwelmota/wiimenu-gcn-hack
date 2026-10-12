typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed char s8;
typedef u8 bool;
typedef int s32;
typedef float f32;

#define true 1
#define false 0
#define NULL 0

typedef struct PADStatus {
    u16 button;
    s8 stickX, stickY, substickX, substickY;
    u8 triggerLeft, triggerRight, analogA, analogB;
    s8 err;
} PADStatus;

typedef struct KPAD {
    u32 hold, trig, release;
    u8 padding0[0x20 - 0x0C];
    f32 posx, posy;
    u8 padding1[0x5C - 0x28];
    u8 dev_type;
    s8 wpad_err;
    s8 dpd_valid_fg;
    u8 data_format;
    u8 ex_status[0x24];
} KPAD;

typedef struct Revo {
    u32 vtable;
    u8 mButton;
    u8 p0[3];
    u32 unk08;
    u32 lastRumbleTime;
    s32 rumbleType;
    u32 chan;
    u32 type;
    u8 unk1C, unk1D, unk1E;
    u8 p1;
    KPAD* kpad;
} Revo;

typedef struct State {
    PADStatus pad[4];
    KPAD kpad;
    Revo revo;
    f32 posX, posY;
    u16 prevBtn;
    bool present;
    bool hijacked;
    bool inited;
} State;

#if defined(REGION_EU)

#define PADInit ((void (*)(void))0x815768D0)
#define PADRead ((u32 (*)(PADStatus*))0x81576A2C)
#define PADReset ((u32 (*)(u32))0x815766BC)
#define WPADDisconnect ((s32 (*)(s32))0x8157AA5C)
#define KPADEnableDPD ((void (*)(s32))0x815884AC)
#define absclamp ((f32 (*)(const f32*, const f32*))0x8133675C)
#define MgrRead ((void (*)(void*))0x81337570)
#define RevoRead ((void (*)(void*))0x8133613C)

#define manager (*(void**)0x81089070)
#define state (*(State*)0x81359F88)

#define REVO_VTABLE 0x81634AA4

#elif defined(REGION_JP)

#define PADInit ((void (*)(void))0x8159F714)
#define PADRead ((u32 (*)(PADStatus*))0x8159F870)
#define PADReset ((u32 (*)(u32))0x8159F500)
#define WPADDisconnect ((s32 (*)(s32))0x815A38A0)
#define KPADEnableDPD ((void (*)(s32))0x815B12F0)
#define absclamp ((f32 (*)(const f32*, const f32*))0x81335BCC)
#define MgrRead ((void (*)(void*))0x813369E0)
#define RevoRead ((void (*)(void*))0x813355AC)

#define manager (*(void**)0x81089070)
#define state (*(State*)0x813593A0)

#define REVO_VTABLE 0x81665144

#else

#define PADInit ((void (*)(void))0x815767D4)
#define PADRead ((u32 (*)(PADStatus*))0x81576930)
#define PADReset ((u32 (*)(u32))0x815765C0)
#define WPADDisconnect ((s32 (*)(s32))0x8157A960)
#define KPADEnableDPD ((void (*)(s32))0x815883B0)
#define absclamp ((f32 (*)(const f32*, const f32*))0x813366EC)
#define MgrRead ((void (*)(void*))0x81337500)
#define RevoRead ((void (*)(void*))0x813360CC)

#define manager (*(void**)0x81089070)
#define state (*(State*)0x81359EE0)

#define REVO_VTABLE 0x816347F4

#endif

#define PAD_ERR_NONE 0
#define PAD_ERR_NO_CONTROLLER -1
#define PAD_CHAN0_BIT 0x80000000u

#define GC_LIM_X 1.8f
#define GC_LIM_Y 1.2f
#define GC_SPEED 0.05f
#define WII_MENU_CONTROLLER_SLOTS 4

// Values read from PADRead are signed 8-bit stick positions.
// Ignore small deviations around neutral to prevent cursor drift.
#define GC_STICK_DEADZONE 12
#define GC_PAD_BUTTON_START 0x1000u
// Wii Menu/KPAD native button bits (not libogc's WPAD_BUTTON_HOME).
#define WII_KPAD_BUTTON_HOME 0x8000u

static const u16 gcBtnMap[20] = {
    0x0001, 0x0001,
    0x0002, 0x0002,
    0x0004, 0x0004,
    0x0008, 0x0008,
    0x0100, 0x0800,
    0x0200, 0x0400,
    0x0400, 0x0200,
    0x0800, 0x0100,
    0x0040, 0x1000,
    0x0020, 0x0010
};

static __attribute__((noinline)) f32 gcMoveAxis(f32 current, s8 stick, f32 scaledSpeed, f32 axisLimit);

void gcnhook(void) {
    void** slots;
    s32 err, i;
    u16 gcRawBtn, btn;
    if (!state.inited) {
        PADInit();
        state.inited = true;
    }
    PADRead(state.pad);
    err = state.pad[0].err;
    if (err == PAD_ERR_NO_CONTROLLER) {
        state.present = false;
        PADReset(PAD_CHAN0_BIT);
    } 
    else if (err == PAD_ERR_NONE) {
        state.present = true;
        state.posX = gcMoveAxis(state.posX, state.pad[0].stickX, GC_SPEED / 128.0f, GC_LIM_X);
        state.posY = gcMoveAxis(state.posY, state.pad[0].stickY, -GC_SPEED / 128.0f, GC_LIM_Y);
        state.kpad.posx = state.posX;
        state.kpad.posy = state.posY;
        state.kpad.dpd_valid_fg = true;
        state.kpad.wpad_err = 0;
        gcRawBtn = state.pad[0].button;
        btn = 0;
        for (i = 0; i < 20; i += 2) {
            if (gcRawBtn & gcBtnMap[i]) {
                btn |= gcBtnMap[i + 1];
            }
        }
        // Start on the GameCube pad emulates HOME on the Wii Menu.
        if (gcRawBtn & GC_PAD_BUTTON_START) {
            btn |= WII_KPAD_BUTTON_HOME;
        }
        state.kpad.hold = btn;
        state.kpad.trig = (u32)(btn & ~state.prevBtn);
        state.kpad.release = (u32)(state.prevBtn & ~btn);
        state.prevBtn = btn;
    }
    // Free only the virtual GameCube slot before the native manager scans
    // connected Wii Remotes. This lets a newly connected Wii Remote reclaim
    // its own slot instead of being shadowed by our virtual controller.
    slots = (void**)manager;
    if (slots && state.revo.chan < WII_MENU_CONTROLLER_SLOTS &&
        slots[state.revo.chan] == (void*)&state.revo) {
        slots[state.revo.chan] = NULL;
    }
    if (manager) {
        MgrRead(manager);
    }
    slots = (void**)manager;
    if (state.present && slots) {
        if (state.revo.vtable == NULL) {
            state.revo.vtable = REVO_VTABLE;
            state.revo.rumbleType = -1;
            state.revo.type = 0;
            state.revo.kpad = &state.kpad;
        }
        // Do not overwrite an actual Wii Remote. Pick the lowest free
        // controller slot (P1 when alone; usually P2 when Wii Remote is P1).
        for (i = 0; i < WII_MENU_CONTROLLER_SLOTS; i++) {
            if (slots[i] == NULL) {
                state.revo.chan = (u32)i;
                slots[i] = (void*)&state.revo;
                RevoRead((void*)&state.revo);
                break;
            }
        }
    }
    if (!state.present) {
        state.kpad.hold = state.kpad.trig = state.kpad.release = 0;
        state.kpad.wpad_err = -1;
        state.kpad.dpd_valid_fg = false;
        state.prevBtn = 0;
    }
}

static __attribute__((noinline)) f32 gcMoveAxis(f32 current, s8 stick, f32 scaledSpeed, f32 axisLimit) {
    f32 result;
    if (stick >= -GC_STICK_DEADZONE && stick <= GC_STICK_DEADZONE) {
        stick = 0;
    }
    result = current + ((f32)stick * scaledSpeed);
    return absclamp(&result, &axisLimit);
}

