export module observer;

import <vector>;

class Event {

};
class Observer;
class Source {
  std::vector<Observer*> subscribers;
 public:
  void subscribe(Observer*);
  void notify(const Event& e);
  virtual ~Source() = 0;
};
class Observer {
 public:
  virtual void onEvent(const Event &e) = 0;
  void registerSelf(Source &s);
};
