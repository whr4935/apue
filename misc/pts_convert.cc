#include <iostream>
#include <stdio.h>
#include <stddef.h>
#include <inttypes.h>

int64_t mLastRecoveredPTS = 1;

int64_t recoverPTS(uint64_t PTS_33bit) {
    // We only have the lower 33-bit of the PTS. It could overflow within a
    // reasonable amount of time. To handle the wrap-around, use fancy math
    // to get an extended PTS that is within [-0xffffffff, 0xffffffff]
    // of the latest recovered PTS.
    if (mLastRecoveredPTS < 0ll) {
        // Use the original 33bit number for 1st frame, the reason is that
        // if 1st frame wraps to negative that's far away from 0, we could
        // never start. Only start wrapping around from 2nd frame.
        mLastRecoveredPTS = static_cast<int64_t>(PTS_33bit);
    } else {
        mLastRecoveredPTS = static_cast<int64_t>(
                ((mLastRecoveredPTS - static_cast<int64_t>(PTS_33bit) + 0x80ll)
                & 0xffffffffffffff00ull) | PTS_33bit);
        // We start from 0, but recovered PTS could be slightly below 0.
        // Clamp it to 0 as rest of the pipeline doesn't take negative pts.
        // (eg. video is read first and starts at 0, but audio starts at 0xfffffff0)
        if (mLastRecoveredPTS < 0ll) {
            printf("Clamping negative recovered PTS (%" PRId64 ") to 0", mLastRecoveredPTS);
            mLastRecoveredPTS = 0ll;
        }
    }

    return mLastRecoveredPTS;
}

int main(int argc, char *argv[])
{
	mLastRecoveredPTS = 129;
    std::cout << recoverPTS(1) << std::endl;

    return 0;
}
