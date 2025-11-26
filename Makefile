CACHE = gcm.cache/usr/include/c++/14/
CXX = g++-14
CXXFLAGS = -std=c++20 -fmodules-ts -Wall -MMD -lncursesw -g
CXXHFLAGS = -std=c++20 -fmodules-ts -c -x c++-system-header
EXEC = mde
HEADERS = $(CACHE)map.gcm \
		  $(CACHE)vector.gcm \
		  $(CACHE)fstream.gcm \
		  $(CACHE)iostream.gcm \
		  $(CACHE)memory.gcm \
		  $(CACHE)string.gcm
OBJECTS = action.o \
		  mode.o \
		  controller.o \
		  mde_control.o \
		  component.o \
		  view.o \
		  cursescreen.o \
		  editorpane.o \
		  footer.o \
		  header.o \
		  previewpane.o \
		  mde_view.o \
		  model.o \
		  observer.o \
		  mde_model.o \
		  mde.o \
		  component-impl.o \
		  controller-impl.o \
		  mde_view-impl.o \
		  mde_control-impl.o \
		  cursescreen-impl.o \
		  editorpane-impl.o \
		  footer-impl.o \
		  header-impl.o \
		  previewpane-impl.o \
		  model-impl.o \
		  mde_model-impl.o \
		  main.o

DEPENDS = ${OBJECTS:.o=.d}

main: $(HEADERS) ${OBJECTS}
	${CXX} ${OBJECTS} ${GIVEN} -o ${EXEC} ${CXXFLAGS}

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.gcm:
	$(CXX) $(CXXHFLAGS) $(patsubst $(CACHE)%.gcm,%,$@)

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS} ./gcm.cache/*.gcm
