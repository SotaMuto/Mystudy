#pragma once

/**
* @file Actor.h
* @brief 全てのゲームオブジェクトの基底クラス
* @author GP12B183 12 小峰遥一
* @details 階層構造とコンポーネントベースをハイブリッドにしてる
*/


/**
* @brief アクタークラス
*/
class Actor
{
public:
	// アクターの状態
	enum STATE
	{
		ACTIVE,	// アクティブ	
		PAUSE,	// deleteはされないが更新されない
		DEAD,	// 次のフレームでdeleteされる
	};

private:
	Actor::STATE	_state = ACTIVE;						// 現在の状態
	XMFLOAT3		_pos   = { 0.0f, 0.0f, 0.0f };			// 座標
	XMFLOAT3		_scl   = { 1.0f, 1.0f, 1.0f };			// 拡縮
	XMFLOAT4		_quat  = { 0.0f, 0.0f, 0.0f, 1.0f };	// クオータニオン
	XMFLOAT4X4		_mtxWorld;								// ワールドマトリクス
	XMFLOAT4X4		_mtxWorldParent;						// 親のワールドマトリクス
	bool			_hasParent = false;
	bool			_needRecalcMtxWorld = true;				// ワールドマトリクスを再計算するフラグ
	std::vector<Component*> _components;					// 所有しているコンポーネント

public:
	Actor();
	virtual ~Actor();

	// 入力
	void ProcessInput(const InputSystem& input);
	virtual void ActorInput(const InputSystem& input);

	// 更新
	void Update(float deltaTime);
	void UpdateComponents(float deltaTime);
	virtual void UpdateActor(float deltaTime);

	//! 移動
	void Translate(XMFLOAT3 translate) { SetPos(GetPos() + translate); }
	void Translate(float x, float y, float z) { Translate(XMFLOAT3(x, y, z)); }

	//! 回転
	void Rotate(XMFLOAT3 angle) { SetRot(GetRot() + angle); }
	void Rotate(float x, float y, float z) { Rotate(XMFLOAT3(x, y, z)); }

	//! 拡縮
	void Scale(XMFLOAT3 scl) { SetScl(GetScl() + scl); }
	void Scale(float x, float y, float z) { Scale(XMFLOAT3(x, y, z)); }

	//! ワールドマトリクス計算
	void CalcWorldMatrix(void);

#pragma region Getter/Setter
	Actor::STATE GetState(void) const { return _state; }
	void SetState(Actor::STATE state) { _state = state; }

	XMFLOAT3 GetPos(void) const { return _pos; }
	void SetPos(XMFLOAT3 pos) { _pos = pos; _needRecalcMtxWorld = true; }
	void SetPos(float x, float y, float z) { SetPos(XMFLOAT3(x, y, z)); }

	XMFLOAT3 GetScl(void) const { return _scl; }
	void SetScl(XMFLOAT3 scl) { _scl = scl; _needRecalcMtxWorld = true; }
	void SetScl(float x, float y, float z) { SetScl(XMFLOAT3(x, y, z)); }

	XMFLOAT3 GetRot(void) const { return QuaternionToRotation(_quat); }
	void SetRot(XMFLOAT3 rot) { _quat = RotationToQuaternion(rot); _needRecalcMtxWorld = true; }
	void SetRot(float x, float y, float z) { SetRot(XMFLOAT3(x, y, z)); }

	XMFLOAT4 GetQuat(void) const { return _quat; }
	void SetQuat(XMFLOAT4 quat) { _quat = quat; _needRecalcMtxWorld = true; }
	void SetQuat(float x, float y, float z, float w) { SetQuat(XMFLOAT4(x, y, z, w)); }

	void SetParentWorldMatrix(XMFLOAT4X4 mtxParent) { _mtxWorldParent = mtxParent; _hasParent = true; }

	//! 前ベクトル
	XMFLOAT3 GetForward(void) const { return VectorRotation(XMFLOAT3(0.0f, 0.0f, 1.0f), GetQuat()); }
	//! 右ベクトル
	XMFLOAT3 GetRight(void) const { return XMFLOAT3Cross(GetForward(), XMFLOAT3(0.0f, 1.0f, 0.0f)); }
	//! 指定座標を向く
	void LookAt(XMFLOAT3 target, XMFLOAT3 up = { 0.0f, 1.0f, 0.0f });

	//! ワールドマトリクス
	XMFLOAT4X4 GetWorldMatrix(void) const 
	{ 
		XMFLOAT4X4 mtxWorld = _mtxWorld;

		if (_hasParent)
		{
			XMStoreFloat4x4(&mtxWorld, 
				XMLoadFloat4x4(&_mtxWorld) * XMLoadFloat4x4(&_mtxWorldParent));
		}

		return mtxWorld;
	}
#pragma endregion

	//! コンポーネント追加/削除
	void AddComponent(Component* component);
	void RemoveComponent(Component* component);
};