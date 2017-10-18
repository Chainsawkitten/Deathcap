#include "ProfilingWindow.hpp"

#include <Engine/Manager/Managers.hpp>
#include <Engine/Manager/ProfilingManager.hpp>

using namespace GUI;

void ProfilingWindow::Show() {
    Managers().profilingManager->ShowResults();
}
