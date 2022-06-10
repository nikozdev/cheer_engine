#include <Lua/L_state.h>
#include <support/ch_support.hpp>

MEM::ArenaAllocator* L::L_state::poolPtr;

namespace L
{
	void L_state::regLuaFunction(const char* function_name,
		const char* lua_code)
	{	// Push code into lua L_state and set it as a function
		lua_pushstring(L, lua_code);
		lua_setglobal(L, function_name);
	}

	void L_state::regCppFunction(const char* func_name,
		lua_CFunction reg_function, UpValues* upVals)
	{	//Register native function in global namespace of the L_state
		lua_pushcfunction(L, reg_function);
		lua_setglobal(L, func_name);
	}

	void L_state::regCppFunction(const char* table_name,
		const char* func_name, lua_CFunction function, UpValues* upVals)
	{	//Register native function in some table
		lua_getglobal(L, table_name);
		if (lua_isnil(L, -1))
		{
			lua_pop(L, 1);
			printf("L_state::REG_CPP_FUNCTION: couldn't find %s. Creation...\n", table_name);
			lua_newtable(L);
			lua_setglobal(L, table_name);
		}
		lua_pushstring(L, func_name);
		if (upVals == NULL)
			lua_pushcfunction(L, function);
		else
		{
			upVals->pushToLua(L);
			lua_pushcclosure(L, function, upVals->up_val_amount);
		}
		lua_settable(L, -3);
		lua_pop(L, 1);
	}

	void L_state::regType(const char* new_struct_name, lua_CFunction constructor,
		lua_CFunction destructor, lua_CFunction index, lua_CFunction newindex)
	{//Create new table with given name, metatable for it, and set redefined metamethods
		lua_getglobal(L, new_struct_name);
		if (!lua_isnil(L, -1))
		{//Such table already exists
			lua_pop(L, 1);
			printError("L_state::REG_TYPE:", "Attempt to create table with registered name\n");
			return;
		}
		lua_newtable(L);
		lua_pushvalue(L, lua_gettop(L));
		lua_setglobal(L, new_struct_name);
		//Table stays 1
		lua_pushcfunction(L, static_cast<lua_CFunction>(constructor));
		lua_setfield(L, -2, "New");
		//Table is -2, meta_Table is -1
		luaL_newmetatable(L, (std::string("meta_") + new_struct_name).c_str());
		if (destructor != nullptr)
		{//Set garbage collection
			lua_pushstring(L, "__gc");
			lua_pushcfunction(L, destructor);
			lua_settable(L, -3);
		}
		if (index != nullptr)
		{//If given key isn't found - use this function as an index
			lua_pushstring(L, "__index");
			lua_pushcfunction(L, index);
			lua_settable(L, -3);
		}
		if (newindex != nullptr)
		{//If there is no given key to set another value - use it
			lua_pushstring(L, "__newindex");
			lua_pushcfunction(L, newindex);
			lua_settable(L, -3);
		}
		lua_pop(L, 3);
		printf("L_state::REG_TYPE: %s is registered.\n", new_struct_name);
	}

	void L_state::regType(const char* new_struct_name, UpValues& up_values,
		lua_CFunction constructor, lua_CFunction destructor, lua_CFunction index, lua_CFunction newindex)
	{
		lua_getglobal(L, new_struct_name);
		if (!lua_isnil(L, -1))
		{//Such table already exists
			lua_pop(L, 1);
			printError("L::STATE::REG_TYPE:", "Attempt to create table with registered name\n");
			return;
		}
		//Acces to native type in lua throught table
		lua_newtable(L);
		lua_pushvalue(L, lua_gettop(L));
		lua_setglobal(L, new_struct_name);

		up_values.pushToLua(L);
		lua_pushcclosure(L, static_cast<lua_CFunction>
			(constructor), up_values.up_val_amount);
		lua_setfield(L, -2, "New");

		//Table is -2, meta_Table is -1
		luaL_newmetatable(L, ("meta_" + static_cast<std::string>(new_struct_name)).c_str());
		if (destructor != nullptr)
		{//Set garbage collection
			up_values.pushToLua(L);
			lua_pushcclosure(L, static_cast<lua_CFunction>
				(destructor), up_values.up_val_amount);
			lua_setfield(L, -2, "__gc");
		}
		if (index != nullptr)
		{//If given key isn't found - use this function as an index
			up_values.pushToLua(L);
			lua_pushcclosure(L, static_cast<lua_CFunction>
				(index), up_values.up_val_amount);
			lua_setfield(L, -2, "__index");
		}
		if (newindex != nullptr)
		{//If there is no given key to set another value - use it
			up_values.pushToLua(L);
			lua_pushcclosure(L, static_cast<lua_CFunction>
				(newindex), up_values.up_val_amount);
			lua_setfield(L, -2, "__newindex");
		}
		lua_pop(L, 3);
		printf("L::STATE::REG_TYPE: %s is registered.\n", new_struct_name);
	}

	void L_state::regLibs(luaL_Reg* libs, lua_State* L)
	{//Register concrete libraries
		for (luaL_Reg* lib = libs; lib->name != NULL; lib++)
		{//In lua: name = function
			luaL_requiref(L, lib->name, lib->func, 1);
			lua_settop(L, 0);
		}
	}

	//Constructors & Destructors
	L_state::L_state(const char* lua_state_file) :
		lStateFile(lua_state_file), level(0),
		pool(MEM::ArenaAllocator(memoryBytes, &memoryBytes[MEM::ArenaAllocator::POOL_SIZE - 1]))
	{
		poolPtr = &pool;
		L = lua_newstate(MEM::ArenaAllocator::l_alloc, &pool);
		pool.printMemoryUsage();
		luaL_openlibs(L);
		assert(L != nullptr);
		printf("L::STATE::CONSTRUCTOR: %s is created.\n", lStateFile);
		pool.printMemoryUsage();
	}

	L_state::~L_state()
	{
		L_state::pool.printMemoryUsage();
		lua_close(L);
		printf("L::STATE::DESTRUCTOR: %s is closed.\n", lStateFile);
		L_state::pool.printMemoryUsage();
		poolPtr = nullptr;
	}

	//Execute functions
	bool L_state::exeScript(const char* lua_script)
	{
		if (luaL_dofile(L, lua_script) != LUA_OK)
		{
			if (luaL_dostring(L, lua_script) != LUA_OK)
			{
				printError("\nL::STATE::EXE_SCRIPT:", "Failed to execute the script");
				return false;
			}
			printf("\nL::STATE::EXE_SCRIPT: StringScript is successfuly done!\n");
			return true;
		}
		printf("\nL::STATE::EXE_SCRIPT: Script %s is successfuly done!\n", lua_script);
		return true;
	}
	bool L_state::loadFile()
	{
		if (luaL_dofile(L, lStateFile) != LUA_OK)
		{
			printError("\nL::STATE::LOAD_FILE:", " Script isn't loaded");
			return false;
		}
		return true;
	}

	//GetFunctions
	bool L_state::getToStack(const std::string& dataName)
	{
		level = 0;
		std::string path = "";
		for (unsigned int i = 0; i < dataName.size(); i++)
		{
			if (dataName[i] == '.')
			{
				if (level == 0)
				{
					lua_getglobal(L, path.c_str());
				}
				else
				{
					lua_getfield(L, -1, path.c_str());
				}
				if (lua_isnil(L, -1))
				{
					printf("L::STATE::GET_TO_STACK:\nfailed on path to %s\nLevel = %d.\n",
						dataName.c_str(), level);
					return false;
				}
				level++;
				path = "";
			}
			else
			{
				path += dataName[i];
			}
		}
		if (level == 0)
		{
			lua_getglobal(L, path.c_str());
		}
		else
		{
			lua_getfield(L, -1, path.c_str());
		}
		if (lua_isnil(L, -1))
		{
			printf("L::STATE::GET_TO_STACK: failed to get %s.\nLevel = %d.\n",
				dataName.c_str(), level);
			return false;
		}
		if (level == 0)
		{
			return true;
		}
		else
		{//Top grew on level amount
			int tableId = lua_gettop(L) - level; //global table
			lua_replace(L, tableId);
			lua_settop(L, tableId); //pop all previous erelevant tables
		}
		return true;
	}

	void L_state::tabToVec(lua_State* L, glm::vec3& vec3)
	{
		if (!lua_istable(L, -1))
		{
			printf("L::STATE::TAB_TO_VEC_3: There is no table at the top...\n");
			return;
		}
		else {//Nil is essential because there will be placed next pair
			lua_pushnil(L);
			for (short i = 0; i < 3; i++)
			{
				if (lua_next(L, -2))
					if (lua_isnumber(L, -1))
						vec3[i] = static_cast<float>(lua_tonumber(L, -1));
				lua_pop(L, 1);
			}
		}
		lua_pop(L, 2);
	}
	void L_state::tabToVec(lua_State* L, glm::vec4& vec4)
	{
		if (!lua_istable(L, -1))
		{
			printf("L::STATE::TAB_TO_VEC_3: There is no table at the top...\n");
			return;
		}
		else {//Nil is essential because there will be placed next pair
			lua_pushnil(L);
			for (short i = 0; i < 5; i++)
			{
				if (lua_next(L, -2))
					if (lua_isnumber(L, -1))
						vec4[i] = static_cast<float>(lua_tonumber(L, -1));
				lua_pop(L, 1);
			}
		}
		lua_pop(L, 2);
	}
	//Vectors
	std::vector<std::string> L_state::getStrVec(const std::string& table_name)
	{//We need to get strings from table
		// function for getting table keys
		lua_getglobal(L, "getKeys");
		if (lua_isnil(L, -1))
		{
			lua_pop(L, 1);
			std::string code =
				"function getKeys(name) "
				"s = ''; " //Initialize varibale for string collection
				"for k, v in pairs(_G[name]) do " //Process all pairs of global table
				"    s = s..k..'|'; " //Concat current string with every key
				"    end; "
				"return s; "
				"end; ";
			luaL_loadstring(L, code.c_str());
			lua_pcall(L, 0, 0, 0);
			lua_getglobal(L, "getKeys"); //Get function
		}

		lua_pushstring(L, table_name.c_str()); //Give table name as an argument
		lua_pcall(L, 1, 1, 0); //Execute function and give new string of table keys to the function
		return luaGetData<std::vector<std::string>>(table_name);
	}

	//Main functions
	bool L_state::initialize()
	{
		regCppFunction("cpp_printStackTypes", cpp_printStackTypes);
		regCppFunction("cpp_printMemory", cpp_printMemory);
		lua_getglobal(L, "Initialize");
		if (lua_pcall(L, 0, 0, 0) != LUA_OK)
		{
			printError("L::STATE::INITIALIZE:", "Initialize function is failed");
			return false;
		}
		return true;
	}

	void L_state::update()
	{
		lua_getglobal(L, "Update");
		if (lua_pcall(L, 0, 0, 0) != LUA_OK)
		{
			printError("L::STATE::UPDATE:", "Update function is failed");
		}
	}

	void L_state::render()
	{
		lua_getglobal(L, "Render");
		if (lua_pcall(L, 0, 0, 0) != LUA_OK)
		{
			printError("L::STATE::RENDER:", "Render function is failed");
		}
	}

	int L_state::cpp_printStackTypes(lua_State* L)
	{
		printf("L::STATE::PRINT_STACK_TYPES: Stack contains...\n");
		int maxStackSize = 5;
		if (lua_gettop(L) < 12)
			maxStackSize = lua_gettop(L);
		else maxStackSize = 12;
		for (short i = 1; i <= maxStackSize; i++)
		{//If stack contains more than 15 values - count to 12
			switch (lua_type(L, -i))
			{
			case LUA_TLIGHTUSERDATA:
				printf("%d-th: is %s\n", -i, "light userdata");
				break;
			case LUA_TNIL:
				printf("%d-th: is %s\n", -i, "nil");
				break;
			case LUA_TNUMBER:
				printf("%d-th: is %s\n", -i, "number");
				break;
			case LUA_TTABLE:
				printf("%d-th: is %s\n", -i, "table");
				break;
			case LUA_TFUNCTION:
				printf("%d-th: is %s\n", -i, "function");
				break;
			case LUA_TSTRING:
				printf("%d-th: is %s\n", -i, "string");
				break;
			case LUA_TUSERDATA:
				printf("%d-th: is %s\n", -i, "userdata");
				break;
			}
		}
		return 0;
	}

	int L_state::cpp_printMemory(lua_State* L)
	{
		MEM::GlobalAllocator::get().printMemoryUsage();
		poolPtr->printMemoryUsage();
		return 0;
	}

	//Debug
	void L_state::printError(const char* location, const char* error)
	{
		printf("%s %s\n%s\n", location, error, lua_tostring(L, -1));
	}
}