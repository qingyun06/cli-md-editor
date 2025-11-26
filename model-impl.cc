module model;

void Model::updateViews() {
  for (const auto &view:views) view->update();
}
void Model::displayViews() {
  for (const auto &view:views) view->display();
}
void Model::addView(std::unique_ptr<View> v) {
  views.emplace_back(std::move(v));
}
void Model::addController(std::unique_ptr<Controller> c) {
  control = std::move(c);
}
Action Model::getAction() {
  return control->getAction();
}
