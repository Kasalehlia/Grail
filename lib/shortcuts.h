
#ifndef SHORTCUTS_H
#define SHORTCUTS_H

#include <string>
#include <SDL/SDL.h>
#include "resource_manager.h"

SDL_RWops* getRW(std::string path, ResourceMode mode);

Resource getResource(std::string path, ResourceMode mode);

#endif // SHORTCUTS_H

