export module view;

export class View {
 public:
 virtual void update() = 0;
 virtual void display() = 0;
 virtual ~View() = default;
};
