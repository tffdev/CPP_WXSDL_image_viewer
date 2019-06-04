#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include <wx/image.h>
#include <iostream>
#include <stdio.h>
#include <string>

enum { ID_FRAME = 10000, ID_PANEL, IDM_FILE_EXIT, IDM_HELP_ABOUT };

class SDLPanel : public wxPanel {
  DECLARE_CLASS(SDLPanel)
  DECLARE_EVENT_TABLE()

 private:
  SDL_Surface* screen;
  SDL_Renderer* renderer;
  SDL_Surface* loadedTexture;

  float scale;

  void OnPaint(wxPaintEvent& event);
  void OnIdle(wxIdleEvent& event);
  void Resize(wxSizeEvent& event);
  void OnScroll(wxMouseEvent& e);
  void InitScreen();
  void Present();
  wxPoint GetMousePosition();

 public:
  void LoadImage(const char* string);
  SDLPanel(wxFrame* parent);
  ~SDLPanel();
};