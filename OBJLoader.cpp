#include "stdafx.h"

const float OBJLoader::SCALE_MODEL = 10.0f;

OBJLoader::OBJLoader()
{
}

OBJLoader::~OBJLoader()
{
	Unload();
}

Mesh * OBJLoader::LoadFromFile(const char * filePath)
{
	// �g���q���擾
	char fname[256];
	strcpy(fname, filePath);

	char *fileEx;
	fileEx = strtok(fname, ".");
	fileEx = strtok(NULL, ".");

	// obj�ȊO�͒e��
	if (strcmp(fileEx, "obj") != 0)
	{
		Debug.Out("�g���q�G���[�F%s\n", fileEx);
		return NULL;
	}

	FILE *fp;
	fp = fopen(filePath, "rt");
	if (fp == NULL)
	{
		Debug.ErrorDialog(".obj�t�@�C����������܂���F%s", filePath);
		return NULL;
	}

	//
	Unload();

	unsigned short	posNum = 0;
	unsigned short	norNum = 0;
	unsigned short	uvNum = 0;
	unsigned short	in = 0;

	char str[256];
	char *s;
	char c;

	// �v�f���J�E���g
	while (true)
	{
		fscanf(fp, "%s", str);

		if (feof(fp) != 0) break;

		if (strcmp(str, "v") == 0)
		{
			posNum++;
		}
		else if (strcmp(str, "vn") == 0)
		{
			norNum++;
		}
		else if (strcmp(str, "vt") == 0)
		{
			uvNum++;
		}
		else if (strcmp(str, "usemtl") == 0)
		{
			_subsetNum++;
		}
		else if (strcmp(str, "f") == 0)
		{
			in = 0;

			do
			{
				fscanf(fp, "%s", str);
				_vertexNum++;
				in++;
				c = fgetc(fp);
			} while (c != '\n' && c != '\r');

			// �l�p�͎O�p�ɕ���
			if (in == 4)
				in = 6;

			_indexNum += in;
		}
	}


	// �������m��
	XMFLOAT3* posAry = new XMFLOAT3[posNum];
	XMFLOAT3* norAry = new XMFLOAT3[norNum];
	XMFLOAT2* uvAry  = new XMFLOAT2[uvNum];

	_vertexAry = new Vertex3D[_vertexNum];
	_indexAry  = new unsigned short[_indexNum];
	_subsetAry = new OBJ_Subset[_subsetNum];

	XMFLOAT3 *pPos = &posAry[0];
	XMFLOAT3 *pNor = &norAry[0];
	XMFLOAT2 *pUV  = &uvAry[0];

	unsigned short vc = 0;
	unsigned short ic = 0;
	unsigned short sc = 0;

	fseek(fp, 0, SEEK_SET);

	while (true)
	{
		fscanf(fp, "%s", str);

		if (feof(fp) != 0)
			break;

		if (strcmp(str, "mtllib") == 0)
		{
			// �}�e���A���t�@�C��
			fscanf(fp, "%s", str);

			char path[256];
			strcpy(path, "data/model/");
			strcat(path, str);

			LoadMaterial(path);
		}
		else if (strcmp(str, "o") == 0)
		{
			// �I�u�W�F�N�g��
			fscanf(fp, "%s", str);
		}
		else if (strcmp(str, "v") == 0)
		{
			// ���_���W
			fscanf(fp, "%f", &pPos->x);
			fscanf(fp, "%f", &pPos->y);
			fscanf(fp, "%f", &pPos->z);
			pPos->x *= SCALE_MODEL;
			pPos->y *= SCALE_MODEL;
			pPos->z *= SCALE_MODEL;
			pPos++;
		}
		else if (strcmp(str, "vn") == 0)
		{
			// �@��
			fscanf(fp, "%f", &pNor->x);
			fscanf(fp, "%f", &pNor->y);
			fscanf(fp, "%f", &pNor->z);
			pNor++;
		}
		else if (strcmp(str, "vt") == 0)
		{
			// �e�N�X�`�����W
			fscanf(fp, "%f", &pUV->x);
			fscanf(fp, "%f", &pUV->y);
			pUV->y = 1.0f - pUV->y;
			pUV++;
		}
		else if (strcmp(str, "usemtl") == 0)
		{
			// �}�e���A��
			fscanf(fp, "%s", str);

			if (sc != 0)
				_subsetAry[sc - 1].indexNum = ic - _subsetAry[sc - 1].startIndex;

			_subsetAry[sc].startIndex = ic;

			for (unsigned short i = 0; i < _matNum; i++)
			{
				if (strcmp(str, _matAry[i].name) == 0)
				{
					_subsetAry[sc].material.material = _matAry[i].material;
					strcpy(_subsetAry[sc].material.textureName, _matAry[i].textureName);
					strcpy(_subsetAry[sc].material.name, _matAry[i].name);

					break;
				}
			}

			sc++;
		}
		else if (strcmp(str, "f") == 0)
		{
			//��
			in = 0;

			do
			{
				fscanf(fp, "%s", str);

				s = strtok(str, "/");
				_vertexAry[vc].position = posAry[atoi(s) - 1];
				if (s[strlen(s) + 1] != '/')
				{
					// �e�N�X�`�����W�����݂��Ȃ��ꍇ������
					s = strtok(NULL, "/");
					_vertexAry[vc].texCoord = uvAry[atoi(s) - 1];
				}
				s = strtok(NULL, "/");
				_vertexAry[vc].normal = norAry[atoi(s) - 1];

				_vertexAry[vc].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

				_indexAry[ic] = vc;
				ic++;
				vc++;

				in++;
				c = fgetc(fp);
			} while (c != '\n' && c != '\r');

			// �l�p�͎O�p�ɕ���
			if (in == 4)
			{
				_indexAry[ic] = vc - 4;
				ic++;
				_indexAry[ic] = vc - 2;
				ic++;
			}
		}
	}


	if (sc != 0)
		_subsetAry[sc - 1].indexNum = ic - _subsetAry[sc - 1].startIndex;

	delete[] posAry;
	delete[] norAry;
	delete[] uvAry;

	fclose(fp);

	return CreateMesh();
}


void OBJLoader::LoadMaterial(const char * filePath)
{
	char str[256];

	FILE *file;
	file = fopen(filePath, "rt");
	if (file == NULL)
	{
		Debug.ErrorDialog(".mtl�t�@�C����������܂���F%s", filePath);
		return;
	}

	// 
	SAFE_DELETE_ARRAY(_matAry);
	_matNum = 0;

	// �v�f���J�E���g
	while (true)
	{
		fscanf(file, "%s", str);

		if (feof(file) != 0)
			break;


		if (strcmp(str, "newmtl") == 0)
		{
			_matNum++;
		}
	}


	// �������m��
	_matAry = new OBJ_Material[_matNum];
	ZeroMemory(_matAry, sizeof(OBJ_Material) * _matNum);


	// �v�f�Ǎ�
	int mc = -1;

	fseek(file, 0, SEEK_SET);

	while (true)
	{
		fscanf(file, "%s", str);

		if (feof(file) != 0)
			break;


		// �}�e���A����
		if (strcmp(str, "newmtl") == 0)
		{
			mc++;
			fscanf(file, "%s", _matAry[mc].name);
			strcpy(_matAry[mc].textureName, "");
			_matAry[mc].material.noTexSampling = 1;
		}
		// �A���r�G���g
		else if (strcmp(str, "Ka") == 0)
		{
			fscanf(file, "%f", &_matAry[mc].material.ambient.x);
			fscanf(file, "%f", &_matAry[mc].material.ambient.y);
			fscanf(file, "%f", &_matAry[mc].material.ambient.z);
			_matAry[mc].material.ambient.w = 1.0f;
		}
		// �f�B�t���[�Y
		else if (strcmp(str, "Kd") == 0)
		{
			fscanf(file, "%f", &_matAry[mc].material.diffuse.x);
			fscanf(file, "%f", &_matAry[mc].material.diffuse.y);
			fscanf(file, "%f", &_matAry[mc].material.diffuse.z);

			// Maya�Ńe�N�X�`����\���0.0f�ɂȂ����Ⴄ�݂����Ȃ̂�
			if ((_matAry[mc].material.diffuse.x + _matAry[mc].material.diffuse.y + _matAry[mc].material.diffuse.z) == 0.0f)
			{
				_matAry[mc].material.diffuse.x = _matAry[mc].material.diffuse.y = _matAry[mc].material.diffuse.z = 1.0f;
			}

			_matAry[mc].material.diffuse.w = 1.0f;
		}
		// �X�y�L����
		else if (strcmp(str, "Ks") == 0)
		{
			fscanf(file, "%f", &_matAry[mc].material.specular.x);
			fscanf(file, "%f", &_matAry[mc].material.specular.y);
			fscanf(file, "%f", &_matAry[mc].material.specular.z);
			_matAry[mc].material.specular.w = 1.0f;
		}
		// �X�y�L�����p�x�H
		else if (strcmp(str, "Ns") == 0)
		{
			fscanf(file, "%f", &_matAry[mc].material.shininess);
		}
		// �A���t�@
		else if (strcmp(str, "d") == 0)
		{
			fscanf(file, "%f", &_matAry[mc].material.diffuse.w);
		}
		// �X�y�L�������x�H
		else if (strcmp(str, "Ni") == 0)
		{
			fscanf(file, "%f", &_matAry[mc].material.specular.w);
		}
		// �Ɩ����f��
		else if (strcmp(str, "illum") == 0)
		{

		}
		// �e�N�X�`��
		else if (strcmp(str, "map_Kd") == 0)
		{
			fscanf(file, "%s", str);

			char path[256];
			strcpy(path, "data/model/");
			strcat(path, str);

			strcat(_matAry[mc].textureName, path);
			_matAry[mc].material.noTexSampling = 0;
		}
	}

	fclose(file);
}

Mesh * OBJLoader::CreateMesh(void)
{
	MeshSubset *meshSubsetAry = new MeshSubset[_subsetNum];

	for (int i = 0; i < _subsetNum; i++)
	{
		meshSubsetAry[i].startIndex = _subsetAry[i].startIndex;
		meshSubsetAry[i].indexNum   = _subsetAry[i].indexNum;
		meshSubsetAry[i].material   = _subsetAry[i].material.material;
		if (_subsetAry[i].material.material.noTexSampling == FALSE)
		{
			meshSubsetAry[i].pTexture = LoadTexture(_subsetAry[i].material.textureName);
		}
	}

	Mesh* mesh = new Mesh(_vertexAry, _vertexNum, _indexAry, _indexNum, meshSubsetAry, _subsetNum);

	return mesh;
}

void OBJLoader::Unload(void)
{
	SAFE_DELETE_ARRAY(_vertexAry);
	SAFE_DELETE_ARRAY(_indexAry);
	SAFE_DELETE_ARRAY(_subsetAry);
	SAFE_DELETE_ARRAY(_matAry);

	_vertexNum = 0;
	_indexNum  = 0;
	_subsetNum = 0;
	_matNum    = 0;
}
