mde_view.o gcm.cache/mde-mde_view.gcm: mde_view.cc \
 gcm.cache/./usr/include/c++/14/vector.gcm \
 gcm.cache/./usr/include/c++/14/memory.gcm gcm.cache/view.gcm \
 gcm.cache/mde-component.gcm gcm.cache/./usr/include/c++/14/string.gcm \
 gcm.cache/mde-header.gcm gcm.cache/action.gcm gcm.cache/mde-footer.gcm \
 gcm.cache/mode.gcm gcm.cache/mde-editorpane.gcm \
 gcm.cache/mde-previewpane.gcm
mde_view.o gcm.cache/mde-mde_view.gcm: mde:editorpane.c++m \
 /usr/include/c++/14/vector.c++m view.c++m mde:footer.c++m \
 mde:component.c++m mde:header.c++m mde:previewpane.c++m \
 /usr/include/c++/14/memory.c++m
mde:mde_view.c++m: gcm.cache/mde-mde_view.gcm
.PHONY: mde:mde_view.c++m
gcm.cache/mde-mde_view.gcm:| mde_view.o
CXX_IMPORTS += mde:editorpane.c++m /usr/include/c++/14/vector.c++m \
 view.c++m mde:footer.c++m mde:component.c++m mde:header.c++m \
 mde:previewpane.c++m /usr/include/c++/14/memory.c++m
