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
	{}
	~PlayerStatus() = default;

	int		HP;
	int		MP;
	int		Stamina;
	int		maxHP;
	int		maxMP;
	int		maxStamina;

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

	void DisplayStats();
private:
	std::shared_ptr<PlayerStatus> m_PlayerStatus = nullptr;
};