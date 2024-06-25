#pragma once
class CAssetMgr
	: public CSingleton<CAssetMgr>
{
	SINGLE(CAssetMgr);


public:
	void Init();

private:
};