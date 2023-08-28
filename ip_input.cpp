#include <sstream>
#include <vector>
#include <string>

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"

#include "ip_input.h"

namespace ftxui
{
    bool isNumber(const std::string& character)
    {
        const char* charArr = character.c_str();
        while (*charArr != 0)
        {
            if (*charArr < '0' || *charArr > '9')
            {
                return false;
            }
            charArr++;  // NOLINT
        }
        return true;
    }

    std::vector<std::string> split(const std::string& content)
    {
        std::vector<std::string> output;
        std::stringstream ss(content);
        std::string line;
        while (std::getline(ss, line, '.'))
        {
            output.push_back(line);
        }
        return output;
    }

    Component IpInput(StringRef content)
    {
        class Impl : public ComponentBase
        {
        public:
            // NOLINTNEXTLINE
            Impl(StringRef content) : content_(std::move(content))
            {
                input_ = Input(content_, "___.___.___.___");
                Add(input_);
            }

            Element Render() override
            {
                return input_->Render();
            }

            bool OnEvent(Event event) override
            {
                if (event.is_character())
                {
                    std::vector<std::string> parts = split(*content_);
                    parts.reserve(4);

                    //ipv4 maximum length
                    if (parts[3].size() == 3)
                    {
                        return false;
                    }

                    // when user input '.' manually
                    if (event.character() == ".")
                    {
                        if (dotsCounter >= 3)
                        {
                            return false;
                        }
                        if (content_->back() == '.')
                        {
                            return false;
                        }
                        if (content_->empty())
                        {
                            return false;
                        }
                        dots[dotsCounter++] = true;
                        return ComponentBase::OnEvent(event);
                    }

                    for (int i = 0; i < 3; ++i)
                    {
                        if (parts[i].size() == 3 && !dots[i])
                        {
                            if (event.character() != ".")
                            {
                                return false;
                            }
                        }
                        // add '.' automatically after every 3 character
                        if (parts[i].size() == 2 && !dots[i])
                        {
                            dots[dotsCounter++] = true;
                            ComponentBase::OnEvent(event);
                            return ComponentBase::OnEvent(Event::Character('.'));
                        }
                    }
                    if (!isNumber(event.character()))
                    {
                        return false;
                    }
                }

                if (event == Event::Backspace)
                {
                    if (content_->back() == '.')
                    {
                        dots[--dotsCounter] = false;
                        std::clamp(dotsCounter, 0, 3);
                        return ComponentBase::OnEvent(event);
                    }
                }
                return ComponentBase::OnEvent(event);
            }

            bool Focusable() const override
            {
                return true;
            }

            std::vector<bool> dots = {false, false, false};
            int dotsCounter = 0;
            StringRef content_;
            Component input_;
        };

        return Make<Impl>(content);
    }

} //namespace ftxui