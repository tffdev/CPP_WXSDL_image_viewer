#include "MainWindow.h"

/**
 * Main window display.
 * TODO:
 * ~> Main menu bar (Options to load image)
 * ~> Panel for SDL viewport
 * ~> Status bar (current loaded image path)
 */
MainWindow::MainWindow(const wxString& title) : wxFrame(NULL, wxID_ANY, title) {
  AddMenu();

  statbar = new wxStatusBar(this, -1);
  this->SetStatusBar(statbar);

  sdl_panel = new SDLPanel(this);
}

MainWindow::~MainWindow() {}

void MainWindow::AddMenu() {
  wxMenuBar* menuBar = new wxMenuBar();
  wxMenu* subMenuFile = new wxMenu();

  subMenuFile->Append(0, "&Open Image...\tCtrl-H",
                    "Help string shown in status bar for this menu item");
  subMenuFile->Append(wxID_EXIT);

  subMenuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, MainWindow::OpenImageFile, this,
                    ID_OPENIMAGE);
  menuBar->Append(subMenuFile, "&File");

  this->SetMenuBar(menuBar);
}

/**
 * Load image file to SDL panel.
 * Displays model for image file selection, and
 * if the file exists and is valid, loads the file
 * 
 * TODO: 
 * ~> Implement validation for image file.
 */
void MainWindow::OpenImageFile(wxCommandEvent& e) {
  wxFileDialog openFileDialog(this, "Open file", "", "", 
                              "Valid Image Files (*.png;*.jpg;*.gif)|*.png;*.jpg;*.gif",
                              wxFD_OPEN | wxFD_FILE_MUST_EXIST);

  if (openFileDialog.ShowModal() == wxID_CANCEL)
    return;
  
  statbar->SetStatusText(openFileDialog.GetPath());
  sdl_panel->LoadImage(openFileDialog.GetPath().c_str());
}