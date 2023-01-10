#pragma once

/**
* @file DebugProc.h
* @brief �f�o�b�O����
* @author GP12B183 12 �����y��
* @details
*/


/**
* @brief �f�o�b�O�p�N���X
*/
class DebugProc
{
private:
	static const int		DEBUG_STR_MAX = 2048;	// �ő啶����
	static const RECT		DEBUG_PRINT_RECT;		// �f�o�b�O�\���ӏ�
	static const COLORREF	DEBUG_PRINT_COLOR;		// �f�o�b�O�����F

	char _printBuff[DEBUG_STR_MAX] = { "\0" };	// ��ʕ\���p�̃f�o�b�O������

public:
	//! �o�͉�ʂɕ\��
	void Out(const char *str, ...);
	//! ��ʍ���ɕ\��
	void Print(const char *str, ...);
	//! �_�C�A���O�ŕ\��
	void ErrorDialog(const char *str, ...);
	//! �t�@�C���ɏ�������
	void ExportFile(const char* fileName, const char *str, ...);

	void Draw(void);


	// --------------�V���O���g����������--------------
private:
	static DebugProc* _instance;

	DebugProc() {}	// �R���X�g���N�^�̓v���C�x�[�g�ɉB��
	~DebugProc() {}
public:
	// �C���X�^���X����
	static void CreateInstance()
	{
		DeleteInstance();

		_instance = new DebugProc();
	}
	// �C���X�^���X�폜
	static void DeleteInstance()
	{
		if (_instance != nullptr)
		{
			delete _instance;
			_instance = nullptr;
		}
	}
	// �C���X�^���X�擾
	static DebugProc& GetInstance()
	{
		return *_instance;
	}
	// --------------�V���O���g�������܂�--------------
};

/**
* @brief �C���X�^���X���ȒP�Ɏ擾���邽�߂̃}�N��
*/
#define Debug DebugProc::GetInstance()