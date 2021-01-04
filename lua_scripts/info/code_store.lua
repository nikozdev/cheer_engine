int CheerEngine::cpp_scaleRotateMove(lua_State* L)
{//cpp_scaleRotateMove(index, tabSc, tabRt, tabPs)
	glm::vec3 vSc = glm::vec3(1.0f);
	glm::vec3 vRt = glm::vec3(0.0f);
	glm::vec3 vPs = glm::vec3(0.0f);
	unsigned short index = 0;
	if (!lua_istable(L, -1))
	{
		printf("CHEER_ENGINE::CPP_SCALE_ROTATE_MOVE: Can't load transformation-vectors.\n");
		return 0;
	}
	else if (lua_isinteger(L, -4))
	{
		index = static_cast<unsigned short>(lua_tointeger(L, -4));
		L::L_state::tabToVec(L, vPs);
		L::L_state::tabToVec(L, vRt);
		L::L_state::tabToVec(L, vSc);
	}
	else if (lua_isinteger(L, -3))
	{
		index = static_cast<unsigned short>(lua_tointeger(L, -3));
		L::L_state::tabToVec(L, vPs);
		L::L_state::tabToVec(L, vRt);
	}
	else if (lua_isinteger(L, -2))
	{
		index = static_cast<unsigned short>(lua_tointeger(L, -2));
		L::L_state::tabToVec(L, vPs);
	}
	lua_getglobal(L, CH_ENGINE);
	auto chEngine = static_cast<CheerEngine*>(lua_touserdata(L, -1));
	lua_pop(L, lua_gettop(L));
	if (chEngine->meshes[index])
	{
		chEngine->meshes[index]->setScale(vSc);
		chEngine->meshes[index]->setRotation(vRt);
		chEngine->meshes[index]->setPosition(vPs);
	}
	return 0;
}

//Loading and settings of scene
int CheerEngine::cpp_loadMesh(lua_State* L)
{//cpp_setMesh("path", index) or cpp_setMesh(vertexArray, index)
	lua_getglobal(L, CH_ENGINE);
	CheerEngine* chEngine = (CheerEngine*)lua_touserdata(L, -1);
	lua_pop(L, 1);

	std::string name = "";
	unsigned short index = 0;

	if (lua_isstring(L, -2) || lua_isstring(L, -1))
	{//cpp_setMesh(meshName, index) || cpp_setMesh(meshName);
		if (lua_isstring(L, -2))
		{
			name = static_cast<std::string>(lua_tostring(L, -2));
			index = static_cast<unsigned short>(lua_tointeger(L, -1));
		}
		else if (lua_isstring(L, -1))
		{
			name = static_cast<std::string>(lua_tostring(L, -1));
			index = 0;
		}

		if (chEngine->meshes.size() > index && name != "clean")
		{//We have enough vaos and we don't need to clean anything
			if (chEngine->meshes.at(index) != nullptr)
			{
				printf("CHEER_ENGINE::CPP_SET_MESH: can't set mesh, %d-th mesh is already filled.\n", index);
				return 0;
			}
		}

		if (name == "clean")
		{
			if (chEngine->meshes[index] != nullptr)
			{
				delete chEngine->meshes[index];
				chEngine->meshes[index] = nullptr;
			}
			return 0;
		}
		else
		{//We have gotten a path as a name argument
			chEngine->setMesh(name, index);
		}
	}
	else if (lua_istable(L, -2) || lua_istable(L, -1))
	{//cpp_setMesh(vertexTable, index) || cpp_setMesh(vertexTable);
		std::vector<float> vData = chEngine->states.top()->getData<std::vector<float>>(name);
		//GL::VertexArr va();
		//GL::VertexBuf vb(&vData, vData.size());
	}
	return 0;
}
/// Change default mesh material on new material in cash.
/// Change only material pointer. Mesh's default stays still
/// NOTE::10.10.2020::original material gets replaced. Fix it
int CheerEngine::cpp_loadMaterial(lua_State* L)
{//cpp_loadMaterial(indexOfMesh, name, diffPath, specPath, normPath)
	lua_getglobal(L, CH_ENGINE);
	CheerEngine* chEngine = (CheerEngine*)lua_touserdata(L, -1);
	lua_pop(L, 1);

	std::string name = "";
	short mesh_index = 0;
	short amount = 4;

	if (lua_isinteger(L, -5) && lua_isstring(L, -4))
	{
		mesh_index = (short)lua_tointeger(L, -5);
		name = (std::string)lua_tostring(L, -4);
		amount = 3;
	}
	else if (lua_isinteger(L, -4) && lua_isstring(L, -3))
	{
		mesh_index = (short)lua_tointeger(L, -4);
		name = (std::string)lua_tostring(L, -3);
		amount = 2;
	}
	else if (lua_isinteger(L, -3))
	{
		mesh_index = (short)lua_tointeger(L, -3);
		name = (std::string)lua_tostring(L, -2);
		amount = 1;
	}
	else
	{
		printf("CHEER_ENGINE::LOAD_MATERIAL: Function calling doesn't match the form\n{cpp_loadMaterial(int mesh_index, string name, string paths[4])}");
		return 0;
	}

	if (name == "clean")
	{
		if (chEngine->meshes[mesh_index])
		{
			chEngine->meshes[mesh_index]->setMaterialPtr(nullptr);
			return 0;
		}
	}
	if (chEngine->meshes[mesh_index] == NULL)
	{
		printf("CHEER_ENGINE::CPP_SET_MATERIAL: The mesh at index %d doesn't exist.\n", mesh_index);
		return 0;
	}
	if (chEngine->meshes[mesh_index]->getMaterialPtr())
	{
		printf("CHEER_ENGINE::CPP_SET_MATERIAL: Material %s of the mesh at index %d is already set.\n",
			name.c_str(), mesh_index);
		return 0;
	}
	//Set new materials
	std::string maps[3] = { "", "", ""};
	for (int i = 0; i < amount; i++)
	{
		maps[i] = lua_tostring(L, -amount + i);
	}
	chEngine->meshes[mesh_index]->setMaterialPtr(new GL::Material(name.c_str(),
		maps[0].c_str(), maps[1].c_str(), maps[2].c_str()));
	return 0;
}


//Get path of executable file with windows.h
TCHAR dest[MAX_PATH];
GetThisPath(dest, MAX_PATH);


//LuaManager example(Singleton)
	static get function;


	lua_State* L;

	char memoryBytes[L::ArenaAllocator::POOL_SIZE];
	L::ArenaAllocator pool(memoryBytes, &memoryBytes[L::ArenaAllocator::POOL_SIZE - 1]);

else if (true)
	{//With own memory pool
		//Memory will be allocated here but not in the heap
		L = lua_newstate(L::ArenaAllocator::l_alloc, &pool);
		luaL_openlibs(L);
	}

	struct Entity
	{
		std::string name;
		int x, y;
		Entity() : name("Ent"), x(0), y(0)
		{
			//
		}
		~Entity()
		{
			//
		}
	};

	struct EntityManager
	{
		int numOfEnts = 0;
		std::vector<Entity*> entities;

		void lookAfterEnt(Entity* ent)
		{
			numOfEnts++;
			entities.push_back(ent);
		}
		void forgetEnt(Entity* ent)
		{
			numOfEnts--;
			for (int i = 0; i < entities.size(); i++)
			{
				if (entities.at(i) == ent)
				{
					entities.erase(entities.begin() + i);
				}
			}
		}
	};

	EntityManager entManager;

	auto createEntity = [](lua_State* L) -> int
	{
		EntityManager* entityManager = (EntityManager*)lua_touserdata(L, lua_upvalueindex(1));
		void* entPtr = lua_newuserdata(L, sizeof(Entity));
		new (entPtr) Entity();
		entityManager->lookAfterEnt((Entity*)entPtr);
		luaL_getmetatable(L, "meta_Entity");
		assert(lua_istable(L, -1));
		lua_setmetatable(L, -2);

		lua_newtable(L);
		lua_setuservalue(L, 1);

		return 1;
	};
	
	auto destroyEntity = [](lua_State* L) -> int
	{
		EntityManager* entityManager = (EntityManager*)lua_touserdata(L, lua_upvalueindex(1));
		void* entPtr = lua_touserdata(L, -1);
		entityManager->forgetEnt((Entity*)entPtr);
		
		return 0;
	};
	if (true)
	{
		lua_newtable(L);
		int entTabInd = lua_gettop(L);
		lua_pushvalue(L, entTabInd);
		lua_setglobal(L, "Entity");

		constexpr int UPVALUES_NR = 1;
		lua_pushlightuserdata(L, &entManager);
		lua_pushcclosure(L, createEntity, UPVALUES_NR);
		lua_setfield(L, -2, "New");
		
		luaL_newmetatable(L, "meta_Entity");
		lua_pushstring(L, "__gc");
		lua_pushcfunction(L, destroyEntity);
	}

	constexpr const char* lua_script = R"(
ent0 = Entity:New()
ent0 = nil
ent1 = Entity:New()
ent1 = nil
)";
	pool.printMemoryUsage();


	lua_close(L);



//Lua Arena memory allocation
	else if (false)
	{//With global memory pool
		L::LuaAllocator globalAllocator;
		L = lua_newstate(L::LuaAllocator::l_alloc, &globalAllocator);
		SP::MemoryAllocator::get().printMemoryUsage();
		L::State::regLibs(libs, L);
		//Counts memory usage
		SP::MemoryAllocator::get().printMemoryUsage();
		lua_close(L);
		SP::MemoryAllocator::get().printMemoryUsage();
	}


//Model class
	struct Model
	{
	private:
		//Model data
		std::vector<Mesh> meshes;
		std::vector<Texture> loadedTex;
		//Location of all relevant data
		std::string directory;
		//Gamma
		bool gammaCorrection;
		//Functions
		void loadModel(std::string path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> loadMatTex(aiMaterial* mat,
			aiTextureType type, std::string typeName);
		GLuint texFromFile(const char* path,
			const std::string& directory, bool gamma);
	public:
		//Constructors&Destructors
		Model(std::string path, bool gamma);
		~Model();
		//Functions
		void drawModel(Shader& shader);
	};

	//Model
Model::Model(std::string path, bool gamma)
{
	gammaCorrection = gamma;
	loadModel(path);
	printf("GL_STRUCTS::MODEL::CONSTRUCTOR: Model %s is created.\n",
		directory.c_str());
}

GL::Model::~Model()
{
	printf("GL_STRUCTS::MODEL::DESTRUCTOR: Model %s is destroyed.\n",
		directory.c_str());
}

void Model::drawModel(Shader& shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{//Draw every mesh
		this->meshes.at(i).draw(shader);
	}
}

void Model::loadModel(std::string path)
{//Get all general data: scene, nodes and meshes in nodes
	Assimp::Importer imp;
	const aiScene* scene = imp.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags && AI_SCENE_FLAGS_INCOMPLETE ||
		!scene->mRootNode)
	{
		printf("GL_STRUCTS::MODEL::LOAD_MODEL: Error %s\n", imp.GetErrorString());
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{//process all meshes and all nodes one by one with this function recursevely
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{//Get meshes from mesh array of node
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{//recursion for hierarchy
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{//Create new vertex and fill it's data
		Vertex vertex;
		glm::vec3 vec = glm::vec3(0.0f);
		vec.x = mesh->mVertices[i].x;
		vec.y = mesh->mVertices[i].y;
		vec.z = mesh->mVertices[i].z;
		vertex.position = vec;
		if (mesh->HasNormals())
		{
			vec.x = mesh->mNormals[i].x;
			vec.y = mesh->mNormals[i].y;
			vec.z = mesh->mNormals[i].z;
			vertex.normal = vec;
		}
		if (mesh->mTextureCoords[0])
		{//Is there any coordinates? - add them
			glm::vec2 texCoord = glm::vec2(0.0f);
			texCoord.x = mesh->mTextureCoords[0][i].x;
			texCoord.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = texCoord;
		}//Or just set zero
		else { vertex.texCoords = glm::vec2(0.0f); }
		//Build mesh of all vertices
		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{//Indicies are in faces
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	if (mesh->mMaterialIndex >= 0)
	{//This mesh has any materials? - load them
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		//Diffuse maps
		std::vector<Texture> diffuseMaps = loadMatTex(material,
			aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		//Specular maps
		std::vector<Texture> specularMaps = loadMatTex(material,
			aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		//Normal maps
		std::vector<Texture> normalMaps = loadMatTex(material,
			aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		//Heihgt maps
		std::vector<Texture> heightMaps = loadMatTex(material,
			aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	}

	return Mesh(vertices, indices, textures);
}
std::vector<Texture> Model::loadMatTex(aiMaterial* mat,
	aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{//Get texture info of every material texture in aiString
		aiString aiStr;
		mat->GetTexture(type, i, &aiStr);
		bool skip = false;
		for (unsigned int j = 0; j < loadedTex.size(); j++)
		{//Check all loaded textures whether we loaded such or not
			if (std::strcmp(loadedTex[j].path.data(), aiStr.C_Str()) == 0)
			{//This texture is loaded already - skip it
				textures.push_back(loadedTex[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{//We still haven't loaded it
			//std::string filename = (std::string)aiStr.C_Str();
			//filename = directory + '/' + filename; //Absolute path
			//Load new texture
			Texture texture(typeName);
			texture.id = texFromFile(aiStr.C_Str(), directory, false);
			texture.path = aiStr.C_Str();
			textures.push_back(texture); //Add to resulting texture vector
			loadedTex.push_back(texture); //Now this texture is loaded
		}
	}
	return textures;
}

GLuint Model::texFromFile(const char* path,
	const std::string& directory, bool gamma)
{//Get project directory, path of the file and load
	GLuint textureID;
	glGenTextures(1, &textureID);
	
	std::string filename = (std::string)path;
	filename = directory + '/' + filename; //Absolute path
	printf("MODEL::TEX_FROM_FILE: load file: %s\n", filename.c_str());
	
	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(),
		&width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;
		else
			format = GL_RG;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0,
			format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		printf("MODEL::TEX_FROM_FILE: couldn't load texture.\n");
		stbi_image_free(data);
	}

	return textureID;
}

//Standard Indexing OpenGL
	GLuint VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesRect1.size(),
		verticesRect1.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		sizeof(unsigned int) * idxs1.size(),
		idxs1.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
{Rendering
		glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}

//File reading
if(ifstream.is_open())
{
	while(std::getline(ifstream, temp))
		src += temp + "\n";
}
else printerror;
ifstream.close();

//TextureLoad
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textures[2]);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, textures[3]);

	glm::vec3 emisGlow(sin(currentFrame)/2, cos(currentFrame) / 2, sin(currentFrame));
	
	dLightPos.x = cos(currentFrame / 3) * 20.0f;
	dLightPos.y = sin(currentFrame / 3) * 20.0f;
	dLightDir = glm::normalize(-dLightPos);

	pLightPos.x = cos(currentFrame / 2) * 10.0f;
	pLightPos.z = sin(currentFrame / 2) * 10.0f;
	//ObjectsRender
	objShader->use();
	//Set material properites to uniform struct
	objShader->setInt("material.tex_diffuse0", 0);
	objShader->setInt("material.tex_specular0", 1);
	objShader->setInt("material.tex_emission0", 2);
	objShader->setInt("material.add", 3);
	objShader->setVec3("emisGlow", emisGlow);
	objShader->setFloat("material.shininness", shininness);
	//dirLight struct
	glm::vec3 lDiff = dlDiffColor * dlColor;
	glm::vec3 lAmb = dlAmbColor * dlColor;
	objShader->setVec3("dLight.ambient", dlAmbColor);
	objShader->setVec3("dLight.diffuse", dlDiffColor);
	objShader->setVec3("dLight.specular", dlSpecColor);

	objShader->setVec3("dLight.direction", dLightDir);
	//PointLightStruct
	lDiff = plDiffColor * plColor;
	lAmb = plAmbColor * plColor;
	objShader->setVec3("pLight.ambient", plAmbColor);
	objShader->setVec3("pLight.diffuse", plDiffColor);
	objShader->setVec3("pLight.specular", plSpecColor);

	objShader->setVec3("pLight.position", pLightPos);

	objShader->setFloat("pLight.constK", attK.x);
	objShader->setFloat("pLight.linK", attK.y);
	objShader->setFloat("pLight.quadK", attK.z);
	//SpotLightStruct
	lDiff = slDiffColor * slColor;
	lAmb = slAmbColor * slColor;
	objShader->setVec3("sLight.ambient", slAmbColor);
	objShader->setVec3("sLight.diffuse", slDiffColor);
	objShader->setVec3("sLight.specular", slSpecColor);

	sLightPos = cam->Position;
	sLightDir = cam->Front;
	objShader->setVec3("sLight.position", sLightPos);
	objShader->setVec3("sLight.direction", sLightDir);

	objShader->setFloat("sLight.constK", attK.x);
	objShader->setFloat("sLight.linK", attK.y);
	objShader->setFloat("sLight.quadK", attK.z);
	
	objShader->setFloat("sLight.cutOffIn", cutOffIn);
	objShader->setFloat("sLight.cutOffOut", cutOffOut);
	//Translate and draw
	viewMat = glm::mat4(1.0f);
	projMat = glm::mat4(1.0f);

	viewMat = cam->getLookAtMatrix();
	projMat = glm::perspective(cam->Zoom,
		(float)scrWidth / (float)scrHeight, 0.1f, 100.0f);
	//Set matricies for 5 coordinate systems
	objShader->setMat4("view", viewMat);
	objShader->setMat4("proj", projMat);

	for (int i = 0; i < 5; i++)
	{
		modelMat = glm::mat4(1.0f);
		modelMat = glm::translate(modelMat, glm::vec3(glm::pow(-i, i+1)/5, glm::pow(-i, i+1)/5, 0.0f - i * 2));
		modelMat = glm::rotate(modelMat, glm::radians(20.0f * i), glm::vec3(0.0f, 1.0f, 0.0f));
		objShader->setMat4("model", modelMat);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}
	//LightSourceRender
	lShader->use();
	lShader->setVec3("lightColor", plColor);
	
	states.top()->render();

	modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat, pLightPos);
	modelMat = glm::scale(modelMat, glm::vec3(0.1f, 0.1f, 0.1f));

	lShader->setMat4("model", modelMat);
	lShader->setMat4("view", viewMat);
	lShader->setMat4("proj", projMat);

	glBindVertexArray(VAOs[2]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	//Directional light
	lShader->setVec3("lightColor", dlColor);

	modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat, dLightPos);
	modelMat = glm::scale(modelMat, glm::vec3(3.0f, 3.0f, 3.0f));

	lShader->setMat4("model", modelMat);

	glBindVertexArray(VAOs[2]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
