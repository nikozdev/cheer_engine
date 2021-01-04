#include <Support/ch_LoadMaster.h>
#include <Support/defer.h>
#include <Support/stb_image.h>

#include <GL/GL_GraphicsObj.h>

#include <framework.hpp>

namespace SP
{
	int LoadMaster::impModels = 0;
	int LoadMaster::impTextures = 0;
	// Load Master
	// Constructors&Destructors
	LoadMaster::LoadMaster()
	{
		printf("SP::LOAD_MASTER::CONSTRUCTOR: Load Master has been created.\n");
	}

	LoadMaster::~LoadMaster()
	{
		printf("SP::LOAD_MASTER::DESTRUCTOR: Load Master has been destroyed.\n");
	}

	bool LoadMaster::saveIt_bin(const std::string& fileName, void* objectPtr, size_t sizeInBytes)
	{
		std::ofstream saver;
		saver.open(fileName, std::ofstream::app);
		if (!saver.is_open())
		{
			saver.close();
			return false;
		}
		saver.write((char*)objectPtr, sizeInBytes);

		saver.close();
		return true;
	}

	bool LoadMaster::loadIt_bin(const std::string& fileName, void* objectPtr, size_t sizeInBytes)
	{
		std::ifstream loader;
		loader.open(fileName, std::ofstream::in);
		if (!loader.is_open())
		{
			loader.close();
			return false;
		}

		while (loader.read((char*)objectPtr, sizeInBytes));

		loader.close();
		return true;
	}

	bool LoadMaster::loadMesh_def(const std::string& path, GL::Mesh* mesh)
	{
		if (path.find(".obj") != std::string::npos)
			return LoadMaster::loadMesh_obj(path.c_str(), mesh);
		else if (path.find(".dae") != std::string::npos)
			return LoadMaster::loadMesh_dae(path.c_str(), mesh);
		return false;
	}

	bool LoadMaster::loadMesh_obj(const char* path, GL::Mesh* mesh)
	{
		std::vector <glm::vec3>positions;
		std::vector <glm::vec3>normals;
		std::vector <glm::vec2>UVs;
		std::vector <unsigned int> indices;

		std::fstream fileObj;
		std::string line;
		float temp[4] = { 0, 0, 0, 0 };
		//[0] = positions, [1] = normals, [2] = uvs, [3] = indices, [4] = min
		int counter[5] = { 0, 0, 0, 0, 0 };
		char char1, char2;

		fileObj.open(path, std::ios::in);
		if (fileObj.is_open())
		{
			printf("LOAD_MASTER::LOAD_MESH_OBJ: %s reading...\n", path);
			while (!fileObj.eof())
			{//Read before it is ended
				std::getline(fileObj, line, '\n');
				if (line.find("#END") != -1)
					break;
				//std::cout << line.c_str();
				//Get two first symbols to find the type of readed info
				char1 = line.c_str()[0];
				char2 = line.c_str()[1];
				switch (char1)
				{
				case 'g':
					printf("Object %s is found.\n", line.c_str());
				case 'v':
					switch (char2)
					{
						printf("|Vertex data| : reading...\n");
					case ' ':
						if (sscanf(line.c_str(), "v %f %f %f ", &temp[0], &temp[1], &temp[2]) == 3)
						{
							positions.push_back(glm::vec3(temp[0] / 100, temp[1] / 100, temp[2] / 100));
							counter[0]++;
						}
						break;
					case 'n':
						if (sscanf(line.c_str(), "vn %f %f %f ", &temp[0], &temp[1], &temp[2]) == 3)
						{
							normals.push_back(glm::vec3(temp[0], temp[1], temp[2]));
							counter[1]++;
						}
						break;
					case 't':
						if (sscanf(line.c_str(), "vt %f %f", &temp[0], &temp[1]) == 2)
						{
							UVs.push_back(glm::vec2(temp[0], temp[1]));
							counter[2]++;
						}
						break;
					default:
						break;
					}
					break;
				case 'f':
					if (sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d",
						&temp[0], &temp[0], &temp[0],
						&temp[1], &temp[1], &temp[1],
						&temp[2], &temp[2], &temp[2]) == 9)
					{
						indices.push_back(temp[0]);
						indices.push_back(temp[1]);
						indices.push_back(temp[2]);
						counter[3] += 3;
					}
					break;
				default:
					break;
				}
			}
		}
		fileObj.close();

		if (!positions.empty())
		{
			counter[4] = glm::min(counter[0], counter[1]);
			counter[4] = glm::min(counter[4], counter[1]);
			counter[4] = glm::min(counter[4], counter[2]);
			mesh->m_directory = path;
			mesh->updateVAO(positions.data(), counter[4], indices.data(), indices.size(),
				normals.data(), UVs.data());
			printf("LOAD_MASTER::LOAD_MODEL_OBJ: model %s is loaded.\n", path);
			printf("Vertex coords: %d; Normal coords: %d; Texture coords: %d;\nIndices: %d; Vertices: %d\n",
				counter[0], counter[1], counter[2], counter[3], counter[4]);
			impModels++;
			return true;
		}
		else
			return false;
	}

	bool LoadMaster::loadMesh_dae(const char* path, GL::Mesh* mesh)
	{
		//if the <triangles> section doesn't have normals (input semantic=NORMALS),it doesn't work
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uvs;
		std::vector<unsigned int> indices;

		std::ifstream daeFile;
		std::string line;

		//[0] = positions, [1] = normals, [2] = uvs, [3] = indices, [4] = min
		int counter[5] = { 0, 0, 0, 0, 0 };
		short max = 0, i = 0, indexNr = 0;
		char* c;
		float temp[4] = { 0, 0, 0, 0 };

		daeFile.open(path);
		if (daeFile.is_open())
		{
			printf("LOAD_MASTER::LOAD_MESH_DAE: %s reading...\n", path);
			while (getline(daeFile, line))
			{//Write every line of model file. Find headers
				if (line.find("float_array id=") != -1 || line.find("float_array count=") != -1)
				{//<float_array id = "" count = "">\n vertex Data \n</float_array>
					std::printf("FOUND %s\n", line.c_str());
					if (line.find("POSITION") != -1 || line.find("Position") != -1 || line.find("positions-array") != -1)
					{//<float_array id ="...-Position-..." count ="x">
						printf("|Position array| : reading...\n");
						do {//Write next line and do it while there is no </>
							getline(daeFile, line);
							if (sscanf(line.c_str(), "%f %f %f ", &temp[0], &temp[1], &temp[2]) == 3)
							{//sscanf scanes string of first arg with given format in second
								//and writes it to next variables returning amount of written
								positions.push_back(glm::vec3(temp[0], temp[1], temp[2]));
								counter[0]++;
							}
							if (line.find("</float_array>") != -1)
							{//We have nothing left to do now
								printf("|Position array| : has been read\n");
							}
						} while (line.find("</float_array>"));
					}//Positions float array is done
					else if (line.find("Normal") != -1 || line.find("normals-array") != -1)
					{//<float_array id ="...-Normal-..." count ="x">
						printf("|Normal array| : reading...\n");
						do {//Read all section
							getline(daeFile, line);
							if (sscanf(line.c_str(), "%f %f %f ", &temp[0], &temp[1], &temp[2]) == 3)
							{//Scan every dimention and write it to data
								normals.push_back(glm::vec3(temp[0], temp[1], temp[2]));
								counter[1]++;
							}
							if (line.find("</float_array>") != -1)
							{//Normals are read
								printf("|Normal array| : has been read\n");
							}
						} while (line.find("</float_array>"));
					}
					else if (line.find("UV") != -1)
					{//<float_array id ="...-UV-..." count ="x">
						printf("|UV array| : reading...\n");
						do {
							std::getline(daeFile, line);
							if (sscanf(line.c_str(), "%f %f", &temp[0], &temp[1]) == 2)
							{
								uvs.push_back(glm::vec2(temp[0], temp[1]));
								counter[2]++;
							}
							if (line.find("</float_array>") != -1)
							{//UV's array is read
								printf("|Normal array| : has been read\n");
							}
						} while (line.find("</float_array>"));
					}
				}
				else if ((line.find("<triangles") != -1) || (line.find("<polylist")))
				{//<triangles count ="x" material="mat">
					if (line.find("VERTEX") != -1)
					{//<input semantic = "VERTEX" offset=0 source="#cuore2_Mesh-VERTEX"/>
						printf("TRIANGLES:VERTEX\n %d MAX\n", max);  max++;
					}
					//<input semantic = "VERTEX" offset=0 source="#cuore2_Mesh-VERTEX"/>
					if (line.find("NORMAL") != -1)
					{//<input semantic = "NORMAL" offset=1 source="#cuore2_Mesh-Normal0"/>
						max++;
						printf("TRIANGLES:NORMAL\n %d MAX\n", max);
					}
					if (line.find("TEXCOORD") != -1)
					{//<input semantic = "TEXCOORD" offset=2 source="#cuore2_Mesh-UV0"/>
						max++;
						printf("TRIANGLES:TEXCOORD\n %d MAX\n", max);
					}
					if (line.find("COLOR") != -1)
					{//<input semantic = "TEXCOORD" offset=2 source="#cuore2_Mesh-UV0"/>
						max++;
						printf("TRIANGLES:COLOR\n%d MAX\n", max);
					}
					if (line.find("<p>") != -1)
					{
						std::printf("INDICES\n");
						counter[4] = glm::min(counter[0], counter[1]);
						counter[4] = glm::min(counter[4], counter[1]);
						counter[4] = glm::min(counter[4], counter[2]);
						//s2 = all content of s1 after <p>
						/*std::string line2;
						line2 = line.substr(line.find("<p>") + 3, line.length());
						//Get every next lexem in c before < is found
						c = strtok((char*)line2.c_str(), " <");
						if (!c == NULL)
						{
							indices.push_back(atoi(c));
						}
						i++;

						while (c = strtok(NULL, " <"))
						{//Get all tokens
							line2 = c;
							if (line.find("/p>") != -1)
							{//Stop it
								break;
							}
							else
							{//Read further
								if (i == 0)
								{//Add vertex index
									indexNr++;
									indices.push_back(atoi(c));
									counter[3]++;
									i++;
									continue;
								}
								if (i == 3 && i <= max)
								{//
									i = 0;
								}
								if (i == 2 && i <= max)
								{//Check uvs indices
									if (mesh->uvs[indices[indexNr]].x > 0 ||
										(mesh->uvs[indices[indexNr]].y > 0))
									{
										mesh->uvs[indices[indexNr]] + uvs[atoi(c)];
									}
									else
									{
										mesh->uvs[indices[indexNr]] =
											mesh->uvs[indices[indexNr]] + uvs[atoi(c)];
										counter[3]++;
									}
									i++;
								}
								if (i == 1 && i <= max)
								{//Check normal indices
									if (max == 2)
									{
										mesh->uvs[indices[indexNr]] = uvs[atoi(c)];
										i = 0;
									}
									if (mesh->normals[indices[indexNr]].x > 0 ||
										(mesh->normals[indices[indexNr]].y > 0) ||
										(mesh->normals[indices[indexNr]].z > 0))
									{
										mesh->normals[indices[indexNr]] =
											(mesh->normals[indices[indexNr]] + normals[atoi(c)])/ glm::vec3(2);
									}
									else
									{
										mesh->normals[indices[indexNr]] =
											(mesh->normals[indices[indexNr]] + normals[atoi(c)]);
									}
									if (mesh->normals[indices[indexNr]].x > 1 ||
										(mesh->normals[indices[indexNr]].y > 1) ||
										(mesh->normals[indices[indexNr]].z > 1))
									{
										printf("TOO NORMAL\n");
									}
									i++;
								}
								if (i >= max)
								{//Check normals again
									i = 0;
								}
							}
						}
						*/
					}//</p> is found
				}//Triangles are read
			}
			printf("|Triangles| : has been read\n");
		}
		else
		{
			printf("LOAD_MASTER::LOAD_MESH_DAE: Unable to load mesh by path %s.\n", path);
			return false;
		}
		if (!positions.empty())
		{
			mesh->m_directory = path;
			mesh->updateVAO(positions.data(), counter[4], indices.data(), indices.size(),
				normals.data(), uvs.data());
			printf("LOAD_MASTER::LOAD_MODEL_DAE: model %s is loaded.\n", path);
			printf("Vertex coords: %d; Normal coords: %d; Texture coords: %d;\nIndices: %d.\n",
				counter[0], counter[1], counter[2], counter[3]);
			impModels++;
			return true;
		}
		else
			return false;
	}

	bool LoadMaster::loadTexture_def(const std::string& path, GL::Texture* tex)
	{
		std::string format = path.substr(path.rfind("."), 4);
		if (path.find(".dds") != -1)
			return LoadMaster::loadTexture_dds(path.c_str(), tex);
		else
			return LoadMaster::loadTexture_stbi(path.c_str(), tex);
	}

	bool LoadMaster::loadTexture_stbi(const char* path, GL::Texture* tex)
	{
		if (tex->getId() != 0)
		{
			printf("LOAD_MASTER::LOAD_TEXTURE_PNG: Texture %s is already set.\n", path);
			return false;
		}
		unsigned char* data = stbi_load(path,
			&tex->width, &tex->height, &tex->channelsNr, 0);
		if (data)
		{
			GLenum format;
			if (tex->channelsNr == 1)
				format = GL_RED;
			else if (tex->channelsNr == 3)
				format = GL_RGB;
			else if (tex->channelsNr == 4)
				format = GL_RGBA;
			GL::Texture::bind(tex);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTexImage2D(GL_TEXTURE_2D, 0, format, tex->width, tex->height, 0,
				format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			tex->unbind();

			stbi_image_free(data);
			printf("LOAD_MASTER::LOAD_TEXTURE_PNG: Texture %s is loaded.\n", path);
			printf("Width: %d; Height: %d; Channels: %d; Total: %d\n",
				tex->width, tex->height, tex->channelsNr, impTextures);
			impTextures++;
			return true;
		}
		else
		{
			std::cerr << "LOAD_MASTER::LOAD_TEXTURE_PNG: Texture failed to load at path: " << path << std::endl;
			stbi_image_free(data);
			return false;
		}
	}

#ifdef _WIN32
#include <windows.h>
	bool LoadMaster::loadTexture_dds(char const* fname, GL::Texture* tex)
	{
		tex->path = fname;
		HANDLE hFile = CreateFile((LPCWSTR)(fname), GENERIC_READ, 0,
			nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			std::cout << "LOAD_MASTER::LOAD_TEXTURE_DDS: CreateFile failed, fname = " << fname << std::endl;
			return false;
		}
		defer(CloseHandle(hFile));

		DWORD dwFileSize = GetFileSize(hFile, nullptr);
		if (dwFileSize == INVALID_FILE_SIZE)
		{
			std::cout << "LOAD_MASTER::LOAD_TEXTURE_DDS: - GetFileSize failed, fname =  " << fname << std::endl;
			return false;
		}

		HANDLE hMapping = CreateFileMapping(hFile, nullptr, PAGE_READONLY, 0, 0, nullptr);
		if (hMapping == nullptr) { // yes, NULL, not INVALID_HANDLE_VALUE, see MSDN
			std::cout << "LOAD_MASTER::LOAD_TEXTURE_DDS: CreateFileMapping failed, fname =  " << fname << std::endl;
			return false;
		}
		defer(CloseHandle(hMapping));

		unsigned char* dataPtr = (unsigned char*)MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, dwFileSize);
		if (dataPtr == nullptr) {
			std::cout << "LOAD_MASTER::LOAD_TEXTURE_DDS: MapViewOfFile failed, fname =  " <<
				fname << std::endl;
			return false;
		}
		defer(UnmapViewOfFile(dataPtr));

		impTextures++;
		return loadTexture_dds_common(fname, dataPtr, tex, (size_t)dwFileSize);
	}

	bool LoadMaster::loadTexture_dds_common(const char* fname,
		unsigned char* dataPtr, GL::Texture* tex, size_t fsize)
	{
		if (fsize < DDS_HEADER_SIZE)
		{
			std::cout << "ERROR: loadDDSTexture failed, fname = " << fname <<
				", fsize = " << fsize << ", less then DDS_HEADER_SIZE (" << DDS_HEADER_SIZE << ")" << std::endl;
			return false;
		}
		unsigned int signature = *(unsigned int*)&(dataPtr[0]);
		unsigned int height = *(unsigned int*)&(dataPtr[12]);
		unsigned int width = *(unsigned int*)&(dataPtr[16]);
		unsigned int mipMapNr = *(unsigned int*)&(dataPtr[28]);
		unsigned int format = *(unsigned int*)&(dataPtr[84]);

		if (signature != DDS_SIGNATURE) {
			std::cout << "LOAD_MASTER::LOAD_TEXTURE_DDS: Failed to load fname = " << fname <<
				"invalid signature: 0x" << std::hex << signature << std::endl;
			return false;
		}

		switch (format)
		{
		case FORMAT_CODE_DXT1:
			format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			break;
		case FORMAT_CODE_DXT3:
			format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			break;
		case FORMAT_CODE_DXT5:
			format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			break;
		default:
			std::cout << "LOAD_MASTER::LOAD_TEXTURE_DDS: Failed to load fname = " << fname <<
				", unknown formatCode: 0x" << std::hex << format << std::endl;
			return false;
		}
		GL::Texture::bind(tex);
		//DXT1 = 8 blocks
		unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
		unsigned int offset = DDS_HEADER_SIZE;
		for (unsigned int level = 0; level < mipMapNr; ++level)
		{//Generate every single bitmap
			unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
			if (fsize < offset + size) {
				std::cout << "LOAD_MASTER::LOAD_TEXTURE_DDS: Failed to load fname = " << fname <<
					", fsize = " << fsize << ", level = " << level <<
					", offset = " << offset << ", size = " << size << std::endl;
				return false;
			}
			glCompressedTexImage2D(GL_TEXTURE_2D, level,
				format, width, height, 0, size, dataPtr + DDS_HEADER_SIZE);
			//w/h is greater than one? move on one bit or set to 1
			width = width > 1 ? width >> 1 : 1;
			height = height > 1 ? height >> 1 : 1;
			offset += size;
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		tex->unbind();

		return true;
	}
#endif //LoadTexture_DDS

	void LoadMaster::flipTexture(unsigned char* textureData, int width, int height, int n)
	{
		for (int h = 0; h < height / 2; ++h)
		{//Every pixel in column
			for (int w = 0; w < width; ++w)
			{//Every pixel in row
				for (int i = 0; i < n; ++i)
				{//Every channel component : RGBA
					int offset1 = (h * width + w) * n + i;
					int offset2 = ((height - h - 1) * width + w) * n + i;
					unsigned char t = textureData[offset1];
					textureData[offset1] = textureData[offset2];
					textureData[offset2] = t;
				}
			}
		}
	}

	void LoadMaster::printCurrentImport()
	{
		printf("LOAD_MASTER::PRINT_CURRENT_IMPORT: %d models and %d textures.\n", impModels, impTextures);
	}
}