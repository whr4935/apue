#include "TextureManager.h"
#include "Game.h"



TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
	for (auto &obj : m_textureMap) {
		SDL_DestroyTexture(obj.second);
	}
	m_textureMap.clear();
}

TextureManager* TextureManager::s_pInstance = nullptr;

TextureManager * TextureManager::Instance()
{
	if (s_pInstance == nullptr) {
		s_pInstance = new TextureManager();
	}

	return s_pInstance;
}

bool TextureManager::load(const std::string& fileName, const std::string &id, SDL_Renderer * pRender)
{
	SDL_Texture* tex = IMG_LoadTexture(pRender, fileName.c_str());
	if (tex == nullptr)
		return false;

	m_textureMap[id] = tex;
	return true;
}

void TextureManager::draw(std::string id, int x, int y, int width, int height, SDL_Renderer* pRender, SDL_RendererFlip flip /* = SDL_FLIP_NONE */)
{
	SDL_Rect srcRect;
	SDL_Rect destRect;

	srcRect.x = 0;
	srcRect.y = 0;
	destRect.w = srcRect.w = width;
	destRect.h = srcRect.h = height;
	destRect.x = x % SCREEN_WIDTH;
	destRect.y = y %SCREEN_HEIGHT;

	SDL_RenderCopyEx(pRender, m_textureMap[id], &srcRect, &destRect, 0, 0, flip);
}

void TextureManager::drawFrame(std::string id, int x, int y, int width, int height, int currentRow, int currentFrame, SDL_Renderer* pRender, SDL_RendererFlip flip /* = SDL_FLIP_NONE */)
{
	SDL_Rect srcRect;
	SDL_Rect destRect;

	srcRect.x = width * (currentFrame % 4);
	srcRect.y = height * (currentRow - 1) ;
	destRect.w = srcRect.w = width;
	destRect.h = srcRect.h = height;
	destRect.x = x % SCREEN_WIDTH;
	destRect.y = y % SCREEN_HEIGHT;

	SDL_RenderCopyEx(pRender, m_textureMap[id], &srcRect, &destRect, 0, 0, flip);
}

void TextureManager::clearFromTextureMap(const std::string& id)
{
	auto it = m_textureMap.find(id);
	if (it != m_textureMap.end()) {
		SDL_DestroyTexture(it->second);
	}
	 m_textureMap.erase(id);
}
