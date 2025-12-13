previewpane.o gcm.cache/mde-previewpane.gcm: previewpane.cc \
 gcm.cache/mde-component.gcm gcm.cache/./usr/include/c++/14/string.gcm \
 gcm.cache/mde-editorpane.gcm gcm.cache/./usr/include/c++/14/vector.gcm \
 gcm.cache/action.gcm
previewpane.o gcm.cache/mde-previewpane.gcm: action.c++m \
 mde:component.c++m mde:editorpane.c++m
mde:previewpane.c++m: gcm.cache/mde-previewpane.gcm
.PHONY: mde:previewpane.c++m
gcm.cache/mde-previewpane.gcm:| previewpane.o
CXX_IMPORTS += action.c++m mde:component.c++m mde:editorpane.c++m
