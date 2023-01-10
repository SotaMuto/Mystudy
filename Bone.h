#pragma once

/**
* @file MeshComponent.cpp
* @brief �{�[������
* @author GP12B183 12 �����y��
* @details
*/


/**
* @brief �{�[��
*/
class Bone
{
private:
	int			_id;			// ID
	Bone*		_pFirstChild;	// �ŏ��̎q�{�[���ւ̃|�C���^
	Bone*		_pSibling;		// �Z��{�[���ւ̃|�C���^

	XMFLOAT4X4	_mtxInit;		// �����p���s��
	XMFLOAT4X4	_mtxOffset;		// �I�t�Z�b�g�s��i�����p���s��̋t�s��j
	XMFLOAT4X4	_mtxBone;		// �����ϊ��s��
	XMFLOAT4X4*	_pMtxCombAry;	// �����ϊ��s��z��ւ̃|�C���^

public:
	Bone() {};
	~Bone() {};

	XMFLOAT4X4 GetMtxInit(void) { return _mtxInit; }
	void SetFirstChild(Bone *pFirstChild);
	void SetSibling(Bone *pSibling);

	void Init(int id, XMMATRIX mtxInit, XMFLOAT4X4 *pMtxCombAry);
	void CalcMtxBone(XMMATRIX mtxFrame)
	{
		//XMMATRIX mtxBone = XMMatrixMultiply(mtxFrame, XMLoadFloat4x4(&_mtxInit));
		XMStoreFloat4x4(&_mtxBone, mtxFrame);
		//XMStoreFloat4x4(&_pMtxCombAry[_id], mtxBone);
	}

	void SetMtxOffset(void)
	{
		XMStoreFloat4x4(&_mtxOffset, XMMatrixInverse(nullptr, XMLoadFloat4x4(&_mtxInit)));
	}

	static void CalcRelativeMat(Bone *me, XMFLOAT4X4 *mtxParetnOffset)
	{
		if (me->_pFirstChild)
			CalcRelativeMat(me->_pFirstChild, &me->_mtxOffset);

		if (me->_pSibling)
			CalcRelativeMat(me->_pSibling, mtxParetnOffset);

		if (mtxParetnOffset)
			XMStoreFloat4x4(&me->_mtxInit, XMMatrixMultiply(XMLoadFloat4x4(&me->_mtxInit), XMLoadFloat4x4(mtxParetnOffset)));
	}

	static void UpdateBone(Bone *me, XMFLOAT4X4 *mtxParetnWorld)
	{
		XMStoreFloat4x4(&me->_mtxBone, 
			XMMatrixMultiply(XMLoadFloat4x4(&me->_mtxBone), XMLoadFloat4x4(mtxParetnWorld)));

		XMStoreFloat4x4(&me->_pMtxCombAry[me->_id], 
			XMMatrixMultiply(XMLoadFloat4x4(&me->_mtxOffset), XMLoadFloat4x4(&me->_mtxBone)));

		if (me->_pFirstChild)
			UpdateBone(me->_pFirstChild, &me->_mtxBone);

		if (me->_pSibling)
			UpdateBone(me->_pSibling, mtxParetnWorld);
	}
};