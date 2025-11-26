export module model;

import <vector>;
import <memory>;

import view;
import controller;
import action;

export class Model {
 public:
  void updateViews();
  void displayViews();
  virtual ~Model() = default;
 
 protected:
  void addView(std::unique_ptr<View> v);
  void addController(std::unique_ptr<Controller> c);
  Action getAction();

 private:
  std::vector<std::unique_ptr<View>> views;
  std::unique_ptr<Controller> control;
};
