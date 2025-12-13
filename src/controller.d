controller.o gcm.cache/controller.gcm: controller.cc gcm.cache/action.gcm \
 gcm.cache/./usr/include/c++/14/string.gcm
controller.o gcm.cache/controller.gcm: action.c++m
controller.c++m: gcm.cache/controller.gcm
.PHONY: controller.c++m
gcm.cache/controller.gcm:| controller.o
CXX_IMPORTS += action.c++m
