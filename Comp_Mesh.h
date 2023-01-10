#pragma once

/**
* @file MeshComponent.h
* @brief ���b�V���̕`��
* @author GP12B183 12 �����y��
* @details
*/


/**
* ���b�V���̕`����s���R���|�[�l���g
*/
class MeshComponent : public Component
{
private:
	Mesh* _pMesh = NULL;	// ���b�V���f�[�^�ւ̃|�C���^

	BOOL _enableDepthTest = TRUE;


public:
	MeshComponent(Actor* pOwner, Mesh* mesh);
	virtual ~MeshComponent();

	void SetDepthTest(BOOL flg){ _enableDepthTest = flg; }

	Mesh* GetMesh(void) { return _pMesh; }
	void SetMesh(Mesh* pMesh) { _pMesh = pMesh; }

	virtual void Draw(void);
};