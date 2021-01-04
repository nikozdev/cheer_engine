print("lua_help script is executed");
--__index is a function that is called if there is no appropriate index for some table
	--{var = table.unknownKey}
--__newindex is a function that adds new keys
--{table.newKey = value}
--__call is a function that is called if we call table like a function
--{table(args)}
--__tostring is called if we try to use table as a string
--{print(table) or tostring(table)}
--__concat is called if we use concatenation with table
--{table..value}
--__metatable is called if we take metatable
--{table.__metatable = "can't get meta"; getmetatable(table)}
--__gc is called if gargabe collector collects a table
--{delete table -> __gc}
--__len is called if we use # to table
--{#table}
--__paitrs is alternative iterator for pairs
--{table.paits}
--__ipairs is alternative for ipairs

--arithmetic operations
--__add+
--__sub-
--__mul*
--__div/
--__pow^
--__mod%
--__idiv//
--__eq==
--__lt<
--__le<=
--
--binary operations
--__band&
--__bor|
--__bxor~
--__bnot
--__bshl<< shift left
--__bshr>> shift right
--
--raw- prefix ignores all metatables and uses default implementation
--
--__index usually equals a new table with uservalues
--If a table contains __index table where is defined __index table...
--there is an opportuinity to make searching chains


--dofile([filename])

--error(message[, level])

--ipairs(table)
	--returns 3 values: iteration-function, table and zero
--next(table(index))
	--move throught all table fields.
	--next(tab) withound index argument - sets nil as index 
	--it allows to check whether a table is empty or not


--BezierSpline
	--((t - 1) + t)^2 if startPos = 1, waypoint = 1 and targetPos = 1
	--startPos*(t-1)(t-1) + 2.0*(t-1)*t*waypoint + t*t*targetPos

--Update manipulation
	--fElapsedTime -> fTimePassed+=fElapsedTime(wasted time)
	--float time = fTimeElapsed / fCompletionTime;
	--if (fTimeElapsed >= fCompletionTime)
		--return complete;

--Dynamic objects
	--vec2 pos
	--vec2 size{1,1}
	--vec2 velocity;
	--int id = 0;
	--bool isDead - false;
--std::vector<std::shared_ptr<Dynamic>> vDinamicObjs;
	--shared_ptr<Dynamic> CreateDynamicObj(int type, x, y)
	--auto dynob = make_shared<Dynamic>()
	--push into vector and return dynob
		--Is there object in a vector? std::find_if(v.begin(), v.end(), [&object](const std::refToVec<T>&) {return obj == refToVec.get()})