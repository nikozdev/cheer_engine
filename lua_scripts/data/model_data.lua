print("MODEL_DATA");
dofile("lua_scripts/data/material_data.lua");

models = {};

models.transform = {
	scale = {
		scale_0_5 = {0.5, 0.5, 0.5},
		scale_1_0 = {1.0, 1.0, 1.0},
		scale_2_0 = {2.0, 2.0, 2.0},
		scale_10_0 = {10.0, 10.0, 10.0}
	},
	rotate = {
		rotate_0_0_0 = {0.0, 0.0, 0.0},
		rotate_0_0_90 = {0.0, 0.0, 90.0},
		rotate_0_90_0 = {0.0, 90.0, 0.0},
		rotate_90_0_0 = {90.0, 0.0, 0.0}
	},
	move = {
		move_0_0_0 = {0.0, 0.0, 0.0},
		move_0_0_5 = {0.0, 0.0, 5.0},
		move_0_5_0 = {0.0, 5.0, 0.0},
		move_5_0_0 = {5.0, 0.0, 0.0}
	}
};

models.backpack0 = {
	paths = {
		obj = "Resources/models/backpack/backpack.obj",
		dae = ""
	}
	--
};

models.book0 = {
	paths = {
		obj = "Resources/models/falmer_book.obj",
		dae = "Resources/models/falmer_book.dae"
	},
	transform = {
		scale = models.transform.scale.scale_10_0,
		rotate = models.transform.rotate.rotate_0_90_0,
		move = models.transform.move.move_0_0_0
	},
	material = {
		name = materials.material0
	}
};

models.book1 = {
	paths = {
		obj = "Resources/models/black_book.obj",
		dae = ""
	}
};

models.decay_dragon0 = {
	paths = {
		obj = "Resources/models/decay_dragon.obj",
		dae = "Resources/models/decay_dragon.dae"
	}
};

models.vampire_lord0 = {
	paths = {
		obj = "Resources/models/vampire_lord.obj",
		dae = ""
	}
};

models.forge0 = {
	paths = {
		obj = "Resources/models/forge.obj",
		dae = ""
	}
};

models.frozen_throne = {
	paths = {
		obj = "Resources/models/frozen_throne.obj",
		dae = ""
	}
};

models.skull0 = {
	paths = {
		obj = "Resources/models/skull.obj",
		dae = ""
	}
};

models.jelly0 = {
	paths = {
		obj = "Resources/models/jelly.obj",
		dae = ""
	}
};