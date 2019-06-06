#include "main.h"

/**
 * Application entry point.
 * Create the main window.
 */
bool Main::OnInit() {
  MainWindow* mainwin = new MainWindow("Hello world");
  mainwin->Show(true);
  return true;
}

wxIMPLEMENT_APP(Main);