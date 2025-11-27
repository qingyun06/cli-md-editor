footer.o gcm.cache/mde-footer.gcm: footer.cc gcm.cache/mode.gcm \
 gcm.cache/mde-component.gcm gcm.cache/./usr/include/c++/14/string.gcm \
 gcm.cache/action.gcm
footer.o gcm.cache/mde-footer.gcm: mde:component.c++m action.c++m \
 mode.c++m
mde:footer.c++m: gcm.cache/mde-footer.gcm
.PHONY: mde:footer.c++m
gcm.cache/mde-footer.gcm:| footer.o
CXX_IMPORTS += mde:component.c++m action.c++m mode.c++m
