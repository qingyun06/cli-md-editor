model.o gcm.cache/model.gcm: model.cc \
 gcm.cache/./usr/include/c++/14/vector.gcm \
 gcm.cache/./usr/include/c++/14/memory.gcm gcm.cache/view.gcm \
 gcm.cache/controller.gcm gcm.cache/action.gcm \
 gcm.cache/./usr/include/c++/14/string.gcm
model.o gcm.cache/model.gcm: controller.c++m \
 /usr/include/c++/14/vector.c++m view.c++m action.c++m \
 /usr/include/c++/14/memory.c++m
model.c++m: gcm.cache/model.gcm
.PHONY: model.c++m
gcm.cache/model.gcm:| model.o
CXX_IMPORTS += controller.c++m /usr/include/c++/14/vector.c++m view.c++m \
 action.c++m /usr/include/c++/14/memory.c++m
