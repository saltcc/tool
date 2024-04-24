#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct WebRtcDetectorInst DetectorInst;

DetectorInst* DetectorCreate();
void DetectorFree(DetectorInst* inst);
void DetectorUpdate(DetectorInst* inst, uint16_t seq, uint32_t timestamp, int32_t hz);
void DetectorTickTimes(DetectorInst* inst, int32_t x);
int32_t DetectorTargetLevel(DetectorInst* inst);
void DetectorSetAudioLength(DetectorInst* inst, int32_t length);
int32_t DetectorGetHistogram(DetectorInst* inst, char* str, int32_t length);

#ifdef __cplusplus
}
#endif