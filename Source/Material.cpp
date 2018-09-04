#include "Material.h"
#include "ShaderManager.h"
#include "Mesh.h"
#include "UUIDGenerator.h"
#include "TextureManager.h"

namespace SGEngine
{
	SGMaterial::SGMaterial(std::string material_name)
	{
		name = material_name;
		SGUUIDGenerator::instance().Create(uuid);
		activeShader = nullptr;
	}

	SGMaterial::~SGMaterial()
	{

	}

	void SGMaterial::BindVAO(const SG_UINT& vao) const
	{
		glBindVertexArray(vao);
	}

	void SGMaterial::BindTexture()
	{
		auto it = _map_textures.begin();

		for( ; it!= _map_textures.end(); it++)
		{
			bool success = SGTextureManager::instance().BindTexture(it->second, it->first);
			if (!success)
			{
				std::cout << "Create the Texture before binding : " << uuid << std::endl << it->second;
			}
		}

	}

	void SGMaterial::UnBindVAO() const
	{
		glBindVertexArray(0);
	}

	void SGMaterial::BuildVAO(const SG_UINT& vao) const
	{
		this->BindVAO(vao);
		SG_UINT offsetBytes = 0;
		SG_UINT prevDataSize = 0;
		for (Shader::ShaderAttributeInfo attribInfo : activeShader->vector_sai)
		{
			offsetBytes =  prevDataSize  * sizeof(float);
			std::cout << "**********************************************************************\n";
			std::cout << attribInfo._strName <<" : "<<offsetBytes << " : " << sizeof(SGVertex) <<"\n";
			std::cout << "**********************************************************************\n";
			SGMaterialManager::instance().EnableAttribute(uuid,attribInfo._shaderVariable._variable
				, sizeof(SGVertex), offsetBytes, false);
			prevDataSize += attribInfo._dataSize;
		}
		this->UnBindVAO();
	}

	void SGMaterial::SetShader(SG_PTRS<Shader> const shader)
	{
		activeShader = shader;
		std::cout << "Material name : " << name << " id : " << uuid << "Active Shader : "<<activeShader->shaderProgramName <<std::endl;
		// Generate Shader data
		SGMaterialManager::instance().Create(uuid, *activeShader);
	}

	void SGMaterial::SetTexture(const char* name , const SG_UCHAR index)
	{
		_map_textures.insert(std::pair<SG_UCHAR, const char*>(index , name));
	}

	void SGMaterial::RenderBegin(const SG_UINT& vao)
	{
		SGMaterialManager::instance().EnableProgram(this->GetUUID());
		this->BindTexture();
		this->BindVAO(vao);
	}

	void SGMaterial::RenderEnd()
	{
		this->UnBindVAO();
		this->BindTexture();
	}
}