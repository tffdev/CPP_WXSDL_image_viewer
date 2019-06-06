#pragma once
#include <wx/wx.h>
#include <stdio.h>
#include "SDLPanel.h"

class MainWindow : public wxFrame {
  wxStatusBar* statbar;
  SDLPanel* sdl_panel;
  enum { ID_OPENIMAGE };
  
public:
  MainWindow(const wxString& title);
  ~MainWindow();
  bool OnInit();
  void AddMenu();
  void OpenImageFile(wxCommandEvent& e);
};