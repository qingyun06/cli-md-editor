action.o gcm.cache/action.gcm: action.cc \
 gcm.cache/./usr/include/c++/14/string.gcm
action.o gcm.cache/action.gcm: /usr/include/c++/14/string.c++m
action.c++m: gcm.cache/action.gcm
.PHONY: action.c++m
gcm.cache/action.gcm:| action.o
CXX_IMPORTS += /usr/include/c++/14/string.c++m
