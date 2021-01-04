print("ENTITY")

Entity = {}

function Entity:New()
	--private members - table for work with the class
	local private = {}
	private.name = "entity";
	--public members - resulting table
	local public = {}
	function public:get_name = function()
		return private.name;
	end;
	function public:set_name = function(new_name)
		private.name = new_name;
	end;
	--Make public derrived table and give it as an object
	setmetatable(public, self); --set Entity as metatable for an object
	self.index = self; --call Entity table if object doesn't have some member
	return public; --return object
end;