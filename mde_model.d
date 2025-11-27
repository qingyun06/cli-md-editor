mde_model.o gcm.cache/mde-mde_model.gcm: mde_model.cc \
 gcm.cache/./usr/include/c++/14/fstream.gcm \
 gcm.cache/./usr/include/c++/14/iostream.gcm \
 gcm.cache/./usr/include/c++/14/vector.gcm \
 gcm.cache/./usr/include/c++/14/memory.gcm \
 gcm.cache/./usr/include/c++/14/string.gcm gcm.cache/action.gcm \
 gcm.cache/model.gcm gcm.cache/view.gcm gcm.cache/controller.gcm \
 gcm.cache/mode.gcm
mde_model.o gcm.cache/mde-mde_model.gcm: model.c++m \
 /usr/include/c++/14/vector.c++m /usr/include/c++/14/string.c++m \
 /usr/include/c++/14/fstream.c++m action.c++m mode.c++m \
 /usr/include/c++/14/memory.c++m /usr/include/c++/14/iostream.c++m
mde:mde_model.c++m: gcm.cache/mde-mde_model.gcm
.PHONY: mde:mde_model.c++m
gcm.cache/mde-mde_model.gcm:| mde_model.o
CXX_IMPORTS += model.c++m /usr/include/c++/14/vector.c++m \
 /usr/include/c++/14/string.c++m /usr/include/c++/14/fstream.c++m \
 action.c++m mode.c++m /usr/include/c++/14/memory.c++m \
 /usr/include/c++/14/iostream.c++m
