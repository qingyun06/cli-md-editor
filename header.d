header.o gcm.cache/mde-header.gcm: header.cc \
 gcm.cache/./usr/include/c++/14/string.gcm gcm.cache/mde-component.gcm \
 gcm.cache/action.gcm
header.o gcm.cache/mde-header.gcm: mde:component.c++m action.c++m \
 /usr/include/c++/14/string.c++m
mde:header.c++m: gcm.cache/mde-header.gcm
.PHONY: mde:header.c++m
gcm.cache/mde-header.gcm:| header.o
CXX_IMPORTS += mde:component.c++m action.c++m \
 /usr/include/c++/14/string.c++m
