
#include "utils.hpp"
#include "miniaudio.h"
#include <iostream>

// reference to https://miniaud.io/docs/examples/simple_enumeration.html

class device_manager {

    ma_context context;
    
    public:

    ma_device_info* pPlaybackDeviceInfos;
    unsigned int playbackDeviceCount;

    ma_device_info* pCaptureDeviceInfos;
    unsigned int captureDeviceCount;

    void update_devices() {
        result = ma_context_get_devices(&context, &pPlaybackDeviceInfos, &playbackDeviceCount, &pCaptureDeviceInfos, &captureDeviceCount);
        check_result("Failed to retrieve device information.\n");
    }

    device_manager() {
        
        result = ma_context_init(NULL, 0, NULL, &context);
        check_result("Failed to initialize context.\n");
        

    }

    ~device_manager() {
        ma_context_uninit(&context);
    }

    

    
    
};