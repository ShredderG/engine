void GM_ROOM_r_start()
{
objectCreate(0, 0, 0, o_hero);
objectCreate(0, 0, 0, o_world); 
}

GM_room
	r_start(GM_ROOM_r_start);
