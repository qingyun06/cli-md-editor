CACHE = ./gcm.cache/usr/include/c++/14/
CXX = g++-14
CXXFLAGS = -std=c++20 -fmodules-ts -Wall -MMD -lncursesw -g
CXXHFLAGS = -std=c++20 -fmodules-ts -c -x c++-system-header
CXXSFLAGS = -std=c++20 -I. -fmodules-ts -fPIC -shared
EXEC = ../mde
HEADERS = $(CACHE)map.gcm \
		  $(CACHE)regex.gcm \
		  $(CACHE)vector.gcm \
		  $(CACHE)filesystem.gcm \
		  $(CACHE)fstream.gcm \
		  $(CACHE)sstream.gcm \
		  $(CACHE)iostream.gcm \
		  $(CACHE)memory.gcm \
		  $(CACHE)utility.gcm \
		  $(CACHE)map.gcm \
		  $(CACHE)unordered_set.gcm \
		  $(CACHE)cstring.gcm \
		  $(CACHE)algorithm.gcm \
		  $(CACHE)iterator.gcm \
		  $(CACHE)string.gcm
OBJECTS = action.o \
		  mode.o \
		  file.o \
		  language-plugin-loader.o \
		  controller.o \
		  mde_control.o \
		  component.o \
		  view.o \
		  editorpane.o \
		  footer.o \
		  header.o \
		  previewpane.o \
		  mde_view.o \
		  model.o \
		  observer.o \
		  mde_model.o \
		  mde.o \
		  file-impl.o \
		  component-impl.o \
		  controller-impl.o \
		  mde_view-impl.o \
		  mde_control-impl.o \
		  editorpane-impl.o \
		  footer-impl.o \
		  header-impl.o \
		  previewpane-impl.o \
		  model-impl.o \
		  mde_model-impl.o \
		  main.o
LANGPLUGINDIR = ../language-plugins
LANGPLUGINSRC = $(wildcard $(LANGPLUGINDIR)/*.cc)
LANGPLUGINS = $(patsubst $(LANGPLUGINDIR)/%.cc,$(LANGPLUGINDIR)/%.so,$(LANGPLUGINSRC))
DEPENDS = ${OBJECTS:.o=.d}

main: $(HEADERS) ${OBJECTS} ${LANGPLUGINS}
	${CXX} ${OBJECTS} ${GIVEN} -o ${EXEC} ${CXXFLAGS}

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(LANGPLUGINDIR)/%.so: $(LANGPLUGINDIR)/%.cc
	@mkdir -p $(LANGPLUGINDIR)
	${CXX} ${CXXSFLAGS} $< -o $@

%.gcm:
	$(CXX) $(CXXHFLAGS) $(patsubst $(CACHE)%.gcm,%,$@)

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS} ./gcm.cache/*.gcm $(LANGPLUGINDIR)/*.so
