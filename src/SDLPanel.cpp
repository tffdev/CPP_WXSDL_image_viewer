#include "SDLPanel.h"

/**
 * TODO:
 * ~> Safe Rendering API, allow this class to be extended and used
 *    as a generic arbitrary rendering panel?
 * ~> Re-initialization of the renderer is undesirable.
 *    Make it so that on resize, renderer/screen surface doesn't need to be
 *    re-instantiated?
 */

IMPLEMENT_CLASS(SDLPanel, wxPanel)

BEGIN_EVENT_TABLE(SDLPanel, wxPanel)
EVT_PAINT(SDLPanel::OnPaint)
EVT_IDLE(SDLPanel::OnIdle)
EVT_SIZE(SDLPanel::Resize)
EVT_MOUSEWHEEL(SDLPanel::OnScroll)
END_EVENT_TABLE()

SDLPanel::SDLPanel(wxFrame* parent) : wxPanel(parent, 0) {
  SDL_Init(SDL_INIT_VIDEO);
  IMG_Init(IMG_INIT_PNG);
}

/**
 * Destructor destroys SDL allocations in the reverse order 
 * that they were created.
 */
SDLPanel::~SDLPanel() {
  if (loadedTexture) SDL_DestroyTexture(loadedTexture);
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
  /* Repaint + Present the rendered surface */
  if(buffer_screenHasBeenInitialised) {
    Repaint();
    Present();
  }
}

/**
 * Paint the texture onto a screen
 * ~> Added mouse position debug
 * 
 * NOTES:
 * ~> This works well, but if SDL_RenderCopyEx applies any modulations
 *    (flipping, or rotation) then zooming in on an image increases resource
 *    usage dramatically. If we want to support flipping/rotating sprites,
 *    another solution will need to be found.
 */
void SDLPanel::Repaint() {
  /* Clear screen */
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);

  /* paint image */
  if (loadedTexture) {
    int width, height;
    GetSize(&width, &height);

    int tex_width, tex_height;
    SDL_QueryTexture(loadedTexture, NULL, NULL, &tex_width, &tex_height);
    
    SDL_Rect src_rect = {0, 0, tex_width, tex_height};

    int onscreen_width = tex_width * scale;
    int onscreen_height = tex_height * scale;

    SDL_Rect onscreen_rect = {(width / 2) - (onscreen_width / 2),
                              (height / 2) - (onscreen_height / 2),
                              (onscreen_width), (onscreen_height)};

    if(SDL_RenderCopyEx(renderer, loadedTexture, &src_rect, &onscreen_rect, 
      0, NULL, SDL_FLIP_NONE)!=0)
      printf("Render copy error: %s\n", SDL_GetError());
  }

  /* Draw debug mouse position */
  const wxPoint pt = wxGetMousePosition();
  int mouseX = pt.x - this->GetScreenPosition().x;
  int mouseY = pt.y - this->GetScreenPosition().y;

  SDL_Rect mr = {mouseX, mouseY, 10, 10};
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderFillRect(renderer, &mr);
}

/*
 * The 2 lines where the magic happens. (Super basic lol)
 * ~> create a bitmap from our SDL-generated pixel data
 * ~> paint the SDL-generated "bitmap" to the panel
 */
void SDLPanel::Present() {
  wxBitmap screen_bitmap((const char*)(screen->pixels), screen->w, screen->h, 32);
  wxBufferedPaintDC dc(this, screen_bitmap);
}

/*
 * Initialise SDL screen/pseudo-widget.
 * Call this on a window resize, as the old widget must be destroyed in
 * order to reallocate and resize it to the new window size.
 * 
 * TODO: 
 * ~> Can only use SoftwareRenderer to solve this problem. 
 *    Any way to bypass this and use real rendering?
 */
void SDLPanel::InitScreen() {
  printf("Initialising screen\n");
  int width, height;
  GetSize(&width, &height);
  if (width == 0 || height == 0)
    return;

  SDL_DestroyRenderer(renderer);
  SDL_FreeSurface(screen);
  screen = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, 0, 0, 0, 0);
  renderer = SDL_CreateSoftwareRenderer(screen);
  buffer_screenHasBeenInitialised = true;
}

/**
 * Load an image into loadedTexture given an absolute filepath.
 */
void SDLPanel::LoadImage(const char* string) {
  scale = 1;
  if (loadedTexture)
    SDL_DestroyTexture(loadedTexture);
  loadedTexture = IMG_LoadTexture(renderer, string);
  if(loadedTexture)
    printf("Loaded image! (%s)\n", string);
  else
    printf("Couldn't load image: %s\n", SDL_GetError());
}

/**
 * On a resize, re-initialize the renderer with the new window dimensions.
 */
void SDLPanel::Resize(wxSizeEvent& e) {
  buffer_shouldResize = true;
}

void SDLPanel::OnIdle(wxIdleEvent& e) {
  Refresh(false);
  
  if(buffer_shouldResize) { 
    InitScreen();
    buffer_shouldResize = false;
  }

  wxMilliSleep(MS_PER_FRAME);
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