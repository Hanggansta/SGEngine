#ifndef _SHADER_MANAGER_H
#define _SHADER_MANAGER_H

#include "SGUtil.h"
#include "Shader.h"
#include "Vector.h"
#include "External/glm/gtc/type_ptr.hpp"

namespace SGEngine
{
class SGMaterialManager
{
	friend class SGMaterial;
private:
  enum ShaderType
  {
    SHADER_VERTEX = 0,
    SHADER_FRAGMENT
  };

  struct AttributeVariable
  {
    AttributeVariable(Shader_SemanticDataType eType, SG_UINT uLocationIn, int size);

    SG_UINT _uLocation;
    Shader_SemanticDataType _eType;    
	int _iSize;
  };

  struct UniformVariable
  {
    UniformVariable(Shader_UniformDataType, SG_UINT uLocation);

    SG_UINT _uLocation;
    Shader_UniformDataType _eType;
  };
  /* Usage:
  * To access the stored variables in the shader
  * User sends the Shader_Semantic ex : Shader_Semantic::Position and the program
    returns the attributes mapped to that particular semantic in the program
  * Multiple Postion semantics can be given as : POSITION0 , POSITION1.....
  * Filled after parsing the shader file and verifying the supplied ShaderInfo by
    the shader class   */
  typedef std::unordered_map<Shader_Semantic, AttributeVariable> MapAttributes;
  typedef std::unordered_map<Shader_Uniform, UniformVariable> MapUniforms;

  struct ProgramBlob
  {
    ProgramBlob();
    SG_UINT _uId;                         // Shader Program Object blob ID
    std::array<unsigned int, 2> _aShaderIndex; //Index at which the shader blobs are loaded in the info shader array of vectors

    MapAttributes _mapAttributes;
    MapUniforms _mapUniform;
  };

  struct ShaderBlob
  {
    ShaderBlob(int shaderId, std::string strShaderFilename);

    SG_UINT uId; // Shader Data Blob ID
    std::string strFilename;
  };

  typedef std::shared_ptr<ProgramBlob> SPTR_ProgramBlob;
  typedef std::shared_ptr<ShaderBlob> SPTR_ShaderBlob;
  typedef std::map<long int, SPTR_ProgramBlob> MAP_ProgramBlob;
  typedef std::vector<SPTR_ShaderBlob> VECTOR_ShaderBlob;

public:

  ~SGMaterialManager();

private:
  SGMaterialManager() {}

  SGMaterialManager(const SGMaterialManager &obj);
  SGMaterialManager &operator=(const SGMaterialManager &c);

  static SGMaterialManager &instance();
  void Create(const long int& material_uuid, Shader &shader);
  void EnableProgram(const long int& material_uuid);
  void EnableAttribute(const long int& material_uuid, Shader_Semantic semantic_name, SG_UINT strideBytes = 0, SG_UINT offsetBytes = 0, bool normalize = false) const;
  void DisableAttribute(const long int& material_uuid, Shader_Semantic semantic_name) const;
  template<typename t_value_type>
  void SetUniform(const long int& material_uuid, const Shader_Uniform uniform_name, const t_value_type& t_data);
  void SetUniform(const long int& material_uuid, const char* uniform_name, const int& value);
  
  //Functions
  SG_UINT InitializeShader(const ShaderType &shaderType, const std::string &filename);
  void ProcessAttributes(const SG_UINT programID, std::string shaderName, Shader::Vector_ShaderAttributeInfo &sai, MapAttributes &mAttributes);
  void ProcessUniforms(const SG_UINT programID, std::string shaderName, Shader::Vector_ShaderUniformInfo &sui, MapUniforms &mUniforms);
  template <typename T>
  void SetVertexAttribute(const long int& material_uuid, Shader_Semantic semantic, const T *const pParam, bool bEnable, SG_UINT strideBytes, SG_UINT offsetBytes, bool normalize) const;
  void SetVertexAttribute(AttributeVariable attrib, const void *const pVoid, bool bEnable, SG_UINT strideBytes, SG_UINT offsetBytes, bool normalize) const;
  void SetUniform(const UniformVariable uniform,const glm::mat4 &matrix) const;
  void SetUniform(const UniformVariable uniform,const SGVector4 &value) const;
  void SetUniform(const UniformVariable uniform,const SGVector3 &value) const;
  void SetUniform(const UniformVariable uniform, const SGVector2 &value) const;
  void SetUniform(const UniformVariable uniform,const int &value) const;
  //Data
  long int activeMaterial;

  /* Loaded Program Objects stored as map<shader.name,ProgramBlob> 
  * Contains programID
  * Index(in the vector list) of the shaders linked to this program object
  * Map of attributes <SemanticType,AttributeVariable(ShaderVariable_Type,Location)>
  * Map of uniforms   <UniformType ,UniformVariable(UniformVariable_type,Location)>
  */
  MAP_ProgramBlob _map_ProgramBlob;

  /* List of all loaded shader in memory
  * Branching into array of vectors -> size = 2
  * array[0] -> Vector_list -> Vertex Shaders loaded
  * array[1] -> Vector_List -> Pixel Shaders loaded */
  std::array<VECTOR_ShaderBlob, 2> _avShaderBlob;
};

#include "ShaderManager.inl"
} // namespace SGEngine

#endif