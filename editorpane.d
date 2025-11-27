editorpane.o gcm.cache/mde-editorpane.gcm: editorpane.cc \
 gcm.cache/./usr/include/c++/14/vector.gcm \
 gcm.cache/./usr/include/c++/14/string.gcm gcm.cache/mde-component.gcm \
 gcm.cache/action.gcm
editorpane.o gcm.cache/mde-editorpane.gcm: \
 /usr/include/c++/14/vector.c++m /usr/include/c++/14/string.c++m \
 action.c++m mde:component.c++m
mde:editorpane.c++m: gcm.cache/mde-editorpane.gcm
.PHONY: mde:editorpane.c++m
gcm.cache/mde-editorpane.gcm:| editorpane.o
CXX_IMPORTS += /usr/include/c++/14/vector.c++m \
 /usr/include/c++/14/string.c++m action.c++m mde:component.c++m
