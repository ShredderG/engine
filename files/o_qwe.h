struct GAME_OBJECT_qwe final : public Engine::Object
{


	GAME_OBJECT_qwe(float GAME_x, float GAME_y, float GAME_z);
	virtual void     GAME_kill()     override;
	virtual void     GAME_step()     override;
	virtual void     GAME_draw()     override;
	virtual ObjectId GAME_id() const override;
} *qwe = (GAME_OBJECT_qwe*) ObjectId::qwe;
