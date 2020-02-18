#include <Events.h>

void Initialize();
void MainLoop();
void Terminate();

void main() {
    Initialize();

    MainLoop();

    Terminate();
}

void Initialize() {
    InitWindows();
}

void MainLoop() {

    EventRecord event;

    while (true) {
        if (GetNextEvent(everyEvent, &event)) {
            if (event.what == keyDown)
                Terminate();
            }
        }
    }

void Terminate() {
    ExitToShell();
}
