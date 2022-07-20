#include "Editor.h"

static constexpr const unsigned int SCR_WIDTH = 1600;
static constexpr const unsigned int SCR_HEIGHT = 900;

int main(void)
{
    Editor* editor = new Editor();
    editor->Run();
    delete editor;
    return 0;
}
