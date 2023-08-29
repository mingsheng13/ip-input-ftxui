#include <string>

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

#include "ip_input.h"
using namespace ftxui;

// example
int main() {
  std::string ipv4;
  std::string mask;

  auto ipInput = IpInput(&ipv4);
  auto maskInput = IpInput(&mask);

  auto container = Container::Vertical({
      ipInput,
      maskInput,
  });

  auto renderer = Renderer(container, [&] {
    return vbox({
        text("Enter ip:    "),
        ipInput->Render(),
        text("Enter mask:  "),
        maskInput->Render(),
        window(text("output"), vbox(
                                   {
                                       text(ipv4),
                                       text(mask),
                                   }, )),
    });
  });
  auto screen = ScreenInteractive::TerminalOutput();
  screen.Loop(renderer);
}
