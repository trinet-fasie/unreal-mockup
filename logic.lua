
local type_button = "button"
local type_lever_arm = "lever_arm"


local type_animation = "animation"
local type_fire = "fire"
local type_lamp = "lamp"
local type_text_screen = "text_screen"

local id_btn_remove_fakel = 0
local id_btn_remove_test = 0
local id_btn_0 = 0
local id_btn_1 = 0
local id_arm_0 = 0
local id_arm_1 = 0

local id_anim = 0
local id_anim_1 = 0
local id_text = 0
local id_fire = 0
local id_lamp = 0

local is_initialized = false

function Init( ... )
	loader.Log("[LUA] enter method Init")

--create buttons
	id_btn_remove_fakel = loader.CreateObject(type_button, -820, -690, 250)
	id_btn_remove_test = loader.CreateObject(type_button, 10, -380, 250)

	id_btn_0 = loader.CreateObject(type_button, 10, -130, 250)
	id_btn_1 = loader.CreateObject(type_button, 10, 360, 250)
	id_arm_0 = loader.CreateObject(type_lever_arm, 10, 860, 250, 0, 0, 90)
	id_arm_1 = loader.CreateObject(type_lever_arm, 10, 1360, 250, 0, 0, 90)

--create objects
	id_anim_1 = loader.CreateObject(type_animation, 500, -380, 200, 0, 0, 90)

	id_anim = loader.CreateObject(type_animation, 500, -130, 200, 0, 0, 90)

	id_text =  loader.CreateObject(type_text_screen, 500, 360, 200, 0, 0, 180)
	loader.SendMessageInObject(type_text_screen, id_text, "This message from lua with love!!!")

	id_fire = loader.CreateObject(type_fire, 500, 860, 350, 0, 0, 0)

	id_lamp = loader.CreateObject(type_lamp, 500, 1360, 200, 0, 0, 0)

  
	lamps = {}
	local x = 0;
	local i = 0;
	local k = 0;
	local j = 0;
	local id = 0;

	for i=1,36 do
		x = -1840 + i * 100
	
		id = loader.CreateObject(type_fire, -2050, x, 350, 0, 0, 0)
		table.insert(lamps, id)
	end

	for i=1,36 do
		x = -1840 + i * 100
	
		id = loader.CreateObject(type_fire, 1450, x, 350, 0, 0, 0)
		table.insert(lamps, id)
	end


	for j=1,35 do
		x = -2100 + j * 100
		 id= loader.CreateObject(type_fire, x, 1780, 350, 0, 0, 0)
		 table.insert(lamps, id)
	end

	for d=1,35 do
		x = -2100 + d * 100
		id = loader.CreateObject(type_fire, x, -1840, 350, 0, 0, 0)
		 table.insert(lamps, id)
	end


	is_initialized = true

	loader.Log("[LUA] end method Init")
end

local l_enabled = false
local prevl_enabled = false

local r_enabled = false
local prevr_enabled = false

local lapm_onoff = false
local prevlapm_onoff = false

local lamp_state = false

function Update(dt)

	if(is_initialized == false) then
		Init()
		return
	end

--loader.Log("[LUA] method Update") 	

	if(loader.ShouldHandleUseEvent(type_button, id_btn_0) == 1) then
		loader.Log("[LUA] ShouldHandleUseEvent")
		loader.UseObject(type_animation, id_anim)

		 for i, lamp in ipairs(lamps) do
  			loader.UseObject(type_fire, lamp)
		end
	end


	if(loader.ShouldHandleUseEvent(type_button, id_btn_remove_fakel) == 1) then
		for i, lamp in ipairs(lamps) do
  			loader.RemoveObject(type_fire, lamp)
  			lamps[i] = nil
		end
	end

	if(loader.ShouldHandleUseEvent(type_button, id_btn_remove_test) == 1 and loader.ExistObject(type_animation, id_anim_1) == 1 ) then
		loader.RemoveObject(type_animation, id_anim_1)
	end

	if(loader.ShouldHandleUseEvent(type_button, id_btn_1) == 1) then
		loader.UseObject(type_text_screen, id_text)
	end


	if(loader.ShouldHandleUseEvent(type_lever_arm, id_arm_0) == 1) then
       r_enabled = not r_enabled
		changeLampState()

		loader.UseObject(type_fire, id_fire)
	end

	if(loader.ShouldHandleUseEvent(type_lever_arm, id_arm_1) == 1) then
		l_enabled = not l_enabled
		changeLampState()
	end

	

end

function Exit(dt)
   loader.Log("[LUA] method Exit")
end

function changeLampState()
   if(l_enabled and r_enabled and lamp_state == false) then
			loader.UseObject(type_lamp, id_lamp)
			lamp_state = true

	elseif(lamp_state == true) then
			loader.UseObject(type_lamp, id_lamp)
			lamp_state = false



	end
end