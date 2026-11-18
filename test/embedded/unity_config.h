// unity_config.h — shared by every embedded (on-target) test under test/embedded/.
// Mirrors the Pizza Project's unity_config at the group folder level, so the
// Unity serial setup isn't duplicated into each hardware test.
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void unityOutputStart(unsigned long baud);
void unityOutputChar(unsigned int c);
void unityOutputFlush(void);
void unityOutputComplete(void);

// Unity hooks into these:
#define UNITY_OUTPUT_START()    unityOutputStart(115200)
#define UNITY_OUTPUT_CHAR(c)    unityOutputChar(c)
#define UNITY_OUTPUT_FLUSH()    unityOutputFlush()
#define UNITY_OUTPUT_COMPLETE() unityOutputComplete()

#ifdef __cplusplus
}
#endif