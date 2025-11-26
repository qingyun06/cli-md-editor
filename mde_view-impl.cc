module;
#include <ncurses.h>
#include <locale.h>
module mde;
// preliminary ncurses terminal setup/config
void initialize_screen() {
  // ncurses window setup
  setlocale(LC_ALL, "");
  setlocale(LC_NUMERIC,"C");
  set_escdelay(0);
  initscr(); noecho(); cbreak(); keypad(stdscr, TRUE);
  start_color(); use_default_colors();

  // ncurses text color initialization; -1 means no text background color
  init_pair(COLOR_BLACK, COLOR_BLACK, -1);
  init_pair(COLOR_RED, COLOR_RED, -1);
  init_pair(COLOR_GREEN, COLOR_GREEN, -1);
  init_pair(COLOR_YELLOW, COLOR_YELLOW, -1);
  init_pair(COLOR_BLUE, COLOR_BLUE, -1);
  init_pair(COLOR_MAGENTA, COLOR_MAGENTA, -1);
  init_pair(COLOR_CYAN, COLOR_CYAN, -1);
  init_pair(COLOR_WHITE, COLOR_WHITE, -1);
}
// clears screen and ends ncurses window
void end_screen() {
  clear(); endwin();
}
CurseView::CurseView(const MarkdownEditor &m) {
  initialize_screen();
  components.emplace_back(std::make_unique<Header>(m));
  components.emplace_back(std::make_unique<Footer>(m));
  components.emplace_back(std::make_unique<PreviewPane>(m));
  components.emplace_back(std::make_unique<EditorPane>(m));
  refresh();
}
void CurseView::update() {
  for (const auto &component:components) component->update();
}
void CurseView::display() {
  for (const auto &component:components) component->display();
  // refresh();
}
CurseView::~CurseView() {
  end_screen();
}
