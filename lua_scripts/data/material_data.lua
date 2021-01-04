print("MATERIAL_DATA");

tex_paths = {
	vampire_lord0_d = "Resources/maps/vampirelord.dds",
	vampire_lord0_n = "Resources/maps/vampirelord_n.dds",
	black_silk0 = "Resources/maps/BlackSilk.jpg",
	blue0 = "Resources/maps/BlueTex.jpg",
	blue1 = "Resources/maps/ScratchedBlue.jpg",
	container0 = "Resources/maps/container.png",
	container_spec0 = "Resources/maps/container_spec.png",
	fissures0 = "Resources/maps/fissures.jpg",
	picture0 = "Resources/maps/Gingerbread.png",
	glass0 = "Resources/maps/GlassTex.png",
	grass0 = "Resources/maps/GrassTex.jpg",
	ground0 = "Resources/maps/GroundSprite.png",
	matrix0 = "Resources/maps/matrix.jpg",
	metal0 = "Resources/maps/MetalTex.jpg",
	stone0 = "Resources/maps/Stone.JPG",
	stump0 = "Resources/maps/stump.png"
};

materials = {}

materials.material0 = {
	textures = {
		diffuse = {
			name = "texture_diffuse0",
			unit_num = 0,
			path = tex_paths.black_silk0
		},
		specular = {
			name = "texture_specular0",
			unit_num = 1,
			path = tex_paths.black_silk0
		}
	},
	RGB_vectors = {
		ambient = {0.15, 0.15, 0.15},
		diffuse = {0.4, 0.6, 0.4},
		specular = {1.0, 1.0, 1.0}
	}
}