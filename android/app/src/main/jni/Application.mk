NDK_TOOLCHAIN_VERSION := 4.9
APP_PLATFORM := android-34
# APP_STL has to match CMAKE_ANDROID_STL_TYPE
APP_STL := c++_static
APP_ABI := all
APP_MODULES := sfml-activity fractochales
APP_OPTIM := release
APP_CFLAG += -g -ggdb -O0 -DVERSION=69