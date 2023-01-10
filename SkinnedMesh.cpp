/**
* @file SkinnedMesh.h
* @brief スキンメッシュ
* @author GP12B183 12 小峰遥一
* @details
*/

#include "stdafx.h"

/**
* @brief コンストラクタ
*/
SkinnedMesh::SkinnedMesh()
{

}


/**
* @brief デストラクタ
*/
SkinnedMesh::~SkinnedMesh()
{
	if (_fbxScene) _fbxScene->destroy();

	SAFE_DELETE(_mesh);
	SAFE_DELETE_ARRAY(_boneAry);
}


/**
* @brief 更新処理
*/
void SkinnedMesh::Update(XMMATRIX * mtxWorld, int animStackID, double animTime)
{
	if (animStackID < 0 || animStackID > _fbxScene->getAnimationStackCount() - 1) return;

	const FBXLoader::Mesh& mesh = *_fbxScene->getMesh(0);		// 0番目のメッシュのみ対応
	const FBXLoader::Geometry& geom = *mesh.getGeometry();
	const FBXLoader::AnimationStack *stack = _fbxScene->getAnimationStack(animStackID);
	const FBXLoader::TakeInfo *takeinfo = _fbxScene->getTakeInfo(stack->name);
	const FBXLoader::AnimationLayer *layer = stack->getLayer(0);
	const FBXLoader::Skin *skin = geom.getSkin();

	//double from = takeinfo->local_time_from;
	//double to = takeinfo->local_time_to;
	//double rate = (to - from) / _fbxScene->getSceneFrameRate();
	//_animTimer += rate;
	//if (_animTimer > to) _animTimer = from;	// clamp


	int clusterCnt = skin->getClusterCount();
	for (int i = 0; i < clusterCnt; i++)
	{
		const FBXLoader::Cluster *cluster = skin->getCluster(i);

		// アニメーションノードの値は親空間ベースになってるので初期姿勢行列を掛けなくて大丈夫
		const FBXLoader::AnimationCurveNode* tNode = layer->getCurveNode(*cluster->getLink(), "Lcl Translation");
		const FBXLoader::AnimationCurveNode* rNode = layer->getCurveNode(*cluster->getLink(), "Lcl Rotation");

		FBXLoader::Vec3 pos = cluster->getLocalTranslation();
		if (tNode)
		{
			pos = tNode->getNodeLocalTransform(animTime);
		}

		FBXLoader::Vec3 eularAngles;
		eularAngles = cluster->getLocalRotation();
		if (rNode)
		{
			eularAngles = rNode->getNodeLocalTransform(animTime);
		}


		if (tNode != NULL && rNode != NULL)
		{
			XMFLOAT4X4 mtx = cluster->evalLocal(pos, eularAngles).ToXMFLOAT4X4();
			_boneAry[i].CalcMtxBone(XMLoadFloat4x4(&mtx));
		}
		else
		{
			XMFLOAT4X4 mtx = _boneAry[i].GetMtxInit();
			_boneAry[i].CalcMtxBone(XMLoadFloat4x4(&mtx));
		}
	}

	XMFLOAT4X4 global;
	XMStoreFloat4x4(&global, *mtxWorld);
	Bone::UpdateBone(&_boneAry[0], &global);
}


/**
* @brief 描画処理
*/
void SkinnedMesh::Draw(void)
{
	Game.GetSkinnedMeshRenderer()->SetCombMatrix(&_mtxCombAry[0]);

	_mesh->Draw(sizeof(VertexSkinned));
}


/**
* @brief アニメーションフレームを計算
*/
double SkinnedMesh::LoopFrame(int animStackID, double animTime, float deltaTime)
{
	const FBXLoader::AnimationStack *stack = _fbxScene->getAnimationStack(animStackID);
	const FBXLoader::TakeInfo *takeinfo = _fbxScene->getTakeInfo(stack->name);

	double from = takeinfo->local_time_from;
	double to = takeinfo->local_time_to;
	double rate = (to - from) / _fbxScene->getSceneFrameRate();

	float ret = (float)Math::Loop(animTime + rate, from, to);

	Debug.Print("animation frame : %f / %f\n", ret, (float)(to -  from));

	return ret;
}



/**
* @brief 読み込み処理
*/
SkinnedMesh * SkinnedMesh::LoadFromFile(const char * filePath)
{
	FILE *fp = fopen(filePath, "rb");
	if (fp == NULL)
	{
		Debug.ErrorDialog("ファイルが開けません：%s", filePath);
		return NULL;
	}

	struct FBX_Material
	{
		char		name[256];
		Material	material;
		char		textureName[256];
	};

	struct FBX_Subset
	{
		unsigned short startIndex;
		unsigned short indexNum;
		FBX_Material   material;
	};

	// 読み込み用モデル構造体
	struct MeshData
	{
		VertexSkinned*	vertexAry;
		unsigned short	vertexNum;

		unsigned short*	indexAry;
		unsigned short	indexNum;

		FBX_Subset*		subsetAry;
		unsigned short	subsetNum;

		void Unload(void)
		{
			delete[] vertexAry;
			delete[] indexAry;
			delete[] subsetAry;
		}
	};
	MeshData data;


	fseek(fp, 0, SEEK_END);
	long fileSize = ftell(fp);

	fseek(fp, 0, SEEK_SET);
	auto* content = new FBXLoader::u8[fileSize];

	fread(content, 1, fileSize, fp);


	SkinnedMesh* skinnedMesh = new SkinnedMesh();
	skinnedMesh->_fbxScene = 
		FBXLoader::load((FBXLoader::u8*)content, fileSize, (FBXLoader::u64)FBXLoader::LoadFlags::TRIANGULATE);


	const FBXLoader::Mesh& mesh = *skinnedMesh->_fbxScene->getMesh(0);		// 0番目のメッシュのみ対応
	const FBXLoader::Geometry& geom = *mesh.getGeometry();

	// 頂点
	{
		data.vertexNum = geom.getVertexCount();
		data.vertexAry = new VertexSkinned[data.vertexNum];
		const FBXLoader::Vec3 *vertices = geom.getVertices();
		for (int i = 0; i < data.vertexNum; i++)
		{
			ZeroMemory(&data.vertexAry[i], sizeof(VertexSkinned));

			FBXLoader::Vec3 v = vertices[i];
			data.vertexAry[i].position = v.ToXMFLOAT3();
			data.vertexAry[i].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

	// 頂点インデックス
	{
		data.indexNum = geom.getIndexCount();
		data.indexAry = new unsigned short[data.indexNum];


		const int *indices = geom.getFaceIndices();
		for (int i = 0; i < (int)data.indexNum; i++)
		{
			int idx = indices[i];
			if (idx < 0) idx = idx * -1 - 1;
			data.indexAry[i] = (unsigned short)idx;
		}
	}

	// 法線
	{
		bool hasNormal = geom.getNormals() != nullptr;
		if (hasNormal)
		{
			const FBXLoader::Vec3* normals = geom.getNormals();
			int cnt = geom.getIndexCount();

			for (int i = 0; i < cnt; i++)
			{
				FBXLoader::Vec3 n = normals[i];
				data.vertexAry[data.indexAry[i]].normal = n.ToXMFLOAT3();
			}
		}
	}

	// UV
	{
		bool hasUV = geom.getUVs() != nullptr;
		if (hasUV)
		{
			const FBXLoader::Vec2* uvs = geom.getUVs();
			int cnt = geom.getIndexCount();

			for (int i = 0; i < cnt; i++)
			{
				FBXLoader::Vec2 uv = uvs[i];
				data.vertexAry[data.indexAry[i]].texCoord = uv.ToXMFLOAT2();
			}
		}
	}


	// マテリアル
	FBX_Material mat;
	ZeroMemory(&mat, sizeof(FBX_Material));
	{
		const FBXLoader::Material* fbxMat = mesh.getMaterial(0);	// とりあえず０番目のマテリアルを使う
		mat.material.ambient = fbxMat->getAmbientColor().ToXMFLOAT4((float)fbxMat->getAmbientFactor());
		mat.material.diffuse = fbxMat->getDiffuseColor().ToXMFLOAT4(1.0f);
		mat.material.emission = fbxMat->getEmissiveColor().ToXMFLOAT4((float)fbxMat->getEmissiveFactor());
		mat.material.specular = fbxMat->getSpecularColor().ToXMFLOAT4((float)fbxMat->getShininessExponent());
		mat.material.shininess = (float)fbxMat->getShininess();

		const FBXLoader::Texture* tex = fbxMat->getTexture(FBXLoader::Texture::TextureType::DIFFUSE);
		if (tex != NULL)
		{
			mat.material.noTexSampling = FALSE;

			char texName[256];
			tex->getFileName().toString(texName);

			// ファイル名だけ取り出す
			char delimiter[] = "/\"";
			strcpy(texName, strtok(texName, delimiter));

			char *p;
			while (p = strtok(NULL, delimiter))
			{
				strcpy(texName, p);
			}

			char path[256];
			strcpy(path, "data/model/");
			strcat(path, texName);

			strcpy(mat.textureName, path);
		}
		else
		{
			mat.material.noTexSampling = TRUE;
		}
	}


	// ボーン
	{
		const FBXLoader::Skin *skin = geom.getSkin();
		if (skin != NULL)
		{
			int clusterCnt = skin->getClusterCount();

			skinnedMesh->_boneNum = clusterCnt;
			skinnedMesh->_boneAry = new Bone[skinnedMesh->_boneNum];

			for (int i = 0; i < clusterCnt; i++)
			{
				const FBXLoader::Cluster *cluster = skin->getCluster(i);

				// 初期姿勢行列（ローカル空間ベース）を取得
				XMMATRIX mtxInit = XMLoadFloat4x4(&cluster->getTransformLinkMatrix().ToXMFLOAT4X4());
				skinnedMesh->_boneAry[i].Init(i, mtxInit, skinnedMesh->_mtxCombAry);

				// 頂点にウェイトをセット
				int indicesCnt = cluster->getIndicesCount();
				const int *indices = cluster->getIndices();			// 影響を与える頂点インデックス
				const double *weights = cluster->getWeights();		// ウェイト
				for (int j = 0; j < indicesCnt; j++)
				{
					data.vertexAry[indices[j]].SetWeight(i, (float)weights[j]);	// ウェイトが大きい順にセットしてくれる
				}

				// 親子関係をセット
				FBXLoader::Object *parent = cluster->getLink()->getParent();
				for (int j = 0; j < clusterCnt; j++)
				{
					if (skin->getCluster(j)->getLink() == parent)
					{
						skinnedMesh->_boneAry[j].SetFirstChild(&skinnedMesh->_boneAry[i]);	// 既に最初の子ボーンがセットされていれば兄弟ボーンとしてセットしてくれる
						break;
					}
				}
			}

			// 各頂点のウェイトを規格化
			for (int i = 0; i < data.vertexNum; i++)
			{
				data.vertexAry[i].NormalizeWeight();
			}
		}

		// 初期姿勢行列をローカル空間ベース→親空間ベースに変換
		Bone::CalcRelativeMat(&skinnedMesh->_boneAry[0], NULL);
	}

	// ファイル読み込み終了
	delete[] content;
	fclose(fp);

	// サブセット作成（とりあえず１個）
	data.subsetAry = new FBX_Subset[1];
	data.subsetNum = 1;
	data.subsetAry[0].material = mat;
	data.subsetAry[0].indexNum = data.indexNum;
	data.subsetAry[0].startIndex = 0;

	// サブセット設定
	MeshSubset *meshSubsetAry = new MeshSubset[data.subsetNum];

	for (int i = 0; i < data.subsetNum; i++)
	{
		meshSubsetAry[i].startIndex = data.subsetAry[i].startIndex;
		meshSubsetAry[i].indexNum = data.subsetAry[i].indexNum;
		meshSubsetAry[i].material = data.subsetAry[i].material.material;
		if (data.subsetAry[i].material.material.noTexSampling == FALSE)
		{
			meshSubsetAry[i].pTexture = LoadTexture(data.subsetAry[i].material.textureName);
		}
	}

	// メッシュデータ生成
	skinnedMesh->_mesh = 
		new Mesh(data.vertexAry, data.vertexNum, data.indexAry, data.indexNum, meshSubsetAry, data.subsetNum);

	// バッファ作り終えたら削除
	data.Unload();

	return skinnedMesh;
}