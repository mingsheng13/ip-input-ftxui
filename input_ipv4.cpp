#include <sstream>
#include <string>
#include <vector>

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/dom/elements.hpp"
#include "input_ipv4.hpp"

namespace ftxui {
namespace {

// Options for the input of one component of the IP address.
InputOption ByteInputOption() {
  InputOption style = InputOption::Spacious();
  style.multiline = false;
  style.transform = [](InputState state) {
    if (state.is_placeholder) {
      state.element = text(" 0 ");
    }

    state.element |= center;
    state.element |= size(WIDTH, EQUAL, 4);

    if (state.focused) {
      state.element |= inverted;
    } else if (state.hovered) {
      state.element |= bgcolor(Color::GrayDark);
    }

    return state.element;
  };
  return style;
}

// Returns true if the event represent a number.
bool IsNumber(const Event& event) {
  if (!event.is_character()) {
    return false;
  }

  if (event.character() != "0" && event.character() != "1" &&
      event.character() != "2" && event.character() != "3" &&
      event.character() != "4" && event.character() != "5" &&
      event.character() != "6" && event.character() != "7" &&
      event.character() != "8" && event.character() != "9") {
    return false;
  }

  return true;
}

// A component handling a single byte of the IP address.
Component InputIPV4Byte(ComponentBase* parent, StringRef byte_content) {
  class Impl : public ComponentBase {
   private:
    ComponentBase* parent_;
    StringRef content_;
    Component input = Input(content_, ByteInputOption());

    Element Render() override {
      RegularizeByte();
      return ComponentBase::Render() | notflex;
    }

    bool OnEventInternal(Event event) {
      if (!IsNumber(event)) {
        return ComponentBase::OnEvent(event);
      }

      const bool handled = ComponentBase::OnEvent(event);
      if (content_().size() >= 3) {
        parent_->OnEvent(Event::ArrowRight);
      }
      return handled;
    }

    bool OnEvent(Event event) override {
      RegularizeByte();
      const bool handled = OnEventInternal(event);
      RegularizeByte();
      return handled;
    }

    void RegularizeByte() {
      int value = std::atoi(content_().c_str());
      if (value <= 0) {
        content_() = "";
      }
      if (value >= 255) {
        content_() = "255";
      }
    }

   public:
    Impl(ComponentBase* parent, StringRef content)
        : parent_(parent), content_(std::move(content)) {
      Add(input);
    }
  };

  return Make<Impl>(parent, byte_content);
}
}  // namespace

// A component handling the full IP address.
Component InputIPV4(StringRef content) {
  class Impl : public ComponentBase {
    private:
     StringRef content_;
     std::string bytes[4];
     Component input[4] = {
        InputIPV4Byte(this, &bytes[0]),
        InputIPV4Byte(this, &bytes[1]),
        InputIPV4Byte(this, &bytes[2]),
        InputIPV4Byte(this, &bytes[3]),
     };

    Element Render() override {
      SynchronizeDownward();
      return dbox({
          ComponentBase::Render(),
          hbox({
              emptyElement() | size(WIDTH, EQUAL, 3),
              text(".") | clear_under,
              emptyElement() | size(WIDTH, EQUAL, 3),
              text(".") | clear_under,
              emptyElement() | size(WIDTH, EQUAL, 3),
              text(".") | clear_under,
              emptyElement() | size(WIDTH, EQUAL, 3),
          }),
      });
    }

    bool OnEvent(Event event) override {
      const bool handled = OnEventInternal(event);
      SynchronizeUpward();
      return handled;
    }

    bool OnEventInternal(const Event& event) {
      // Special case for the '.' character. It is used to move to the next
      // byte.
      if (event == Event::Character('.')) {
        return OnEvent(Event::ArrowRight);
      }
      
      // Special case for the backspace character. If we can't use backspace on
      // the current byte, we move to the previous byte.
      if (event == Event::Backspace) {
        if (ComponentBase::OnEvent(event)) {
          return true;
        }
        return ComponentBase::OnEvent(Event::ArrowLeft);
      }

      // Special case for the number keyrs. If we can't add more characters to
      // the current byte, we move to the next byte and try again.
      if (IsNumber(event)) {
        while(!ComponentBase::OnEvent(event)) {
          if (!ComponentBase::OnEvent(Event::ArrowRight)) {
            return false;
          }
        }
        return true;
      }
      
      return ComponentBase::OnEvent(event);
    }
    
    // Update the input bytes from the output string.
    void SynchronizeDownward() {
      std::stringstream ss(content_());
      std::getline(ss, bytes[0], '.');
      std::getline(ss, bytes[1], '.');
      std::getline(ss, bytes[2], '.');
      std::getline(ss, bytes[3], '.');
    }

    // Update the output string from the input bytes.
    void SynchronizeUpward() {
      std::stringstream ss;
      auto f = [](const std::string& s) { return s.empty() ? "0" : s; };
      ss << f(bytes[0]) << "."  //
         << f(bytes[1]) << "."  //
         << f(bytes[2]) << "."  //
         << f(bytes[3]);
      content_() = ss.str();
    }

   public:
    Impl(StringRef content) : content_(std::move(content)) {
      Add(Container::Horizontal({
          input[0],
          input[1],
          input[2],
          input[3],
      }));
    }
  };

  return Make<Impl>(content);
}

}  // namespace ftxui
