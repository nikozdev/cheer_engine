print("HELPER_FUNCTIONS");

local help_f = {}
--Get keys of a table
function help_f.getTabKeys(tab)
	keys = {};
	for key, value in pairs(tab) do
		table.instert(keys, key);
		end;
	return keys;
end;
--Get keys of a table in new table
function help_f.getKeys(tab)
	string = '';
	for key, value in pairs(tab) do
		string = string..key..'|';
	end;
	return string;
end;

function help_f.derive(child, parent)
	setmetatable(child, {__index = parent});
end;

function help_f.newCorout(func)
	coroutine.create(func);
end;

function help_f.resumeCorout(behaviour)
	--The coroutine function is gone to the end
	if (coroutine.state(behaviour) != "dead")
	coroutine.resume(behaviour);
end;

return help_f;