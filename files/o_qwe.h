struct GAME_OBJECT_qwe final : public Engine::Object
{


	GAME_OBJECT_qwe(float GAME_x, float GAME_y, float GAME_z);
	virtual void     GAME_kill()     override final;
	virtual void     GAME_step()     override final;
	virtual void     GAME_draw()     override final;
	virtual ObjectId GAME_id() const override final;
} *qwe = (GAME_OBJECT_qwe*) ObjectId::qwe;
