"""Host-side tests of controller slot arbitration and input mapping.

Tests simulated native code, NOT the Wii's memory manager or hardware.
"""
from pathlib import Path
import subprocess
import tempfile

src=Path(__file__).with_name('gcnhook.c').read_text()
a=src.index('#if defined(REGION_EU)')
b=src.index('\n#endif',a)+len('\n#endif')
stubs=r'''
static State testState;
#define state testState
static void* testSlots[4];
static void* testManager = testSlots;
#define manager testManager
#define REVO_VTABLE 0x12345678u
static PADStatus testPad;
static int connectedRemotes[4];
static int fakeRemotes[4];
static int resetCount;
static int readCount;
static void PADInit(void) {}
static u32 PADRead(PADStatus* pad) { pad[0] = testPad; return 0; }
static u32 PADReset(u32 mask) { resetCount++; return mask; }
static f32 absclamp(const f32* v, const f32* lim) {
  return (*v > *lim) ? *lim : ((*v < -*lim) ? -*lim : *v);
}
static void MgrRead(void* mgr) {
  void** slots = (void**)mgr;
  for(int i=0; i<4; i++) {
    if (connectedRemotes[i]) slots[i] = &fakeRemotes[i];
    else if (slots[i] == &fakeRemotes[i]) slots[i] = NULL;
  }
}
static void RevoRead(void* revo) { (void)revo; readCount++; }
'''
src=src[:a]+stubs+src[b:]
tests=r'''
#include <assert.h>
#include <math.h>
#include <string.h>
static void zero(void) {
  memset(&testState,0,sizeof(testState)); memset(&testPad,0,sizeof(testPad));
  memset(testSlots,0,sizeof(testSlots)); memset(connectedRemotes,0,sizeof(connectedRemotes));
  readCount=0;resetCount=0;
}
static void near(float a,float b){assert(fabsf(a-b)<0.00001f);}
static void assert_gc_slot(int channel){
  for(int i=0;i<4;i++){
    if (i==channel){assert(testSlots[i]==&testState.revo); assert(testState.revo.chan==(unsigned)i);}
    else {assert(testSlots[i]!=&testState.revo);}
  }
}
int main(void){
  // Isolated controller: P1 slot; no drift inside deadzone.
  zero();testPad.err=0;testPad.stickX=12;testPad.stickY=-12;
  gcnhook();assert_gc_slot(0);
  near(testState.posX,0);near(testState.posY,0);
  testPad.stickX=13;testPad.stickY=-13;gcnhook();
  near(testState.posX,13.f/128.f*.05f);near(testState.posY,13.f/128.f*.05f);
  // GC Start/Home + A press and release.
  testPad.button=0x1100;gcnhook();assert(testState.kpad.hold==0x8800);
  assert(testState.kpad.trig==0x8800);testPad.button=0;gcnhook();
  assert(testState.kpad.release==0x8800);
  // Wii Remote connected before GC: physical P1 must remain unchanged.
  zero();connectedRemotes[0]=1;testPad.err=0;
  gcnhook();assert(testSlots[0]==&fakeRemotes[0]);assert_gc_slot(1);
  // GC connected before Wii Remote: virtual GC must move to next slot.
  zero();testPad.err=0;gcnhook();assert_gc_slot(0);
  connectedRemotes[0]=1;gcnhook();assert(testSlots[0]==&fakeRemotes[0]);assert_gc_slot(1);
  // A second Wii Remote can connect and GC can migrate again.
  connectedRemotes[1]=1;gcnhook();assert(testSlots[1]==&fakeRemotes[1]);assert_gc_slot(2);
  // Full slots: no GC slot; do not overwrite any real controller.
  connectedRemotes[2]=1;connectedRemotes[3]=1;gcnhook();
  for(int i=0;i<4;i++) assert(testSlots[i]==&fakeRemotes[i]);
  // Remove remotes: GC can reclaim P1.
  for(int i=0;i<4;i++)connectedRemotes[i]=0;
  gcnhook();assert_gc_slot(0);
  // Unplug GC: only GC virtual slot removed, no stale button state.
  testPad.err=-1;gcnhook();assert(testSlots[0]==NULL);
  assert(!testState.present);assert(resetCount==1);assert(!testState.kpad.hold);
  assert(!testState.kpad.dpd_valid_fg);
  return 0;
}
'''
with tempfile.TemporaryDirectory() as d:
  c=Path(d)/'coexist_test.c';exe=Path(d)/'coexist_test';c.write_text(src+tests)
  subprocess.run(['gcc','-std=c99','-O2','-Wall','-Wextra',str(c),'-lm','-o',str(exe)],check=True)
  subprocess.run([str(exe)],check=True)
print('PASS: simulated GC-only, Wii-first, GC-first, controller migration, full slots, disconnect, A/Start, deadzone')
