#include "SDLPanel.h"

/**
 * TODO:
 * Safe Rendering API, allow this class to be extended and used
 * as a generic arbitrary rendering panel.
 * "OnPaint" will be overridden by child classes.
 */
IMPLEMENT_CLASS(SDLPanel, wxPanel)

BEGIN_EVENT_TABLE(SDLPanel, wxPanel)
EVT_PAINT(SDLPanel::OnPaint)
EVT_IDLE(SDLPanel::OnIdle)
EVT_SIZE(SDLPanel::Resize)
EVT_MOUSEWHEEL(SDLPanel::OnScroll)
END_EVENT_TABLE()

SDLPanel::SDLPanel(wxFrame* parent) : wxPanel(parent, 0) {
  IMG_Init(IMG_INIT_PNG);
  InitScreen();
}

/**
 * Destructor destroys SDL allocations in the reverse order 
 * that they were created.
 */
SDLPanel::~SDLPanel() {
  if (loadedTexture)
    SDL_FreeSurface(loadedTexture);
  SDL_FreeSurface(screen);
  SDL_DestroyRenderer(renderer);
}

wxPoint SDLPanel::GetMousePosition() {
  const wxPoint pt = wxGetMousePosition();
  int mouseX = pt.x - this->GetScreenPosition().x;
  int mouseY = pt.y - this->GetScreenPosition().y;
  return wxPoint(mouseX, mouseY);
}

/**
 * Refresh/Draw function. Clears the background (white) and
 * draws the appropriately-scaled image in the center of the canvas.
 */
void SDLPanel::OnPaint(wxPaintEvent&) {
  // Clear screen
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);

  // paint image
  if (loadedTexture) {
    int width, height;
    GetSize(&width, &height);

    SDL_Rect src_rect = {0, 0, loadedTexture->w, loadedTexture->h};

    int onscreen_width = loadedTexture->w * scale;
    int onscreen_height = loadedTexture->h * scale;

    SDL_Rect onscreen_rect = {(width / 2) - (onscreen_width / 2),
                              (height / 2) - (onscreen_height / 2),
                              (onscreen_width), (onscreen_height)};

    SDL_BlitScaled(loadedTexture, &src_rect, screen, &onscreen_rect);
  }

  // Present the rendered surface
  Present();
}

/*
 * The 2 lines where the magic happens. (Super basic lol)
 * ~> create a bitmap from our SDL-generated pixel data
 * ~> paint the SDL-generated "bitmap" to the panel
 */
void SDLPanel::Present() {
  wxBitmap screen_bitmap((const char*)(screen->pixels), screen->w,
                         screen->h, 32);
  wxBufferedPaintDC dc(this, screen_bitmap);
}

/*
 * Initialise SDL screen/pseudo-widget.
 * Call this on a window resize, as the old widget must be destroyed in
 * order to reallocate and resize it to the new window size.
 * 
 * TODO: Can only use SoftwareRenderer to solve this problem. 
 * Any way to bypass this and use real rendering?
 */
void SDLPanel::InitScreen() {
  int width, height;
  GetSize(&width, &height);
  if (width == 0 || height == 0)
    return;

  SDL_FreeSurface(screen);
  screen = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, 0, 0, 0, 0);
  renderer = SDL_CreateSoftwareRenderer(screen);
}

/*
 * Load an image into loadedTexture given an absolute filepath.
 */
void SDLPanel::LoadImage(const char* string) {
  scale = 1;
  if (loadedTexture)
    SDL_FreeSurface(loadedTexture);
  loadedTexture = IMG_Load(string);
  if(loadedTexture)
    printf("Loaded image!\n");
  else
    printf("Couldn't load image.\n");
}

/**
 * On a resize, re-initialize the renderer with the new window dimensions.
 */
void SDLPanel::Resize(wxSizeEvent& e) {
  InitScreen();
}

void SDLPanel::OnIdle(wxIdleEvent& e) {
  Refresh(false);
  wxMilliSleep(33);
}

/**
 * Scroll to zoom an image in and out. 
 * Scaling is currently nearest-neighbor and doesn't look too great.
 * Combination of mulplicitive and additive factors make scaling feel more natural.
 */
void SDLPanel::OnScroll(wxMouseEvent& e) {
  int move = e.GetWheelRotation();
  if (move > 0)
    scale *= 1.1 + 0.1;
  if (move < 0)
    scale *= 0.9 - 0.1;
  if (scale < 0.01)
    scale = 0.01;
}