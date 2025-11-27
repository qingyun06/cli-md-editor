mde.o gcm.cache/mde.gcm: mde.cc gcm.cache/mde-component.gcm \
 gcm.cache/./usr/include/c++/14/string.gcm gcm.cache/mde-header.gcm \
 gcm.cache/action.gcm gcm.cache/mde-footer.gcm gcm.cache/mode.gcm \
 gcm.cache/mde-editorpane.gcm gcm.cache/./usr/include/c++/14/vector.gcm \
 gcm.cache/mde-previewpane.gcm gcm.cache/mde-mde_view.gcm \
 gcm.cache/./usr/include/c++/14/memory.gcm gcm.cache/view.gcm \
 gcm.cache/mde-mde_control.gcm gcm.cache/./usr/include/c++/14/map.gcm \
 gcm.cache/controller.gcm gcm.cache/mde-mde_model.gcm \
 gcm.cache/./usr/include/c++/14/fstream.gcm \
 gcm.cache/./usr/include/c++/14/iostream.gcm gcm.cache/model.gcm
mde.o gcm.cache/mde.gcm: mde:mde_model.c++m mde:previewpane.c++m \
 mde:header.c++m mde:editorpane.c++m mde:mde_view.c++m \
 mde:mde_control.c++m mde:footer.c++m mde:component.c++m
mde.c++m: gcm.cache/mde.gcm
.PHONY: mde.c++m
gcm.cache/mde.gcm:| mde.o
CXX_IMPORTS += mde:mde_model.c++m mde:previewpane.c++m mde:header.c++m \
 mde:editorpane.c++m mde:mde_view.c++m mde:mde_control.c++m \
 mde:footer.c++m mde:component.c++m
