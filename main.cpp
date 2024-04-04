#include <iostream>
#include <thread>

void gui()
{
    // Logger{} << "game\n";
}

int main()
{
    std::thread gui_thread(gui);
    // Logger{} << "super\n";

    return 0;
}