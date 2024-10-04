#ifndef __RESOURCE_MANAGER_HPP__
#define __RESOURCE_MANAGER_HPP__

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <string>
#include <unordered_map>

class ResourceManager{
	public:
		static ResourceManager *Instance(){
			if(mInstance == nullptr){
				mInstance = new ResourceManager();
			}
			return mInstance;
		}

        void releaseResources() {
            for(auto &tex : mTextures) {
                glDeleteTextures(1, &tex.second);
            } 
        }

        unsigned int setTexture(const std::string &path, bool flip = false);

        unsigned int getTexture(const std::string &path);

		static void destroy() {
			delete ResourceManager::Instance();
		}

	private:

		ResourceManager() {
            mTextures.clear();
			
		};

		static ResourceManager* mInstance;
		std::unordered_map<std::string, unsigned int> mTextures;
};
typedef ResourceManager _ResourceManager;

#endif