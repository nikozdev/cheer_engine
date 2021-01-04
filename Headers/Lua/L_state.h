#ifndef L_STATE_H
#define L_STATE_H

#include <framework.hpp>
#include <support/ch_support.hpp>

#include "lua_structs.h"
#ifndef LUA_STRUCTS_H
#include <Lua_header.hpp>
#endif

#include <math/glm/vec3.hpp>
#include <math/glm/vec4.hpp>

namespace L
{
	class L_state
	{
	private:
		//StateData
		lua_State* L;
		const char* lStateFile;
		//Memory management
		char memoryBytes[MEM::ArenaAllocator::POOL_SIZE];
		MEM::ArenaAllocator pool;
		static MEM::ArenaAllocator* poolPtr;
		//Helping variables
		int level;

		//Debug
		void printError(const char* location, const char* error);
	public:
		//RegisterFunctions
		/// Push code into a lua L_state and set it as a function
		void regLuaFunction(const char* function_name,
			const char* lua_code);
		/// Push C function into a lua L_state and set it as a global variable
		void regCppFunction(const char* table_name,
			const char* function_name, lua_CFunction function, UpValues* upVals = nullptr);
		/// Get table in a lua L_state and set into it a C function
		void regCppFunction(const char* function_name,
			lua_CFunction function, UpValues* upVals = nullptr);


		void regType(const char* new_struct_name,
			lua_CFunction constructor, lua_CFunction destructor = nullptr,
			lua_CFunction index = nullptr, lua_CFunction newindex = nullptr);
		void regType(const char* new_struct_name, UpValues& up_values,
			lua_CFunction constructor, lua_CFunction destructor = nullptr,
			lua_CFunction index = nullptr, lua_CFunction newindex = nullptr);

		static void regLibs(luaL_Reg* libs, lua_State* L);

		//Constructors & Destructors
		L_state(const char* lua_state_file);
		~L_state();

		//Accessors
		lua_State* getLState() const { return L; }

		//Execute functions
		bool exeScript(const char* lua_script);
		bool loadFile();

		//GetFunctions
		template<typename T>
		T getData(const std::string& dataName)
		{//Set variable on the top - get it with required type
			T result;
			if (getToStack(dataName))
			{//Get required data by given type
				std::string searchName = dataName;
				if (level != 0)
				{
					searchName = "";
					for (int i = dataName.size() - 1; i > 0; i--)
					{//We need to get last variable name
						if (dataName.at(i) == '.')
						{//...table.variable
							searchName.append(&dataName.at(i + 1));
							break;
						}
					}
				}
				result = luaGetData<T>(searchName);
			}
			else
			{//Can't get to stack - return nil
				result = luaGetNull<T>();
			}
			//Don't forget to clean the stack
			lua_pop(L, lua_gettop(L));
			return result;
		}
		template<typename T>
		T luaGetData(const std::string& dataName)
		{//Without specification - return 0
			return 0;
		}
		template<typename T>
		T luaGetNull()
		{//There is no any data
			return 0;
		}

		bool getToStack(const std::string& dataName);
		
		static void tabToVec(lua_State* L, glm::vec3& vec3);
		static void tabToVec(lua_State* L, glm::vec4& vec4);
		std::vector<std::string> getStrVec(const std::string& table_name);
		//Main L_state functions
		bool initialize();
		void update();
		void render();
	
		//Static lua functions
		static int cpp_printStackTypes(lua_State* L);
		static int cpp_printMemory(lua_State* L);
	};
	//Specializations for types
	template<>
	inline bool L_state::luaGetData<bool>(const std::string& dataName)
	{
		return (bool)lua_toboolean(L, -1);
	}
	template<>
	inline int L_state::luaGetData<int>(const std::string& dataName)
	{
		if (!lua_isinteger(L, -1))
		{
			printf("L_state::GET_DATA::LUA_GET_DATA: Variable isn't an integer\n");
		}
		return (int)lua_tointeger(L, -1);
	}
	template<>
	inline float L_state::luaGetData<float>(const std::string& dataName)
	{
		if (!lua_isnumber(L, -1))
		{
			printf("L_state::GET_DATA::LUA_GET_DATA: Variable isn't a float\n");
		}
		return (float)lua_tonumber(L, -1);
	}
	template<>
	inline double L_state::luaGetData<double>(const std::string& dataName)
	{
		if (!lua_isnumber(L, -1))
		{
			printf("L_state::GET_DATA::LUA_GET_DATA: Variable isn't a double\n");
		}
		return (double)lua_tonumber(L, -1);
	}
	template<>
	inline std::string L_state::luaGetData<std::string>(const std::string& dataName)
	{
		if (!lua_isstring(L, -1))
		{
			printf("L_state::GET_DATA::LUA_GET_DATA:\nVariable is not a string.\n");
			return "nil";
		}
		return (std::string)lua_tostring(L, -1);
	}
	//Specializations for vectors
	template<>
	inline std::vector<int> L_state::luaGetData<std::vector<int>>(const std::string& dataName)
	{
		std::vector<int> intVec;
		lua_pushnil(L);
		while (lua_next(L, -2))
			if (lua_isnumber(L, -1))
				intVec.push_back((int)lua_tointeger(L, -1));

		lua_pop(L, 1);
		lua_pop(L, lua_gettop(L));
		return intVec;
	}
	template<>
	inline std::vector<float> L_state::luaGetData<std::vector<float>>(const std::string& dataName)
	{
		std::vector<float> floatVec;
		lua_pushnil(L);
		while (lua_next(L, -2))
			if (lua_isnumber(L, -1))
				floatVec.push_back((float)lua_tonumber(L, -1));

		lua_pop(L, 1);
		lua_pop(L, lua_gettop(L));
		return floatVec;
	}
	template<>
	inline std::vector<std::string> L_state::luaGetData<std::vector<std::string>>(const std::string& dataName)
	{
		std::vector<std::string> strVec;
		//Table is new table from the function, temp - every word there
		std::string table = lua_tostring(L, -1);
		std::string temp = "";

		for (unsigned int i = 0; i < table.size(); i++) {
			if (table.at(i) != '|')
			{//Build string further
				temp += table.at(i);
			}
			else
			{//Add str, refresh temp variable and go further
				strVec.push_back(temp);
				temp = "";
			}
		}
		//Clean the stack and return the vector
		lua_pop(L, 1);
		return strVec;
	}
	//Specialization of null for not number types
	template<>
	inline std::string L_state::luaGetNull<std::string>()
	{//There is no any data for str
		return "nil";
	}
	template<>
	inline std::vector<std::string> L_state::luaGetNull<std::vector<std::string>>()
	{//There is no any data for str
		return std::vector<std::string>();
	}
	template<>
	inline std::vector<int> L_state::luaGetNull<std::vector<int>>()
	{//There is no any data for str
		return std::vector<int>();
	}
	template<>
	inline std::vector<float> L_state::luaGetNull<std::vector<float>>()
	{//There is no any data for str
		return std::vector<float>();
	}
}

#endif