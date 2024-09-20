#pragma once
class PlayerStatus
{
public:
	PlayerStatus()
		: HP(600)
		, MP(50)
		, Stamina(100)
		, maxHP(600)
		, maxMP(50)
		, maxStamina(100)
		, Damage(5.f)
		, Pos{0.f, 0.f, 1.9f}
		, CamPos{ 0.f, 0.f, 0.f }
	{}
	~PlayerStatus() = default;

	float		HP;
	float		MP;
	float		Stamina;
	float		maxHP;
	float		maxMP;
	float		maxStamina;

	float		Damage;
	Vec3		Pos;
	Vec3		CamPos;
};

class CPlayerManager
	: public CSingleton<CPlayerManager>
{
	SINGLE(CPlayerManager)
public:
	void Init();

	std::shared_ptr<PlayerStatus>& GetPlayerStatusRef() { return m_PlayerStatus; }

	void TakeDamage(float _Damage) 
	{ 
		m_PlayerStatus.get()->HP -= _Damage;
		if (m_PlayerStatus.get()->HP < 0)
			m_PlayerStatus.get()->HP = 0;
	}
	void UseStamina(float _Stamina)
	{ 
		m_PlayerStatus.get()->Stamina -= _Stamina;
		if (m_PlayerStatus.get()->Stamina < 0)
			m_PlayerStatus.get()->Stamina = 0;
	}
	void RecoverStamina(float _Stamina)
	{
		if (m_PlayerStatus.get()->Stamina >= m_PlayerStatus.get()->maxStamina)
			return;

		m_PlayerStatus.get()->Stamina += _Stamina;
		if (m_PlayerStatus.get()->Stamina > m_PlayerStatus.get()->maxStamina)
			m_PlayerStatus.get()->Stamina = m_PlayerStatus.get()->maxStamina;
	}
	void Recover(float _HP)
	{ 
		m_PlayerStatus.get()->HP += _HP; 
		if (m_PlayerStatus.get()->HP > m_PlayerStatus.get()->maxHP)
			m_PlayerStatus.get()->HP = m_PlayerStatus.get()->maxHP;
	}

	void SetNextPos(Vec3 _Pos) { m_PlayerStatus.get()->Pos = _Pos; }
	void SetNextCamPos(Vec3 _CamPos) { m_PlayerStatus.get()->CamPos = _CamPos; }
private:
	std::shared_ptr<PlayerStatus> m_PlayerStatus = nullptr;
};