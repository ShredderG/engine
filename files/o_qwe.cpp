GAME_OBJECT_qwe::GAME_OBJECT_qwe(float GAME_x, float GAME_y, float GAME_z)
{
x = GAME_x;
y = GAME_y;
z = GAME_z;
qwe = this;

}

void GAME_OBJECT_qwe::GAME_kill()
{

if (qwe == this)
{
	if (GAME_right) if (GAME_right->GAME_id() == GAME_OBJECT_qwe::GAME_id()) qwe = (GAME_OBJECT_qwe*) GAME_right;
	if (qwe == this) qwe = (GAME_OBJECT_qwe*) GAME_OBJECT_qwe::GAME_id();
}
}

void GAME_OBJECT_qwe::GAME_step()
{

}

void GAME_OBJECT_qwe::GAME_draw()
{

}

ObjectId GAME_OBJECT_qwe::GAME_id() const
{
return ObjectId::qwe;
}