mde_control.o gcm.cache/mde-mde_control.gcm: mde_control.cc \
 gcm.cache/./usr/include/c++/14/map.gcm gcm.cache/mode.gcm \
 gcm.cache/action.gcm gcm.cache/controller.gcm
mde_control.o gcm.cache/mde-mde_control.gcm: controller.c++m \
 /usr/include/c++/14/map.c++m action.c++m mode.c++m
mde:mde_control.c++m: gcm.cache/mde-mde_control.gcm
.PHONY: mde:mde_control.c++m
gcm.cache/mde-mde_control.gcm:| mde_control.o
CXX_IMPORTS += controller.c++m /usr/include/c++/14/map.c++m action.c++m \
 mode.c++m
