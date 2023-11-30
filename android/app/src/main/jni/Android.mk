LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := fractochales

LOCAL_SRC_FILES := ../../../../../main.cpp
LOCAL_SRC_FILES += ../../../../../includes/Button/button.cpp
LOCAL_SRC_FILES += ../../../../../includes/Lightning/Lightning.cpp
LOCAL_SRC_FILES += ../../../../../includes/Physics/physics.cpp
LOCAL_SRC_FILES += ../../../../../includes/Slider/slider.cpp
LOCAL_SRC_FILES += ../../../../../includes/Switch/switch.cpp
LOCAL_SRC_FILES += ../../../../../includes/ThickLine/thickline.cpp
LOCAL_SHARED_LIBRARIES := sfml-system
LOCAL_SHARED_LIBRARIES += sfml-window
LOCAL_SHARED_LIBRARIES += sfml-graphics
LOCAL_SHARED_LIBRARIES += sfml-audio
LOCAL_SHARED_LIBRARIES += sfml-network
LOCAL_SHARED_LIBRARIES += sfml-activity
LOCAL_SHARED_LIBRARIES += openal
LOCAL_WHOLE_STATIC_LIBRARIES := sfml-main

include $(BUILD_SHARED_LIBRARY)

$(call import-module,third_party/sfml)
