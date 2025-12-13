export module mde:header;
import <string>;
import :component;
import action;
class MarkdownEditor;
export class Header : public Component {
 public:
  Header(const MarkdownEditor &m);
  void update() override;
  void display() override;
};
