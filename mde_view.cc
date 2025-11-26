export module mde:mde_view;
import <vector>;
import <memory>;
import view;
import :component;
import :header;
import :footer;
import :editorpane;
import :previewpane;
export class MarkdownEditor;
export class CurseView : public View {
 public:
  CurseView(const MarkdownEditor &m);
  void update() override;
  void display() override;
  void resize();
  ~CurseView();

 private:
  std::vector<std::unique_ptr<Component>> components;
};
