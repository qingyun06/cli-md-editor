export module mde:previewpane;
import :component;
import :editorpane;
import action;
export class MarkdownEditor;
export class PreviewPane : public Component {
  const EditorPane &e;
  void drawhorizontal(char c);
  void resizepad(int xoffset, int yoffset);
  size_t miny;
  int winminx;
 public:
  PreviewPane(const MarkdownEditor &m, const EditorPane &e);
  void update() override;
  void display() override;
};
