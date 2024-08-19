#include "spch.h"
#include "CParticleScript.h"

CParticleScript::CParticleScript()
	: CScript(UINT(SCRIPT_TYPE::PARTICLESCRIPT))
{
	AddScriptParam(SCRIPT_PARAM::TEXTURE, "Texture", &m_Texture);
	//m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN] = true;
	//m_Module.SpawnRate = 20;
	//m_Module.vSpawnColor = Vec4(0.24f, 0.67f, 0.87f, 1.f);
	// 
	//m_Module.MinLife = 1.f;
	//m_Module.MaxLife = 5.f;
	//m_Module.vSpawnMinScale = Vec3(50.f, 5.f, 1.f);
	//m_Module.vSpawnMaxScale = Vec3(500.f, 50.f, 1.f);
	AddScriptParam(SCRIPT_PARAM::BOOLEAN_TOGGLE, "Spawn", &m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN]);  // �Ҹ���
	AddScriptParam(SCRIPT_PARAM::INT, "SpawnRate", &m_Module.SpawnRate);
	AddScriptParam(SCRIPT_PARAM::COLOR, "SpawnColor", &m_Module.vSpawnColor, 0, 0, u8"ƾƮ"); // �÷�
	AddScriptParam(SCRIPT_PARAM::FLOAT, "MinLife", &m_Module.MinLife);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "MaxLife", &m_Module.MaxLife);
	AddScriptParam(SCRIPT_PARAM::VEC3, "SpawnMinScale", &m_Module.vSpawnMinScale);
	AddScriptParam(SCRIPT_PARAM::VEC3, "SpawnMaxScale", &m_Module.vSpawnMaxScale);
	

	//m_Module.SpawnShape = 1;
	//m_Module.SpawnShapeScale.x = 500.f;
	AddScriptParam(SCRIPT_PARAM::INT, "SpawnShape", &m_Module.SpawnShape, 0, 0, u8"0: �ڽ� / 1: ��"); // 0 �ڽ� 1 ��
	AddScriptParam(SCRIPT_PARAM::FLOAT, "SpawnShapeScale", &m_Module.SpawnShapeScale, 0, 0, u8"����, ��� ����");


	//m_Module.BlockSpawnShape = 1;
	//m_Module.BlockSpawnShapeScale.x = 0.f;
	AddScriptParam(SCRIPT_PARAM::INT, "BlockSpawnShape", &m_Module.BlockSpawnShape, 0, 0, u8"0: �ڽ� / 1: ��"); // 0 �ڽ� 1 ��
	AddScriptParam(SCRIPT_PARAM::FLOAT, "BlockSpawnShapeScale", &m_Module.BlockSpawnShapeScale, 0, 0, u8"���� ����� �ʰ� �ϱ� ����");

	//m_Module.SpaceType = 1; // Local Space 
	AddScriptParam(SCRIPT_PARAM::INT, "SpaceType", &m_Module.SpaceType, 0, 0, u8"0: �ڽ� / 1: �� (���ý����̽�)"); // 0 �ڽ� 1 ��

	//// Spawn Burst Module
	//m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST] = true;
	//m_Module.SpawnBurstRepeat = true;
	//m_Module.SpawnBurstCount = 100;
	//m_Module.SpawnBurstRepeatTime = 3.f;
	AddScriptParam(SCRIPT_PARAM::BOOLEAN_TOGGLE, "BurstModule", &m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST], 0, 0, u8"�������� ���� �� �� ����"); // �Ҹ���
	AddScriptParam(SCRIPT_PARAM::BOOLEAN_CHECKBOX, "BurstRepeat", &m_Module.SpawnBurstRepeat); // �Ҹ���
	AddScriptParam(SCRIPT_PARAM::INT, "BurstCount", &m_Module.SpawnBurstCount, 0, 0, u8"�ݺ� Ƚ��");
	AddScriptParam(SCRIPT_PARAM::FLOAT, "BurstRepeatTime", &m_Module.SpawnBurstRepeatTime, 0, 0, u8"�ݺ��ð�(��)");


	//// Scale Module
	//m_Module.Module[(UINT)PARTICLE_MODULE::SCALE] = true;
	//m_Module.StartScale = 1.f;
	//m_Module.EndScale = 1.f;
	AddScriptParam(SCRIPT_PARAM::BOOLEAN_TOGGLE, "ScaleModule", &m_Module.Module[(UINT)PARTICLE_MODULE::SCALE], 0, 0, u8"��ƼŬ ũ�� ������ ����"); // �Ҹ���
	AddScriptParam(SCRIPT_PARAM::FLOAT, "StartScale", &m_Module.StartScale);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "EndScale", &m_Module.EndScale);

	//// AddVelocity Module
	//m_Module.Module[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = true;
	//m_Module.AddVelocityType = 1;
	//m_Module.AddVelocityFixedDir = Vec3(0.f, 1.f, 0.f);
	//m_Module.AddMinSpeed = 100.f;
	//m_Module.AddMaxSpeed = 500.f;
	AddScriptParam(SCRIPT_PARAM::BOOLEAN_TOGGLE, "VelocityModule", &m_Module.Module[(UINT)PARTICLE_MODULE::ADD_VELOCITY]); // �Ҹ���
	AddScriptParam(SCRIPT_PARAM::INT, "Type", &m_Module.AddVelocityType);
	AddScriptParam(SCRIPT_PARAM::VEC3, "FixedDir", &m_Module.AddVelocityFixedDir);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "MinSpeed", &m_Module.AddMinSpeed);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "MaxSpeed", &m_Module.AddMaxSpeed);


	//// Drag Module (����)
	//m_Module.Module[(UINT)PARTICLE_MODULE::DRAG] = false;
	//m_Module.DestNormalizedAge = 1.f;
	//m_Module.LimitSpeed = 0.f;
	AddScriptParam(SCRIPT_PARAM::BOOLEAN_TOGGLE, "DragModule", &m_Module.Module[(UINT)PARTICLE_MODULE::DRAG], 0, 0, u8"���� ���"); // �Ҹ���
	AddScriptParam(SCRIPT_PARAM::FLOAT, "NormalizedAge", &m_Module.DestNormalizedAge, 0, 0, "(interpolated)Expected to Slow down Until");
	AddScriptParam(SCRIPT_PARAM::FLOAT, "LimitSpeed", &m_Module.LimitSpeed);

	//// Noise Force Module
	//m_Module.Module[(UINT)PARTICLE_MODULE::NOISE_FORCE] = false;
	//m_Module.NoiseForceTerm = 0.3f;
	//m_Module.NoiseForceScale = 100.f;
	AddScriptParam(SCRIPT_PARAM::BOOLEAN_TOGGLE, "RandomForceModule", &m_Module.Module[(UINT)PARTICLE_MODULE::NOISE_FORCE], 0, 0, u8"�ֱ������� ��ƼŬ�� �����ϰ�\n���� �ݴϴ�"); // �Ҹ���
	AddScriptParam(SCRIPT_PARAM::FLOAT, "ForceFrequency", &m_Module.NoiseForceTerm, 0, 0, u8"�ֱ�(��)");
	AddScriptParam(SCRIPT_PARAM::FLOAT, "ForceScale", &m_Module.NoiseForceScale, 0, 0, u8"���� ũ��");

	//// Render Module
	//m_Module.Module[(UINT)PARTICLE_MODULE::RENDER] = true;
	//m_Module.EndColor = Vec3(1.f, 0.f, 0.f);
	//m_Module.FadeOut = true;
	//m_Module.FadeOutStartRatio = 0.9f;
	//m_Module.VelocityAlignment = true;
	AddScriptParam(SCRIPT_PARAM::BOOLEAN_TOGGLE, "RenderModule", &m_Module.Module[(UINT)PARTICLE_MODULE::RENDER]); // �Ҹ���
	AddScriptParam(SCRIPT_PARAM::COLOR, "EndColor", &m_Module.EndColor); // �÷�
	AddScriptParam(SCRIPT_PARAM::BOOLEAN_CHECKBOX, "FadeOut", &m_Module.FadeOut); // �Ҹ���
	AddScriptParam(SCRIPT_PARAM::FLOAT, "FadeStartRatio", &m_Module.FadeOutStartRatio);
	AddScriptParam(SCRIPT_PARAM::BOOLEAN_CHECKBOX, "VelocityAlignment", &m_Module.VelocityAlignment); // �Ҹ���

}

void CParticleScript::Begin()
{
	ParticleSystem()->SetParticleTexture(m_Texture);
	ParticleSystem()->SetParticleModule(m_Module);
}

void CParticleScript::Tick()
{
}

void CParticleScript::SaveToFile(FILE* _File)
{
	SaveAssetRef(m_Texture, _File);
}

void CParticleScript::LoadFromFile(FILE* _File)
{
	LoadAssetRef(m_Texture, _File);
}
