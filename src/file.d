file.o gcm.cache/file.gcm: file.cc \
 gcm.cache/./usr/include/c++/14/fstream.gcm \
 gcm.cache/./usr/include/c++/14/iostream.gcm \
 gcm.cache/./usr/include/c++/14/sstream.gcm \
 gcm.cache/./usr/include/c++/14/vector.gcm \
 gcm.cache/./usr/include/c++/14/utility.gcm \
 gcm.cache/./usr/include/c++/14/string.gcm
file.o gcm.cache/file.gcm: /usr/include/c++/14/vector.c++m \
 /usr/include/c++/14/string.c++m /usr/include/c++/14/fstream.c++m \
 /usr/include/c++/14/sstream.c++m /usr/include/c++/14/iostream.c++m \
 /usr/include/c++/14/utility.c++m
file.c++m: gcm.cache/file.gcm
.PHONY: file.c++m
gcm.cache/file.gcm:| file.o
CXX_IMPORTS += /usr/include/c++/14/vector.c++m \
 /usr/include/c++/14/string.c++m /usr/include/c++/14/fstream.c++m \
 /usr/include/c++/14/sstream.c++m /usr/include/c++/14/iostream.c++m \
 /usr/include/c++/14/utility.c++m
