#include "main.h"

/**
 * Program entry point.
 * Create the simple WxWidgets window.
 * Needs:
 * - Main menu bar (Options to load image)
 * - Panel for SDL viewport
 * - Status bar (current loaded image path)
 */
bool Main::OnInit() {
  window = new wxFrame(NULL, wxID_ANY, "Hello world");

  wxMenu* subMenuFile = new wxMenu();

  subMenuFile->Append(0, "&Open Image...\tCtrl-H",
                      "Help string shown in status bar for this menu item");
  subMenuFile->Append(wxID_EXIT);

  subMenuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, Main::openImageFile, this,
                    ID_OPENIMAGE);

  wxMenuBar* menuBar = new wxMenuBar();
  menuBar->Append(subMenuFile, "&File");

  window->SetMenuBar(menuBar);
  statbar = new wxStatusBar(window, -1);
  window->SetStatusBar(statbar);

  sdl_panel = new SDLPanel(window);

  window->Show();
  return true;
}

/**
 * Load image file to SDL panel.
 * Displays model for image file selection, and
 * if the file exists and is valid, loads the file
 * 
 * TODO: Implement validation for image file.
 */
void Main::openImageFile(wxCommandEvent& e) {
  wxFileDialog openFileDialog(window, "Open file", "", "", "Any file (*.*)|*.*",
                              wxFD_OPEN | wxFD_FILE_MUST_EXIST);
  if (openFileDialog.ShowModal() == wxID_CANCEL)
    return;
  statbar->SetStatusText(openFileDialog.GetPath());
  sdl_panel->LoadImage(openFileDialog.GetPath().c_str());
}

wxIMPLEMENT_APP(Main);