import mde;
import <iostream>;
int main(int argc, char *argv[]) {
  try {
    MarkdownEditor m{argc, argv};
    m.run();
  } catch (...) {
    std::cerr << "Invalid file format." << std::endl;
    return 0;
  }
}
