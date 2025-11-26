export module controller;

import action;

export class Controller {
  virtual Action action() = 0;

 public:
  Action getAction();
  virtual ~Controller() = default;
};
