language-plugin-loader.o gcm.cache/langpluginloader.gcm: \
 language-plugin-loader.cc language-plugin-api.h \
 gcm.cache/./usr/include/c++/14/string.gcm \
 gcm.cache/./usr/include/c++/14/filesystem.gcm \
 gcm.cache/./usr/include/c++/14/iostream.gcm \
 gcm.cache/./usr/include/c++/14/map.gcm \
 gcm.cache/./usr/include/c++/14/memory.gcm
language-plugin-loader.o gcm.cache/langpluginloader.gcm: \
 /usr/include/c++/14/map.c++m /usr/include/c++/14/filesystem.c++m \
 /usr/include/c++/14/string.c++m /usr/include/c++/14/memory.c++m \
 /usr/include/c++/14/iostream.c++m
langpluginloader.c++m: gcm.cache/langpluginloader.gcm
.PHONY: langpluginloader.c++m
gcm.cache/langpluginloader.gcm:| language-plugin-loader.o
CXX_IMPORTS += /usr/include/c++/14/map.c++m \
 /usr/include/c++/14/filesystem.c++m /usr/include/c++/14/string.c++m \
 /usr/include/c++/14/memory.c++m /usr/include/c++/14/iostream.c++m
