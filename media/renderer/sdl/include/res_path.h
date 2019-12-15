#ifndef _RES_PATH_H
#define	_RES_PATH_H
#include <iostream>
#include <string>
#include <SDL2/SDL.h>

std::string getResourcePath(const std::string& subDir = "")
{
#ifdef _WIN32
	const char PATH_SEP = '\\';
#else
	const char PATH_SEP = '/';
#endif
	static std::string baseRes;

#ifdef _DEBUG
	const char *find_dir = "Debug";
#else
	const char *find_dir = "Release";
#endif

	if (baseRes.empty()) {
		char *basePath = SDL_GetBasePath();
		if (basePath) {
			baseRes = basePath;
			SDL_free(basePath);
		} else {
			std::cout << "Error getting resource path: " << SDL_GetError() << std::endl;
			return "";
		}
		size_t pos = baseRes.rfind(find_dir);
		baseRes = baseRes.substr(0, pos) + "SDL/res" + PATH_SEP;
	}
	
	return subDir.empty() ? baseRes : baseRes + subDir + PATH_SEP;
}


#endif
