#pragma once

/**
* @file ManageMemory.h
* @brief �������Ǘ��p�}�N���̒�`
* @author GP12B183 12 �����y��
* @details
*/

// �C���^�[�t�F�C�X���
#define SAFE_RELEASE(x)       if(x){ x->Release();x=NULL; }
#define SAFE_DELETE(x)        if(x){ delete x;x=NULL; }
#define SAFE_DELETE_ARRAY(x)  if(x){ delete[] x;x=NULL; }
#define SAFE_DELOBJ(p)        if(p){ DeleteObject(p); (p)=NULL; }
