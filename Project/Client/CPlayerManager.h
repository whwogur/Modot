#pragma once
class PlayerStatus
{
public:
	PlayerStatus()
		: HP(100.f)
		, MP(100.f)
		, Stamina(100.f)
		, maxHP(100.f)
		, maxMP(100.f)
		, maxStamina(100.f)
		, Damage(5.f)
	{}
	~PlayerStatus() = default;

	float	HP;
	float	MP;
	float	Stamina;

	float	maxHP;
	float	maxMP;
	float	maxStamina;

	float	Damage;
	bool	Sprinting = false;
};

class CPlayerManager
	: public CSingleton<CPlayerManager>
{
	SINGLE(CPlayerManager)
public:
	void Init();
	const std::shared_ptr<PlayerStatus>& GetPlayerStatusRef() { return m_PlayerStatus; }

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
	void Recover(float _HP)
	{ 
		m_PlayerStatus.get()->HP += _HP; 
		if (m_PlayerStatus.get()->HP > m_PlayerStatus.get()->maxHP)
			m_PlayerStatus.get()->HP = m_PlayerStatus.get()->maxHP;
	}

	void Sprint(bool _b) { m_PlayerStatus.get()->Sprinting = _b; }
private:
	std::shared_ptr<PlayerStatus> m_PlayerStatus = nullptr;
};