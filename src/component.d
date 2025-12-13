component.o gcm.cache/mde-component.gcm: component.cc \
 gcm.cache/./usr/include/c++/14/string.gcm
component.o gcm.cache/mde-component.gcm: /usr/include/c++/14/string.c++m
mde:component.c++m: gcm.cache/mde-component.gcm
.PHONY: mde:component.c++m
gcm.cache/mde-component.gcm:| component.o
CXX_IMPORTS += /usr/include/c++/14/string.c++m
