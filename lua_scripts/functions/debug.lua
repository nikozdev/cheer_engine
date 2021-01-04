function Debug()
	--hero_ent = Entity:New("Hero");
	--hero_ent:say("I am your second Entity!");
	
	--cpp_createGLWindow(800, 600, "SupportTools");
	
	--cpp_loadMesh(models.book0.paths.obj, 0);
	--cpp_loadMaterial(0, "clean", "somePath");
	--cpp_loadMaterial(0, "material", tex_paths.glass0, tex_paths.glass0);

	--cpp_scaleRotateMove(0, models.transform.scale.scale_1_0, models.transform.rotate.rotate_90_0_0,
	--models.transform.move.move_0_0_0);
	
	--text0_shader = Shader:New(shaders.text0.src.vertex, shaders.text0.src.fragment);

	collectgarbage(collect);
	cpp_printMemory();
end;