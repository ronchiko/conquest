#include "conquest/engine/ui/component/text/Title.h"


namespace conquest::ui
{

ftxui::Component title(const std::string& text, const ftxui::Color color)
{
    return ftxui::Renderer([=] {
        return ftxui::text(text) | ftxui::bold | ftxui::color(color);
    });
}

}
