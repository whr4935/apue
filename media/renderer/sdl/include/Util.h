#ifndef _UTIL_H
#define _UTIL_H

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int TILE_SIZE = 40;

void logSDLError(std::ostream &os, const std::string &msg);
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);
void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, SDL_Rect dst, SDL_Rect *clip = nullptr);
void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y, SDL_Rect *clip = nullptr);
SDL_Texture* renderText(const std::string &message, const std::string &fontFile, SDL_Color color,
						int fontSize, SDL_Renderer *render);


#endif
