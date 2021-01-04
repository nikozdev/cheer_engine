print("LIGHT_DATA");

lights = {};

lights.direct0 = {
	ambient = {0.15, 0.15, 0.15},
	diffuse = {0.5, 0.5, 0.5},
	specular = {1.0, 1.0, 1.0},
	
	direction = {0.0, 0.5, 0.5}
};

lights.point0 = {
	ambient = {0.15, 0.15, 0.15},
	diffuse = {0.5, 0.5, 0.5},
	specular = {1.0, 1.0, 1.0},

	const_k = 1.0,
	lin_k = 0.35,
	quad_k = 0.15
};

lights.spot0 = {
	ambient = {0.15, 0.15, 0.15},
	diffuse = {0.5, 0.5, 0.5},
	specular = {1.0, 1.0, 1.0},
	
	direction = {0.0, 0.0, -1.0},
	
	const_k = 1.0,
	lin_k = 0.35,
	quad_k = 0.15,
	
	cutoff_in = 25.0,
	cutoff_out = 30.0
};