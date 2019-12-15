#ifndef _TEXTUREMANAGER_H
#define _TEXTUREMANAGER_H

#include <iostream>
#include <string>
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class TextureManager
{
public:
	~TextureManager();

	static TextureManager* Instance();

	bool load(const std::string& fileName, const std::string &id, SDL_Renderer* pRender);
	void draw(std::string id, int x, int y, int width, int height, SDL_Renderer* pRender,
			  SDL_RendererFlip flip = SDL_FLIP_NONE);
	void drawFrame(std::string id, int x, int y, int width, int height, int currentRow, int currentFrame,
				   SDL_Renderer* pRender, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void clearFromTextureMap(const std::string& id);

private:
	TextureManager();
	std::map<std::string, SDL_Texture*> m_textureMap;
	static TextureManager* s_pInstance;
};

#endif