export module mde:previewpane;
import :component;
import action;
export class MarkdownEditor;
export class PreviewPane : public Component {
 public:
  PreviewPane(const MarkdownEditor &m);
  void update() override;
  void display() override;
};
