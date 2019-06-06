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

#define FRAME_RATE 120
#define MS_PER_FRAME 1000/FRAME_RATE

class SDLPanel : public wxPanel {
  DECLARE_CLASS(SDLPanel)
  DECLARE_EVENT_TABLE()

 private:
  SDL_Surface* screen;
  SDL_Renderer* renderer;
  SDL_Texture* loadedTexture;
  bool buffer_shouldResize;
  float scale;

  void OnPaint(wxPaintEvent& event);
  void Repaint();
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