// OBJECT_qwe
class OBJECT_qwe final : public Engine::Object {
public:

	OBJECT_qwe();
	virtual void     GAME_kill()     override;
	virtual void     GAME_step()     override;
	virtual void     GAME_draw()     override;
	virtual ObjectId GAME_id() const override;
} *qwe = (OBJECT_qwe*)ObjectId::qwe;
