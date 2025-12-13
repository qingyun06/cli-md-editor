export module mde:mde_control;
import <map>;
import mode;
import action;
import controller;
export class MarkdownEditor;
export class CurseKeyboard : public Controller {
 public:
  CurseKeyboard(const MarkdownEditor &m);

 private:
  const MarkdownEditor &m;
  std::map<int, Command> mapping;
  Action action() override;
  Action action(Mode m);
};
