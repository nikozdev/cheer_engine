print("|MAIN_STATE::LOAD|")
notInitialized = true;

function Initialize()
	luahf = require("lua_scripts/functions/helper_functions");

	print("INITIALIZE_LUA_STATE");
	dofile("lua_scripts/data/light_data.lua");
	dofile("lua_scripts/data/vertex_data.lua");
	dofile("lua_scripts/data/shader_data.lua");
	dofile("lua_scripts/data/model_data.lua");
	notInitialized = false;
	print("|MAIN_STATE::INITIALIZED|\n");

	--cpp_createGLWindow(800, 600, "SupportTools");
	obj0_shader = Shader:New(shaders.obj0_3d.src.vertex, shaders.obj0_3d.src.fragment_phong);
	text0_shader = Shader:New(shaders.text0.src.vertex, shaders.text0.src.fragment);
	helper0_shader = Shader:New(shaders.helper0.src.vertex, shaders.helper0.src.fragment);
end

function Update()
	--
end

function Render()
	if (obj0_shader == nil) then
		return;
	end;
	cpp_scaleRotateMove(0, models.transform.scale.scale_1_0, models.transform.rotate.rotate_90_0_0,
	models.transform.move.move_0_0_0);
end

function ReCall()
	--cpp_loadMesh("clean", 0);
	--cpp_loadMesh(models.book0.paths.obj, 0);

	--cpp_loadMaterial(0, "clean", "somePath");
	--cpp_loadMaterial(0, "material", tex_paths.glass0, tex_paths.glass0);
end

if (Hero == nil) then
Hero = {
	GraphicsComponent = {
		sprite_name = "Resources/maps/GingerBread.png",
		anim_name = "Resources/animations/hero_idle_0.png"
	},
	LifeComponent = {
		max_health = 20,
		health = 0,
		health_state = 2
	},
	NpcComponent = {
		can_talk = true,
		greet_phrase = "Hello Lua! I am your first Entity"
	}
};

GingerBread = {
	GraphicsComponent = {
		sprite_name = "Resources/maps/GingerBread.png",
		anim_name = "Resources/animations/hero_idle_0.png"
	},
	LifeComponent = {
		max_health = 5,
		health = 0,
		health_state = 2
	},
	NpcComponent = {
		can_talk = true,
		greet_phrase = "Hello Lua! I am your second entity!"
	}
};
end;

if(notInitialized == false) then
	print("|MAIN_STATE::LOADED|\n");
end;