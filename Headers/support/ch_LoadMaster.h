#ifndef LOAD_MASTER_H
#define LOAD_MASTER_H

#include <string>
#include <fstream>
#include <iostream>

#define DDS_HEADER_SIZE 128
#define DDS_SIGNATURE    0x20534444 // "DDS "
#define FORMAT_CODE_DXT1 0x31545844 // "DXT1"
#define FORMAT_CODE_DXT3 0x33545844 // "DXT3"
#define FORMAT_CODE_DXT5 0x35545844 // "DXT5"

// Predeclarations
namespace GL
{
	class Texture;
	class Mesh;
}

namespace SP
{
	class LoadMaster
	{
	private:
		static int impModels;
		static int impTextures;
	public:
		//Constructors&Destructors
		LoadMaster();
		~LoadMaster();

		// Static functions
		/// Default binary object saving with ofstream
		static bool saveIt_bin(const std::string& fileName, void* objectPtr, size_t sizeInBytes);
		/// Default binary object loading with ifstream
		static bool loadIt_bin(const std::string& fileName, void* objectPtr, size_t sizeInBytes);
		/// Default mesh loading.
		/// It makes the choice whatever format will be loaded by appropriate function
		static bool loadMesh_def(const std::string& path, GL::Mesh* mesh);
		static bool loadMesh_obj(const char* path, GL::Mesh* mesh);
		static bool loadMesh_dae(const char* path, GL::Mesh* mesh);
		/// Default texture loading
		/// It makes the choice whatever format will be loaded by appropriate function
		static bool loadTexture_def(const std::string& path, GL::Texture* tex);
		static bool loadTexture_stbi(const char* path, GL::Texture* tex);
		static bool loadTexture_dds(const char* path, GL::Texture* tex);
		/// Read the header of dds file before you export it
		static bool loadTexture_dds_common(const char* fname,
			unsigned char* dataPtr, GL::Texture* tex, size_t fsize);

		static void flipTexture(unsigned char* textureData, int width, int height, int n);
		static void printCurrentImport();
	};
}

#endif