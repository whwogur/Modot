#pragma once
#include "global.h"

//===============
// Struct of FBX 
//===============
struct tFbxMaterial
{
	tMtrlData	tMtrl;
	wstring     strMtrlName;
	wstring		strDiff;
	wstring		strNormal;
	wstring		strSpec;
	wstring		strEmis;
};

struct tWeightsAndIndices
{
	int		iBoneIdx;
	double	dWeight;
};

struct tContainer
{
	wstring											strName;
	std::vector<Vec3>								vecPos;
	std::vector<Vec3>								vecTangent;
	std::vector<Vec3>								vecBinormal;
	std::vector<Vec3>								vecNormal;
	std::vector<Vec4>								vecColor;
	std::vector<Vec2>								vecUV;

	std::vector<Vec4>								vecIndices;
	std::vector<Vec4>								vecWeights;

	std::vector<std::vector<UINT>>					vecIdx;
	std::vector<tFbxMaterial>						vecMtrl;

	// Animation °ü·Ã Á¤º¸
	bool											bAnimation;
	std::vector<std::vector<tWeightsAndIndices>>	vecWI;

	void Resize(UINT _iSize)
	{
		vecPos.resize(_iSize);
		vecTangent.resize(_iSize);
		vecBinormal.resize(_iSize);
		vecNormal.resize(_iSize);
		vecUV.resize(_iSize);
		vecColor.resize(_iSize);
		vecIndices.resize(_iSize);
		vecWeights.resize(_iSize);
		vecWI.resize(_iSize);
	}
};

struct tKeyFrame
{
	FbxAMatrix  matTransform;
	double		dTime;
};

struct tBone
{
	wstring					strBoneName;
	int						iDepth;             // °èÃþ±¸Á¶ ±íÀÌ
	int						iParentIndx;        // ºÎ¸ð Bone ÀÇ ÀÎµ¦½º
	FbxAMatrix				matOffset;   // Offset Çà·Ä( -> »Ñ¸® -> Local)
	FbxAMatrix				matBone;
	std::vector<tKeyFrame>	vecKeyFrame;
};

struct tAnimClip
{
	wstring		strName;
	FbxTime		tStartTime;
	FbxTime		tEndTime;
	FbxLongLong	llTimeLength;
	FbxTime::EMode eMode;
};



class CMesh;

class CFBXLoader
{
public:
	CFBXLoader();
	~CFBXLoader();

public:
	void init();
	void LoadFbx(const wstring& _strPath);

public:
	int GetContainerCount() { return (int)m_vecContainer.size(); }
	const tContainer& GetContainer(int _iIdx) { return m_vecContainer[_iIdx]; }
	std::vector<tBone*>& GetBones() { return m_vecBone; }
	std::vector<tAnimClip*>& GetAnimClip() { return m_vecAnimClip; }

private:
	void LoadMeshDataFromNode(FbxNode* _pRoot);
	void LoadMesh(FbxMesh* _pFbxMesh);
	void LoadMaterial(FbxSurfaceMaterial* _pMtrlSur);

	void GetTangent(FbxMesh* _pMesh, tContainer* _pContainer, int _iIdx, int _iVtxOrder);
	void GetBinormal(FbxMesh* _pMesh, tContainer* _pContainer, int _iIdx, int _iVtxOrder);
	void GetNormal(FbxMesh* _pMesh, tContainer* _pContainer, int _iIdx, int _iVtxOrder);
	void GetUV(FbxMesh* _pMesh, tContainer* _pContainer, int _iIdx, int _iVtxOrder);
	void GetColor(FbxMesh* _pMesh, tContainer* _pContainer, int _iIdx, int _iVtxOrder);

	Vec4 GetMtrlData(FbxSurfaceMaterial* _pSurface, const char* _pMtrlName, const char* _pMtrlFactorName);
	wstring GetMtrlTextureName(FbxSurfaceMaterial* _pSurface, const char* _pMtrlProperty);

	void LoadTexture();
	void CreateMaterial();

	// Animation
	void LoadSkeleton(FbxNode* _pNode);
	void LoadSkeleton_Re(FbxNode* _pNode, int _iDepth, int _iIdx, int _iParentIdx);
	void LoadAnimationClip();
	void Triangulate(FbxNode* _pNode);

	void LoadAnimationData(FbxMesh* _pMesh, tContainer* _pContainer);
	void LoadWeightsAndIndices(FbxCluster* _pCluster, int _iBoneIdx, tContainer* _pContainer);
	void LoadOffsetMatrix(FbxCluster* _pCluster, const FbxAMatrix& _matNodeTransform, int _iBoneIdx, tContainer* _pContainer);
	void LoadKeyframeTransform(FbxNode* _pNode, FbxCluster* _pCluster, const FbxAMatrix& _matNodeTransform
		, int _iBoneIdx, tContainer* _pContainer);

	int FindBoneIndex(string _strBoneName);
	FbxAMatrix GetTransform(FbxNode* _pNode);

	void CheckWeightAndIndices(FbxMesh* _pMesh, tContainer* _pContainer);

private:
	FbxManager* m_pManager;
	FbxScene* m_pScene;
	FbxImporter* m_pImporter;

	std::vector<tContainer>				m_vecContainer;

	// Animation
	std::vector<tBone*>					m_vecBone;
	FbxArray<FbxString*>			m_arrAnimName;
	std::vector<tAnimClip*>				m_vecAnimClip;
};

