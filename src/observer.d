observer.o gcm.cache/observer.gcm: observer.cc \
 gcm.cache/./usr/include/c++/14/vector.gcm
observer.o gcm.cache/observer.gcm: /usr/include/c++/14/vector.c++m
observer.c++m: gcm.cache/observer.gcm
.PHONY: observer.c++m
gcm.cache/observer.gcm:| observer.o
CXX_IMPORTS += /usr/include/c++/14/vector.c++m
