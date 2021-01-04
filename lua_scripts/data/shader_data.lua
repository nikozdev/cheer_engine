print("SHADER_PATHS");

shaders = {};

shaders.test0 = {
	src = {
		vertex = "Shaders/test0_vs.lua",
		fragment = "Shaders/test0_fs.lua",
	}
}

shaders.light0 = {
	src = {
		vertex = "Shaders/light_vs.lua",
		fragment = "Shaders/light_fs.lua",
	}
}

shaders.obj0_2d = {
	src = {
		vertex = "Shaders/obj0_2d_vs.lua",
		fragment = "Shaders/obj0_2d_fs.lua"
	}
}

shaders.obj0_3d = {
	src = {
		vertex = "Shaders/obj0_3d_vs.lua",
		fragment = "Shaders/obj0_3d_fs.lua",
		fragment_phong = "Shaders/obj_3d_phong_fs.lua"
	}
}

shaders.text0 = {
	src = {
		vertex = "Shaders/text0_vs.lua",
		fragment = "Shaders/text0_fs.lua"
	}
}

shaders.helper0 = {
	src = {
		vertex = "Shaders/helper_vs.lua",
		fragment = "Shaders/helper_fs.lua"
	}
}