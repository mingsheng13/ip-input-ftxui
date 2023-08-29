#include <string>

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

#include "input_ipv4.hpp"
using namespace ftxui;

// example
int main() {
  std::string ipv4 = "0.0.0.0";
  std::string mask = "255.0.0.255";

  auto ipInput = InputIPV4(&ipv4);
  auto maskInput = InputIPV4(&mask);

  auto container = Container::Vertical({
      ipInput,
      maskInput,
  });

  auto renderer = Renderer(container, [&] {
    return vbox({
        text("Enter ip:    "),
        ipInput->Render() | border,
        text("Enter mask:  "),
        maskInput->Render() | border,
        window(text("output"), vbox({
                                   text(ipv4),
                                   text(mask),
                               })),
    });
  });
  auto screen = ScreenInteractive::TerminalOutput();
  screen.Loop(renderer);
}
