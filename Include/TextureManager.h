#ifndef _TEXTURE_MANAGER_H
#define _TEXTURE_MANAGER_H

#include"SGUtil.h"
#include "External/GL/glew.h"
#include "External/stb_image.h"

namespace SGEngine
{
	enum class TextureFiltering : int
	{
		TF_LINEAR = GL_LINEAR,
		TF_NEAREST = GL_NEAREST
	};

	enum class TextureWrapMode : int
	{
		WRAP_REPEAT = GL_REPEAT,
		WRAP_CLAMP_EDGE = GL_CLAMP_TO_EDGE,
		WRAP_CLAMP_BORDER = GL_CLAMP_TO_BORDER
	};
	/* SGTexture:
		* Texture Object
		* Handles all the texture properties supported by the engine*/
	class SGTexture
	{
		friend class SGTextureManager;
	public:
		struct PConfig
		{
			TextureFiltering magFilter, minFilter;
			TextureWrapMode wrapMode;
			bool flipY = false;
		};

	public:
		SGTexture(const char* texturename,const char* filename, const PConfig& config ,const bool& mipMap = false);
		SGTexture(const char* texturename , const char* filename , const bool& mipMap = false);
		void SetParameters(const PConfig& config);   // TODO :  RE - GENERATE TEXTURES HERE!!!
		~SGTexture();

	private:
		const char* mapName;
		const char* textureFile;
		PConfig configParams;
	};

	/*Texture Manager :
		* Loads (GL_Calls) and stores all the textures in memory.
		* Handles Binding of the textures for the current draw call*/
    class SGTextureManager
    {
		friend class SGTexture;
		friend class SGMaterial;

        private:
			SGTextureManager(){}
        ~SGTextureManager(){}

		static SGTextureManager& instance();
		void GenerateTexture(const SGTexture& textureObject, const bool& EnableMipMapLevels = false);
		bool BindTexture(const char* map_name, const SG_UCHAR index);
		void UnBindTexture();

        std::unordered_map<std::string,SG_UINT> _map_tBufferObjects;
    };
}

#endif