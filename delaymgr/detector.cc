#include <sstream>
#include <cstring>
#include "detector.h"
#include "delay_manager.h"
#include "delay_peak_detector.h"
#include "tick_timer.h"

static const size_t kMaxPacketInBuffer = 100;

typedef struct DetectorInstT_
{
    std::unique_ptr<DelayManager> delay_manager_;
    std::unique_ptr<TickTimer> tick_timer_;
    std::unique_ptr<DelayPeakDetector> delay_peak_detector_;
} DetectorInstImp;

DetectorInst* DetectorCreate()
{
    DetectorInstImp* instImp = new DetectorInstImp;
    instImp->tick_timer_.reset(new TickTimer);
    instImp->delay_peak_detector_.reset(new DelayPeakDetector(instImp->tick_timer_.get()));
    instImp->delay_manager_.reset(new DelayManager(kMaxPacketInBuffer, instImp->delay_peak_detector_.get(), instImp->tick_timer_.get()));
    instImp->delay_manager_->ResetPacketIatCount();
    DetectorInst* inst = reinterpret_cast<DetectorInst*>(instImp);

    return inst;
}
void DetectorFree(DetectorInst* inst)
{
    if (inst == nullptr) {
        return;
    }

    DetectorInstImp* instImp = reinterpret_cast<DetectorInstImp*>(inst);

    delete instImp;

    return;
}
void DetectorUpdate(DetectorInst* inst, uint16_t seq, uint32_t timestamp, int32_t hz)
{
    if (inst == nullptr) {
        return;
    }

    DetectorInstImp* instImp = reinterpret_cast<DetectorInstImp*>(inst);

    instImp->delay_manager_->Update(seq, timestamp, hz);

    return;

}
void DetectorTickTimes(DetectorInst* inst, int32_t x)
{
    if (inst == nullptr) {
        return;
    }

    DetectorInstImp* instImp = reinterpret_cast<DetectorInstImp*>(inst);

    instImp->tick_timer_->Increment(x);

    return;
}
int32_t DetectorTargetLevel(DetectorInst* inst)
{
    if (inst == nullptr) {
        return -1;
    }

    DetectorInstImp* instImp = reinterpret_cast<DetectorInstImp*>(inst);

    return (instImp->delay_manager_->TargetLevel() >> 8);
}
void DetectorSetAudioLength(DetectorInst* inst, int32_t length)
{
    if (inst == nullptr || length < 10 || length % 10 != 0) {
        return;
    }

    DetectorInstImp* instImp = reinterpret_cast<DetectorInstImp*>(inst);

    instImp->delay_manager_->SetPacketAudioLength(length);

    return;
}

int32_t DetectorGetHistogram(DetectorInst* inst, char* str, int32_t len)
{
    if (inst == nullptr || str == nullptr) {
        return 0;
    }

    DetectorInstImp* instImp = reinterpret_cast<DetectorInstImp*>(inst);
    
    const DelayManager::IATVector& iat = instImp->delay_manager_->iat_vector();

    std::stringstream ss;
    for (int data : iat) {
        ss << data <<",";
    }
    std::string result = ss.str();
    if (static_cast<uint32_t>(len) <= result.length() || result.length() <= 0) {
        return 0;
    }
    
    memcpy(str, result.c_str(), result.length());
    str[result.length() - 1] = '\0';

    return result.length() - 1;
}