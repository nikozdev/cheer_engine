#include <GL/GL_structs.h>
#include <math/glm/trigonometric.hpp>
#include <GL/GL_Shader.h>

namespace GL
{
	void LightSource::setInShader(const Shader* shader, DirectLight& lSource)
	{
		shader->setVec3("directLight.ambient", lSource.ambient);
		shader->setVec3("directLight.diffuse", lSource.diffuse);
		shader->setVec3("directLight.specular", lSource.specular);
		shader->setVec3("directLight.direction", lSource.direction);
	}

	void LightSource::setInShader(const Shader* shader, PointLight& lSource)
	{
		shader->setVec3("pointLight.ambient", lSource.ambient);
		shader->setVec3("pointLight.diffuse", lSource.diffuse);
		shader->setVec3("pointLight.specular", lSource.specular);

		shader->setVec3("pointLight.position", lSource.position);
		shader->setFloat("pointLight.constK", lSource.att_const);
		shader->setFloat("pointLight.linK", lSource.att_linear);
		shader->setFloat("pointLight.quadK", lSource.att_quad);
	}
	
	void LightSource::setInShader(const Shader* shader, SpotLight& lSource)
	{
		shader->setVec3("spotLight.ambient", lSource.ambient);
		shader->setVec3("spotLight.diffuse", lSource.diffuse);
		shader->setVec3("spotLight.specular", lSource.specular);
		shader->setVec3("spotLight.direction", lSource.direction);
		
		shader->setVec3("spotLight.position", lSource.position);
		shader->setFloat("spotLight.constK", lSource.att_const);
		shader->setFloat("spotLight.linK", lSource.att_linear);
		shader->setFloat("spotLight.quadK", lSource.att_quad);
		
		shader->setFloat("spotLight.cutOffIn", glm::cos(glm::radians(lSource.cutoffIn)));
		shader->setFloat("spotLight.cutOffOut", glm::cos(glm::radians(lSource.cutoffOut)));
	}
}