WX/SDL Image Viewer
====================
Proof-of-concept image viewer demo for a basic interface between WxWidgets and SDL.
Currently only supports software rendering (very slow if images are modulated through rotation or flipping).

Dependencies:
* SDL2.0
* SDL_Image
* wxwidgets

Runtime binaries needed: 
(all found in dev packages for SDL2, SDL_Image and wxwidgets)
* libjpeg-9.dll
* libpng16-16.dll                    
* mingwm10.dll
* SDL2.dll
* SDL2_image.dll
* wxbase312u_gcc510TDM_x64.dll
* wxmsw312u_core_gcc510TDM_x64.dll
* zlib1.dll (dependency for libpng16)
