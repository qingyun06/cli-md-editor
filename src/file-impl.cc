module;
#include <ncurses.h>
module file;

int min(int x, int y) { return (x < y) ? x : y; }
bool isutf8(char c) { return (c & 0xC0) != 0x80; }
size_t len8(std::string s) {
  size_t len = 0;
  for (char c:s) if ((c & 0xC0) != 0x80) ++len;
  return len;
} 

// NAVIGATION -----------------------------------------------------------------

void File::moveCursorUp() {
  if (cursory) {
    --cursory;
    cursorx = min(cursormaxx, len8(lines[cursory]));
  } else {
    cursorx = 0;
  }
  if (viewstart && cursory - viewstart < 5) shiftviewup();
}
void File::moveCursorDown() {
  if (cursory < lines.size() - 1) {
    ++cursory;
    cursorx = min(cursormaxx, len8(lines[cursory]));
  } else {
    cursorx = len8(lines[cursory]);
  }
  if (lines.size() > viewend && viewend - cursory < 5) shiftviewdown();
}
void File::moveCursorLeft() {
  if (cursorx) {
    cursormaxx = --cursorx;
  } else if (cursory) {
    --cursory;
    cursorx = len8(lines[cursory]);
  }
  if (viewstart && cursory - viewstart < 5) shiftviewup();
}
void File::moveCursorRight() {
  if (cursorx < len8(lines[cursory])) {
    cursormaxx = ++cursorx;
  } else if (cursory < lines.size() - 1) {
    ++cursory;
    cursorx = 0;
  }
  if (lines.size() > viewend && viewend - cursory < 5) shiftviewdown();
}
unsigned int File::peekf() {
  if (cursorx < len8(lines[cursory]) - 1) return lines[cursory][cursorx + 1];
  else if (cursory < lines.size() - 1) return '\n';
  else return 0;
}
unsigned int File::peekb() {
  if (cursorx) return lines[cursory][cursorx - 1];
  else if (cursory) return '\n';
  else return 0;
}
unsigned int File::peek2b() {
  if (cursorx > 1) return lines[cursory][cursorx - 2];
  else if (cursorx) {
    if (cursory) return lines[cursory - 1][lines[cursory].size() - 1];
    return 0;
  } else {
    if (cursory) {
      if (lines[cursory - 1].size() > 0) return lines[cursory - 1][lines[cursory - 1].size() - 2];
      if (cursory > 1) return lines[cursory - 2][lines[cursory - 2].size() - 1];
      return 0;
    }
    return 0;
  }
}
void File::skipwsl() {
  while (peekb() && (lines[cursory].empty() ||
         std::isspace(lines[cursory][cursorx]))) moveCursorLeft();
}
void File::skipwsr() {
  while (peekf() && (lines[cursory].empty() ||
         std::isspace(lines[cursory][cursorx]))) moveCursorRight();
}
void File::moveCursorw() {
  if (lines[cursory].empty()) {
    moveCursorRight(); skipwsr();
  }
  else if (std::isspace(lines[cursory][cursorx])) {
    skipwsr(); return;
  } else {
    if (cursorx == lines[cursory].size()) {
      moveCursorRight();
      skipwsr();
    } else if (std::ispunct(lines[cursory][cursorx])) {
      moveCursorRight();
      if (std::isspace(lines[cursory][cursorx])) skipwsr();
    } else {
      while (!std::ispunct(lines[cursory][cursorx]) &&
             !std::isspace(lines[cursory][cursorx]) &&
             cursorx != lines[cursory].size()) {
        // if (cursorx == lines[cursory].size()) {
        //   moveCursorRight();
        //   skipwsr();
        //   return;
        // } else {
          moveCursorRight();
        // }
      }
      skipwsr();
    }
  }
  
  // (std::isalnum(peekf())) {
  //   moveCursorRight(); skipwsr();
  // }
  //   skipwsr();
//   if (std::ispunct(lines[cursory][cursorx]) && std::isalnum(peekf())) {
//     moveCursorRight();
//     return;
//   }
//   int c;
//   while (peekf() && cursorx < len8(lines[cursory]) &&
//            !std::ispunct(c) && !std::isspace(c)) {
//     moveCursorRight();
//     c = lines[cursory][cursorx];
//   }
//   if (cursorx == lines[cursory].size() || lines[cursory].empty()) {
//     moveCursorDown(); cursorx = 0;
//   }
//   skipwsr();
}
void File::moveCursorb() {
  moveCursorLeft();
  if (lines[cursory].empty()) return;
  skipwsl();
  int c = lines[cursory][cursorx];
  if (std::isalnum(c)) {
    while (std::isalnum(peekb())) moveCursorLeft();
  }
}
void File::findforward() {
  for (auto i = cursorx + 1; i < len8(lines[cursory]); ++i)
    if (lines[cursory][i] == searchc) { cursorx = i; return; }
}
void File::findbackward() {
  for (int i = cursorx - 1; i >= 0; --i)
    if (lines[cursory][i] == searchc) { cursorx = i; return; }
}
void File::findrepeat() {
  if (lastsearchdir) findforward();
  else findbackward();
}
void File::findopposite() {
  if (lastsearchdir) findbackward();
  else findforward();
}
void File::moveCursorbegl() {
  cursorx = 0;
}
void File::moveCursorendl() {
  cursorx = len8(lines[cursory]);
}
void File::moveCursorbegf() {
  cursorx = 0; cursormaxx = 0; cursory = 0;
  viewend = viewend - viewstart;
  viewstart = 0;
}
void File::moveCursorendf() {
  cursorx = 0; cursormaxx = 0; cursory = lines.size() - 1;
  viewstart = lines.size() - (viewend - viewstart);
  viewend = lines.size();
}
void File::moveAppend() {
  if (!lines[cursory].empty()) moveCursorRight();
}

// INSERTION ------------------------------------------------------------------
int countwords(std::string s) {
  size_t twc = 0;
  if (!std::isspace(*s.begin())) ++twc;
  for (auto it = s.begin(); it != s.end(); ++it) {
    auto next = it;
    if (std::isspace(*it) && ++next != s.end() && !std::isspace(*next)) ++twc;
  }
  return twc;
}
void File::insert(int c) {
  std::string s;
  size_t oldx, oldy;
  if ((isspace(c) && isalnum(peekb()) && isalnum(lines[cursory][cursorx])) ||
      (isalnum(c) && (!peekb() || isspace(peekb())) &&
        (cursorx == lines[cursory].size() || isspace(lines[cursory][cursorx])))) ++wc;
  switch (c) {
    case KEY_UP:
      moveCursorUp();
      break;
    case KEY_DOWN:
      moveCursorDown();
      break;
    case KEY_LEFT:
      moveCursorLeft();
      break;
    case KEY_RIGHT:
      moveCursorRight();
      break;
    case KEY_BACKSPACE:
      if (((cursorx || cursory) && (!peekb() || isalnum(peekb())) && (!peek2b() || isspace(peek2b())) && (cursorx == lines[cursory].size() || isspace(lines[cursory][cursorx])))
          || (isalnum(peek2b()) && isalnum(lines[cursory][cursorx]) && isspace(peekb()))) --wc;
      if (!cursorx) {
        if (cursory) {
          s = lines[cursory];
          moveCursorLeft();
          lines.erase(lines.begin() + cursory + 1);
          lines[cursory] += s;
        }
      } else {
        oldx = cursorx; oldy = cursory;
        moveCursorLeft();
        undoStack.push_back(Operation{false, oldx, oldy, cursorx, cursory, 
          std::string(1, lines[cursory][cursorx])});
        redoStack.clear();
        lines[cursory].erase(cursorx, 1);
      }
      if (cc) --cc;
      break;
    case '\n':
    case '\r':
    case KEY_ENTER:
      // if (cursorx && isalnum(peekb()) && isalnum(peekf())) ++wc;
      oldx = cursorx; oldy = cursory;
      s = lines[cursory];
      lines[cursory] = s.substr(0, cursorx);
      s = s.substr(cursorx);
      lines.insert(lines.begin() + cursory + 1, s);
      moveCursorDown();
      cursorx = 0;
      undoStack.push_back(Operation{true, oldx, oldy, cursorx, cursory, s, true});
      redoStack.clear();
      ++cc;
      break;
    case '\t':
      lines[cursory].insert(cursorx, 4, ' ');
      cursorx += 4;
      ++cc;
      break;
    default:
      oldx = cursorx; oldy = cursory;
      lines[cursory].insert(cursorx, 1, c);
      moveCursorRight();
      undoStack.push_back(Operation{true, oldx, oldy, cursorx, cursory, std::string(1, c)});
      redoStack.clear();
      ++cc;
  }
}
void File::insert(std::string s) {
  size_t oldx = cursorx, oldy = cursory;
  lines[cursory].insert(cursorx, s);
  if (cursorx == len8(lines[cursory]) - s.size()) moveCursorDown();
  else cursorx += s.size();
  undoStack.push_back(Operation{true, oldx, oldy, cursorx, cursory, s});
  redoStack.clear();
  cc += s.size();
}
void File::insertnl(std::string s) {
  size_t oldx = cursorx, oldy = cursory;
  lines.insert(lines.begin() + cursory + 1, s);
  moveCursorDown();
  cursorx = 0;
  undoStack.push_back(Operation{true, oldx, oldy, cursorx, cursory, s, true});
  redoStack.clear();
  ++cc += s.size();
}
void File::newlineblw() {
  size_t oldx = cursorx, oldy = cursory;
  lines.insert(lines.begin() + cursory + 1, "");
  moveCursorDown();
  undoStack.push_back(Operation{true, oldx, oldy, cursorx, cursory, std::string{"\n"}, true});
  redoStack.clear();
}
void File::newlineabv() {
  size_t oldx = cursorx, oldy = cursory;
  lines.insert(lines.begin() + cursory, "");
  undoStack.push_back(Operation{true, oldx, oldy, cursorx, cursory, std::string{"\n"}, true});
  redoStack.clear();
  cursorx = 0;
}

// MISC -----------------------------------------------------------------------

void File::undo() {
  if (undoStack.size()) {
    Operation op = undoStack[undoStack.size() - 1];
    redoStack.push_back(op);
    undoStack.pop_back();
    cursorx = op.oldx; cursory = op.oldy;
    if (op.isInsertion) {
      if (op.isNewline) {
        lines.erase(lines.begin() + op.newy);
      } else {
        lines[cursory].erase(op.newx - op.s.length(), op.s.length());
      }
    } else {
      if (op.isNewline) {
        lines.insert(lines.begin() + cursory, op.s);
      } else {
        lines[cursory].insert(cursorx, op.s);
      }
    }
  }
}

void File::redo() {
  if (redoStack.size()) {
    Operation op = redoStack[redoStack.size() - 1];
    if (op.isInsertion) {
      if (op.isNewline) {
        lines.insert(lines.begin() + op.newy, op.s);
      } else {
        lines[cursory].insert(op.oldx, op.s);
      }
      redoStack.pop_back();
      undoStack.push_back(op);
      cursorx = op.newx;
      cursory = op.newy;
    }
    else {
    // cursorx = op.posx - op.s.length(); cursory = op.posy;
      if (!cursorx) {
        if (cursory) {
          std::string s = lines[cursory];
          moveCursorLeft();
          lines.erase(lines.begin() + cursory + 1);
          lines[cursory] += s;
          }
        } else {
          // moveCursorLeft();
          // undoStack.push_back(Operation{false, oldx, oldy, cursorx, cursory, 
          //   std::string(1, lines[cursory][cursorx])});
          // lines[cursory].erase(cursorx, 1);
        }
        if (cc) --cc;
    }
  }
}

std::string File::yanky() { return lines[cursory]; }
std::string File::yankw() {
  std::string s;
  int c;
  size_t i = cursorx;
  while (i < len8(lines[cursory])) {
    c = lines[cursory][i];
    if (std::ispunct(static_cast<unsigned char>(c))) return s;
    if (std::isspace(static_cast<unsigned char>(c))) break;
    s.push_back(c);
    ++i;
  }
  while (std::isspace(lines[cursory][i])) s.push_back(lines[cursory][i++]);
  return s;
}
std::string File::yankiw() {
  auto x = cursorx;
  moveCursorb();
  std::string s;
  int i = cursorx;
  while (std::isalnum(lines[cursory][i])) s += lines[cursory][i++];
  cursorx = x;
  return s;
}
std::string File::yankaw() {
  auto x = cursorx;
  if (std::isspace(lines[cursory][cursorx])) {
    while (std::isspace(peekb())) moveCursorLeft();
  } else {
    while (std::isalnum(peekb())) moveCursorLeft();
    while (std::isspace(peekb())) moveCursorLeft();
  }
  auto start = cursorx;
  cursorx = x;
  skipwsr();
  while (std::isalnum(lines[cursory][cursorx])) moveCursorRight();
  if (std::isspace(lines[cursory][cursorx])) skipwsr();
  auto end = cursorx;
  cursorx = x;
  return lines[cursory].substr(start, end - start);
}
std::string File::yankend() {
  return lines[cursory].substr(cursorx, lines[cursory].size() - cursorx);
}
std::string File::yankbeg() {
  return lines[cursory].substr(0, cursorx);
}
std::string File::yanktx(char x) {
  auto start = cursorx;
  findforward();
  auto end = cursorx;
  if (start == end) return "";
  cursorx = start;
  return lines[cursory].substr(start, end);
}
std::string File::yankfx(char x) {
  auto end = cursorx;
  findbackward();
  auto start = cursorx;
  if (start == end) return "";
  cursorx = end;
  return lines[cursory].substr(start, end);
}
// void File::changeline() {
//   std::string s = lines[cursory];
//   lines[cursory].clear();
//   cursorx = 0;
//   undoStack.push_back(Operation{false, cursorx, cursory, s});
// }
// void changew() {
//   auto start = cursorx;
//   auto starty = cursory;
//   moveCursorw();
//   if (starty != cursory) auto end = lines[starty].size();
//   else auto end = cursorx;
//   std::string s = lines[starty].substr(start, end);

//   undoStack.push_back(Operation{false, })
// }
void File::changeline() {

}
void File::changew() {
  
}
void File::changeendw() {

}
void File::changeendl() {

}
void File::deleteline() {
  size_t oldx = cursorx, oldy = cursory;
  std::string s = lines[cursory];
  lines.erase(lines.begin() + cursory);
  if (cursory == lines.size()) --cursory;
  cursorx = 0;
  undoStack.push_back(Operation{false, oldx, oldy, cursorx, cursory, s, true});
  redoStack.clear();
}
void File::deleteword() {
  size_t oldx = cursorx, oldy = cursory;
  auto endx = cursorx;
  if (std::ispunct(lines[cursory][endx])) {
    while (std::ispunct(lines[cursory][endx])) ++endx;
  } else if (std::isalnum(lines[cursory][endx])) {
    while (std::isalnum(lines[cursory][endx])) ++endx;
  } else {
    while (std::isspace(lines[cursory][endx])) ++endx;
    while (std::isalnum(lines[cursory][endx])) ++endx;
  }
  while (std::isspace(lines[cursory][endx])) ++endx;
  undoStack.push_back(Operation{false, oldx, oldy, cursorx, cursory,
    lines[cursory].substr(cursorx, endx)});
  redoStack.clear();
  lines[cursory].erase(cursorx, endx);
}
void File::deleteendword() {
  size_t oldx = cursorx, oldy = cursory;
  auto x = cursorx;
  while (std::isspace(lines[cursory][cursorx]) && 
    cursorx < lines[cursory].size()) ++cursorx;
  while (std::isalnum(peekf() &&
    cursorx < lines[cursory].size())) ++cursorx;
  undoStack.push_back(Operation{false, oldx, oldy, x, cursory, 
    lines[cursory].substr(x, cursorx)});
  redoStack.clear();
  lines[cursory].erase(x, cursorx - x);
  cursorx = x;
}
void File::deleteendline() {
  
}

void File::write() {
  std::ofstream f{filename};
  for (const auto &r:lines) {
    std::string s(r.begin(), r.end());
    f << s << std::endl;
  }
}
