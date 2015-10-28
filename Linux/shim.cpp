// Compile this file with: arm-linux-androideabi-g++ -shared -fPIC -ldl -o shim.so shim.cpp
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <dlfcn.h>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE // For RTLD_NEXT
#endif

typedef enum {
    AUDIO_SOURCE_DEFAULT             = 0,
    AUDIO_SOURCE_MIC                 = 1,
    AUDIO_SOURCE_VOICE_UPLINK        = 2,
    AUDIO_SOURCE_VOICE_DOWNLINK      = 3,
    AUDIO_SOURCE_VOICE_CALL          = 4,
    AUDIO_SOURCE_CAMCORDER           = 5,
    AUDIO_SOURCE_VOICE_RECOGNITION   = 6,
    AUDIO_SOURCE_VOICE_COMMUNICATION = 7,
    AUDIO_SOURCE_CNT,
    AUDIO_SOURCE_MAX                 = AUDIO_SOURCE_CNT - 1,
} audio_source_t;

namespace android {

struct AudioSource {
	AudioSource(
            audio_source_t inputSource,
            uint32_t sampleRate,
            uint32_t channels = 1);
};

AudioSource::AudioSource(
        audio_source_t inputSource,
        uint32_t sampleRate,
        uint32_t channels) {
    typedef void (AudioSource::*Original)(audio_source_t, uint32_t, uint32_t, int);

    static Original originalMethod = 0;

    if (originalMethod == 0) {
        // Save a pointer to the original constructor in originalMethod.
        void *tmpPtr = dlsym(RTLD_NEXT, "_ZN7android11AudioSourceC1E14audio_source_tjji");
        memcpy(&originalMethod, &tmpPtr, sizeof(void *));
    }

    // Call the original constructor with last argument hardcoded to 0.
    printf("Calling original (originalMethod=%p)\n", originalMethod);
    (this->*originalMethod)(inputSource, sampleRate, channels, 0);
}

} // namespace android
