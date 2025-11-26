export module mde:footer;
import mode;
import :component;
import action;
export class MarkdownEditor;
export class Footer : public Component {
 public:
  Footer(const MarkdownEditor &m);
  void update() override;
  void display() override;
};
