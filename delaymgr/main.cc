#include "delay_manager.h"
#include "delay_peak_detector.h"
#include "detector.h"

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

int main()
{
    static const int fs_hz_ = 48000;
    const size_t packet_length_samples = 20 * 8 * 6;
    DetectorInst *inst = DetectorCreate();
    DetectorSetAudioLength(inst, 20);

    uint32_t main_timestamp = 0;
    uint32_t timestamp_ = main_timestamp;
    uint16_t sequenceNumber = 100;
    static int count1 = 0;
    static int count2 = 0;
    static int count3 = 0;
    static int count4 = 0;

    for (int i = 0; i < 10000; ++i) {
        int32_t rd = rand()%200;
        int sec = rd / 10;

        if (sec >= 4) {
            count1++;
            sec = 0;
        } else if (sec >= 3) {
            count2++;
            sec = 5;
        } else if (sec >= 2) {
            count3++;
        } else if (sec >= 1) {
            count4++;
            sec = 0;
        } else {
            sec = 0;
        }

        for (int j = 0; j <= sec; ++j) {
            DetectorTickTimes(inst, 2);
        }

        main_timestamp += packet_length_samples;
        sequenceNumber += 1;

        DetectorUpdate(inst, sequenceNumber, main_timestamp, fs_hz_);
    }

    printf("%d\n", DetectorTargetLevel(inst));
    printf("%d,%d,%d,%d\n",count1,count2,count3,count4);
    
    return 0;
}