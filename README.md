# Overview
A lightweight cli-based text editor for Markdown (`.md`) files that provides a general preview render of Markdown elements.

# Setup
Running `make` in the `src` directory will produce the executable `mde` in the root. Requires a compiler that supports C++ 20.

# Usage
Running `./mde a.md` will open `a.md` in the editor. If `a.md` does not exist, the program will create `a.md` upon saving. The following vim commands are supported:

### Motion/Navigation:
* `ESC` for Normal mode
* arrow keys and `h`, `j`, `k`, `l`
* `0`, `$`, `:0` and `:$`
* `w`, `W`, `b`, and `B`
* `fx`, `Fx`, `,`, and `;`
* command multipliers

### Editing:
* `i`, `a`, `o`, `O`
* `dd`, `dw`, `diw`, `daw`
* `u`, `CTRL`+`r`

### Yank/Put:
* `yy`, `yw`, `yiw`, `yaw`, `y$`
* `p`

### Saving/Quitting:
* `:q`, `:q!`, `:wq`, `:w`

## Additional Features:
### Multiple Files:
To open a new file, type `CTRL`+`t` and `a.md` while in Normal mode to open `a.md` in a new tab. If `a.md` does not exist, the application will create a new file of that name. 
To navigate between tabs, use `-` and `=`. To close a tab, type `CTRL`+`w` while in Normal mode.

### Markdown:
The following Markdown elements are supported:
* headers (`#...`)
* inline and block code, with support for C/C++ syntax highlighting and primitive LaTex interpretation
  * To render LaTex: ` ```latex {latex code}``` `
* ordered and unordered lists
* block quotes (`>...`)
* horizontal lines (`---`)
* bold, italic, and bold-italic text

### Plugins:
There exists basic plugin support for block-code interpretation. Plugin implementation files (`*.cc`) reside in the `language-plugins` directory. They must be of the form:
```cpp
#include "language-plugin-api.h"
...
class ExampleInterpreter : public LanguagePlugin {
 public:
  ExampleInterpreter(): LanguagePlugin{std::string("example-language")} {} // where "example-language" follows the ``` to tell the renderer to use this plugin to render block code
  std::string interpret(const std::string &s) const override {
    ... // return a string in a marked-up format for the renderer (see below for more infomration)
  }
};

extern "C" LanguagePlugin *create_interp() { return new ExampleInterpreter; }
extern "C" void destroy_interp(LanguagePlugin *i) { delete i; }
```

Running `make` after adding a new `*.cc` plugin file to `language-plugins` will automatically compile it. The program will dynamically load all compiled plugins at run-time.

The method `interpret` transforms input `s` into a new string, which will be displayed in the preview (right-most) pane. The following colors are supported:
* `<BLUE>...</BLUE>` for blue
* identical formatting for `RED`, `YELLOW`, `GREEN`, `CYAN`, `MAGENTA`, `BLACK`, `WHITE`

### Other Features:
* You can adjust the horizontal spacing of the panes with `[` and `]`

