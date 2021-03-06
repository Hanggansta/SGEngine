#ifndef _SHADER_H
#define _SHADER_H

#include "SGUtil.h"
#include <External/GL/glew.h>
#include <External/glm/glm.hpp>

namespace SGEngine
{
/* Variable types present inside the shader file , GLSL types (vec3,vec4,vec2)*/
enum Shader_SemanticDataType
{
  VT_FLOAT_VEC2 = GL_FLOAT_VEC2,
  VT_FLOAT_VEC3 = GL_FLOAT_VEC3,
  VT_FLOAT_VEC4 = GL_FLOAT_VEC4
};
/* Variables of type uniform present inside the shade file*/
enum Shader_UniformDataType
{
  UT_FLOAT_MAT4 = GL_FLOAT_MAT4,
  UT_FLOAT_VEC4 = GL_FLOAT_VEC4,
  UT_FLOAT_VEC3 = GL_FLOAT_VEC3,
  UT_FLOAT_VEC2 = GL_FLOAT_VEC2,
  UT_SAMPLER2D = GL_SAMPLER_2D,
  UT_BOOL = GL_BOOL
};
/*Forward declaration by the application*/
enum class Shader_Semantic : SG_UINT;

enum class Shader_Uniform : SG_UINT;

template <typename variable, typename type>
class ShaderVariable
{
  friend class SGMaterialManager;
  friend class SGMaterial;
public:
  ShaderVariable();
  ShaderVariable(variable, type);
  SG_INLINE bool operator==(const ShaderVariable<variable, type> &inValue);
  ShaderVariable<variable, type> &operator=(const ShaderVariable<variable, type> &v);

private:
  //Prevent Copying

  variable _variable;
  type _type;
};

typedef ShaderVariable<Shader_Semantic, Shader_SemanticDataType> ShaderAttribute;
typedef ShaderVariable<Shader_Uniform, Shader_UniformDataType> ShaderUniform;

class Shader
{
  friend class SGMaterialManager;
  friend class SGMaterial;

public:
  Shader(const char *shaderName, const char *vs_file, const char *fs_file);
  ~Shader();

  template <typename variable, typename type>
  void AddVariable(const ShaderVariable<variable, type> &_variable, std::string variableName, int size);

private:
  /* VariableInfo : structure to specify shader inside variables
* Contents:
  * ShaderVariable :  Class to store Semantics with datatype(GLSL) present in 
                        the shader file
  * std:<string> :    variable name as presnt inside the shader file  
  * isVerified :      Verifies the specified shaderVariable with the shader file supplied*/
  template <typename variable, typename type>
  struct VariableInfo
  {
    ShaderVariable<variable, type> _shaderVariable;

    std::string _strName; // ACtual name of the variable inside the shader file
    bool _isVerified;
    int _dataSize;			// unique value count per variable , ex : vec4 = 4

    VariableInfo(ShaderVariable<variable, type> v, std::string variableName, int size);
  };

  //Prevent Copying of object
  Shader(const Shader &copy);
  Shader &operator=(const Shader &copy);

  typedef VariableInfo<Shader_Semantic, Shader_SemanticDataType> ShaderAttributeInfo;
  typedef VariableInfo<Shader_Uniform, Shader_UniformDataType> ShaderUniformInfo;
  /* Vector list to store all the shader's variable info , will sent to the 
      ShaderManager for storage and verification*/
  typedef std::vector<ShaderAttributeInfo> Vector_ShaderAttributeInfo;
  using Vector_ShaderUniformInfo = std::vector<ShaderUniformInfo>;

  Vector_ShaderAttributeInfo &getVector(const ShaderAttribute &attribute)
  {
    return vector_sai;
  }
  Vector_ShaderUniformInfo &getVector(const ShaderUniform &uniform)
  {
    return vector_sui;
  }

  std::string _vertex_shader_file;
  std::string _fragment_shader_file;
  std::string shaderProgramName; // MUST BE UNIQUE THROGHOUT THE LIFE OF APPLICATION

  Vector_ShaderAttributeInfo vector_sai;
  Vector_ShaderUniformInfo vector_sui;
};

#include "ShaderInfo.inl"

} // namespace SGEngine
#endif