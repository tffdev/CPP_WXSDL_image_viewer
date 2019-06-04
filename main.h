#include <wx/wx.h>
#include <stdio.h>
#include "SDLPanel.h"

class Main : public wxApp {
  wxFrame* window;
  wxStatusBar* statbar;
  SDLPanel* sdl_panel;

  enum { ID_OPENIMAGE };
  bool OnInit();
  void openImageFile(wxCommandEvent& e);
};