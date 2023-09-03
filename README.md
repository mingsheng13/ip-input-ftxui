## Description

A [FTXUI](https://github.com/ArthurSonzogni/FTXUI) component to input IPv4
addresses:
```
╭────────────────┬───────────╮
│Input           │Output     │
├────────────────┼───────────┤
│192.168. 0 . 0  │192.168.0.0│
│255. 0 . 0 .255 │255.0.0.255│
╰────────────────┴───────────╯
```

## API

```cpp
namespace ftxui {
  Component InputIPV4(StringRef content);
}
```

## Example

See [./example](./example).

```cpp
#include <string>
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui-ip-input/ipv4.hpp"

using namespace ftxui;

int main() {
  std::string ipv4 = "0.0.0.0";
  auto input = InputIPV4(&ipv4) | border;
  auto screen = ScreenInteractive::TerminalOutput();
  screen.Loop(input);
}
```

```cmake
cmake_minimum_required (VERSION 3.14)
project(my-app LANGUAGES CXX VERSION 1.0.0)

include(FetchContent)

FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v5.0.0
)

FetchContent_Declare(ftxui-ip-input
  GIT_REPOSITORY https://github.com/mingsheng13/ip-input-ftxui
  GIT_TAG main
)

FetchContent_MakeAvailable(ftxui)
FetchContent_MakeAvailable(ftxui-ip-input)
add_executable(my-app src/main.cpp)
target_link_libraries(my-app
  PRIVATE ftxui-ip-input
)
```
