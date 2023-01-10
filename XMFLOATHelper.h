#pragma once

/**
* @file XMFLOATHelper.h
* @brief XMFLOAT型の演算子オーバーロード
* @author GP12B183 12 小峰遥一
* @details
*/

//! 許容誤差
#define EPSILON (0.01f)

// プロトタイプ宣言
static bool AdjEqual(float f1, float f2);


/**
* @brief floatとfloat同士の比較
*/
static bool AdjEqual(float f1, float f2)
{
	return (fabsf(f1 - f2) < EPSILON);
}


// XMFLOAT2

#pragma region XMFLOAT2

static XMFLOAT2 operator+ (const XMFLOAT2& v1, const XMFLOAT2& v2)
{
	return { v1.x + v2.x, v1.y + v2.y };
}

static XMFLOAT2 operator- (const XMFLOAT2& v1, const XMFLOAT2& v2)
{
	return { v1.x - v2.x, v1.y - v2.y };
}

static XMFLOAT2 operator* (const XMFLOAT2& v1, const XMFLOAT2& v2)
{
	return { v1.x * v2.x, v1.y * v2.y };
}

static XMFLOAT2 operator/ (const XMFLOAT2& v1, const XMFLOAT2& v2)
{
	return { v1.x / v2.x, v1.y / v2.y };
}

static XMFLOAT2 operator% (const XMFLOAT2& v1, const XMFLOAT2& v2)
{
	return { fmodf(v1.x, v2.x), fmodf(v1.y, v2.y) };
}

static void operator+= (XMFLOAT2& v1, const XMFLOAT2& v2)
{
	v1.x += v2.x;
	v1.y += v2.y;
}

static void operator-= (XMFLOAT2& v1, const XMFLOAT2& v2)
{
	v1.x -= v2.x;
	v1.y -= v2.y;
}

static void operator*= (XMFLOAT2& v1, const XMFLOAT2& v2)
{
	v1.x *= v2.x;
	v1.y *= v2.y;
}

static void operator/= (XMFLOAT2& v1, const XMFLOAT2& v2)
{
	v1.x /= v2.x;
	v1.y /= v2.y;
}

static void operator%= (XMFLOAT2& v1, const XMFLOAT2& v2)
{
	v1.x = fmodf(v1.x, v2.x);
	v1.y = fmodf(v1.y, v2.y);
}

static XMFLOAT2 operator+ (const XMFLOAT2& v1, const float& num)
{
	return { v1.x + num, v1.y + num };
}

static XMFLOAT2 operator- (const XMFLOAT2& v1, const float& num)
{
	return { v1.x - num, v1.y - num };
}

static XMFLOAT2 operator* (const XMFLOAT2& v1, const float& num)
{
	return { v1.x * num, v1.y * num };
}

static XMFLOAT2 operator/ (const XMFLOAT2& v1, const float& num)
{
	return { v1.x / num, v1.y / num };
}

static XMFLOAT2 operator% (const XMFLOAT2& v1, const float& num)
{
	return { fmodf(num, v1.x), fmodf(num, v1.y) };
}

static bool operator== (const XMFLOAT2& v1, const XMFLOAT2& v2)
{
	return (AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y));
}

static bool operator!= (const XMFLOAT2& v1, const XMFLOAT2& v2)
{
	return !(AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y));
}

#pragma endregion




// XMFLOAT3

#pragma region XMFLOAT3

static XMFLOAT3 operator+ (const XMFLOAT3& v1, const XMFLOAT3& v2)
{
	return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

static XMFLOAT3 operator- (const XMFLOAT3& v1, const XMFLOAT3& v2)
{
	return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

static XMFLOAT3 operator* (const XMFLOAT3& v1, const XMFLOAT3& v2)
{
	return { v1.x * v2.x, v1.y * v2.y, v1.z * v2.z };
}

static XMFLOAT3 operator/ (const XMFLOAT3& v1, const XMFLOAT3& v2)
{
	return { v1.x / v2.x, v1.y / v2.y, v1.z / v2.z };
}

static XMFLOAT3 operator% (const XMFLOAT3& v1, const XMFLOAT3& v2)
{
	return { fmodf(v1.x, v2.x), fmodf(v1.y, v2.y), fmodf(v1.z, v2.z) };
}

static void operator+= (XMFLOAT3& v1, const XMFLOAT3& v2)
{
	v1.x += v2.x;
	v1.y += v2.y;
	v1.z += v2.z;
}

static void operator-= (XMFLOAT3& v1, const XMFLOAT3& v2)
{
	v1.x -= v2.x;
	v1.y -= v2.y;
	v1.z -= v2.z;
}

static void operator*= (XMFLOAT3& v1, const XMFLOAT3& v2)
{
	v1.x *= v2.x;
	v1.y *= v2.y;
	v1.z *= v2.z;
}

static void operator/= (XMFLOAT3& v1, const XMFLOAT3& v2)
{
	v1.x /= v2.x;
	v1.y /= v2.y;
	v1.z /= v2.z;
}

static void operator%= (XMFLOAT3& v1, const XMFLOAT3& v2)
{
	v1.x = fmodf(v1.x, v2.x);
	v1.y = fmodf(v1.y, v2.y);
	v1.z = fmodf(v1.z, v2.z);
}

static XMFLOAT3 operator+ (const XMFLOAT3& v1, const float& num)
{
	return { v1.x + num, v1.y + num, v1.z + num };
}

static XMFLOAT3 operator- (const XMFLOAT3& v1, const float& num)
{
	return { v1.x - num, v1.y - num, v1.z - num };
}

static XMFLOAT3 operator* (const XMFLOAT3& v1, const float& num)
{
	return { v1.x * num, v1.y * num, v1.z * num };
}

static XMFLOAT3 operator/ (const XMFLOAT3& v1, const float& num)
{
	return { v1.x / num, v1.y / num, v1.z / num };
}

static XMFLOAT3 operator% (const XMFLOAT3& v1, const float& num)
{
	return { fmodf(num, v1.x), fmodf(num, v1.y), fmodf(num, v1.z) };
}

static bool operator== (const XMFLOAT3& v1, const XMFLOAT3& v2)
{
	return (AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y) && AdjEqual(v1.z, v2.z));
}

static bool operator!= (const XMFLOAT3& v1, const XMFLOAT3& v2)
{
	return !(AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y) && AdjEqual(v1.z, v2.z));
}

#pragma endregion




// XMFLOAT4

#pragma region XMFLOAT4

static XMFLOAT4 operator+ (const XMFLOAT4& v1, const XMFLOAT4& v2)
{
	return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w };
}

static XMFLOAT4 operator- (const XMFLOAT4& v1, const XMFLOAT4& v2)
{
	return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w };
}

static XMFLOAT4 operator* (const XMFLOAT4& v1, const XMFLOAT4& v2)
{
	return { v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w };
}

static XMFLOAT4 operator/ (const XMFLOAT4& v1, const XMFLOAT4& v2)
{
	return { v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w };
}

static XMFLOAT4 operator% (const XMFLOAT4& v1, const XMFLOAT4& v2)
{
	return { fmodf(v1.x, v2.x), fmodf(v1.y, v2.y), fmodf(v1.z, v2.z), fmodf(v1.w, v2.w) };
}

static void operator+= (XMFLOAT4& v1, const XMFLOAT4& v2)
{
	v1.x += v2.x;
	v1.y += v2.y;
	v1.z += v2.z;
	v1.w += v2.w;
}

static void operator-= (XMFLOAT4& v1, const XMFLOAT4& v2)
{
	v1.x -= v2.x;
	v1.y -= v2.y;
	v1.z -= v2.z;
	v1.w -= v2.w;
}

static void operator*= (XMFLOAT4& v1, const XMFLOAT4& v2)
{
	v1.x *= v2.x;
	v1.y *= v2.y;
	v1.z *= v2.z;
	v1.w *= v2.w;
}

static void operator/= (XMFLOAT4& v1, const XMFLOAT4& v2)
{
	v1.x /= v2.x;
	v1.y /= v2.y;
	v1.z /= v2.z;
	v1.w /= v2.w;
}

static void operator%= (XMFLOAT4& v1, const XMFLOAT4& v2)
{
	v1.x = fmodf(v1.x, v2.x);
	v1.y = fmodf(v1.y, v2.y);
	v1.z = fmodf(v1.z, v2.z);
	v1.w = fmodf(v1.w, v2.w);
}

static XMFLOAT4 operator+ (const XMFLOAT4& v1, const float& num)
{
	return { v1.x + num, v1.y + num, v1.z + num, v1.w + num };
}

static XMFLOAT4 operator- (const XMFLOAT4& v1, const float& num)
{
	return { v1.x - num, v1.y - num, v1.z - num, v1.w - num };
}

static XMFLOAT4 operator* (const XMFLOAT4& v1, const float& num)
{
	return { v1.x * num, v1.y * num, v1.z * num, v1.w * num };
}

static XMFLOAT4 operator/ (const XMFLOAT4& v1, const float& num)
{
	return { v1.x / num, v1.y / num, v1.z / num, v1.w / num };
}

static XMFLOAT4 operator% (const XMFLOAT4& v1, const float& num)
{
	return { fmodf(num, v1.x), fmodf(num, v1.y), fmodf(num, v1.z), fmodf(num, v1.w) };
}

static bool operator== (const XMFLOAT4& v1, const XMFLOAT4& v2)
{
	return (AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y) && AdjEqual(v1.z, v2.z) && AdjEqual(v1.w, v2.w));
}

static bool operator!= (const XMFLOAT4& v1, const XMFLOAT4& v2)
{
	return !(AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y) && AdjEqual(v1.z, v2.z) && AdjEqual(v1.w, v2.w));
}

#pragma endregion




// 型変換

static XMVECTOR ToXMVECTOR(const XMFLOAT4& vec)
{
	return XMLoadFloat4(&vec);
}

static XMVECTOR ToXMVECTOR(const XMFLOAT3& vec)
{
	return XMLoadFloat3(&vec);
}

static XMVECTOR ToXMVECTOR(const XMFLOAT2& vec)
{
	return XMLoadFloat2(&vec);
}

static XMVECTOR ToXMVECTOR(const float& vec)
{
	return XMLoadFloat(&vec);
}

static XMVECTOR ToNormalizeXMVECTOR(const XMFLOAT4& vec)
{
	return XMVector4Normalize(XMLoadFloat4(&vec));
}

static XMVECTOR ToNormalizeXMVECTOR(const XMFLOAT3& vec)
{
	return XMVector3Normalize(XMLoadFloat3(&vec));
}

static XMVECTOR ToNormalizeXMVECTOR(const XMFLOAT2& vec)
{
	return XMVector2Normalize(XMLoadFloat2(&vec));
}

static XMFLOAT2 XMFLOAT2Normalize(const XMFLOAT2& vec)
{
	XMFLOAT2 ret;
	XMStoreFloat2(&ret, XMVector2Normalize(XMLoadFloat2(&vec)));
	return ret;
}

static XMFLOAT3 XMFLOAT3Normalize(const XMFLOAT3& vec)
{
	XMFLOAT3 ret;
	XMStoreFloat3(&ret, XMVector3Normalize(XMLoadFloat3(&vec)));
	return ret;
}

static XMFLOAT4 XMFLOAT4Normalize(const XMFLOAT4& vec)
{
	XMFLOAT4 ret;
	XMStoreFloat4(&ret, XMVector4Normalize(XMLoadFloat4(&vec)));
	return ret;
}

static XMFLOAT4 ToXMFLOAT4(const XMVECTOR& vec)
{
	XMFLOAT4 ret;
	XMStoreFloat4(&ret, vec);
	return ret;
}

static XMFLOAT3 ToXMFLOAT3(const XMVECTOR& vec)
{
	XMFLOAT3 ret;
	XMStoreFloat3(&ret, vec);
	return ret;
}

static XMFLOAT2 ToXMFLOAT2(const XMVECTOR& vec)
{
	XMFLOAT2 ret;
	XMStoreFloat2(&ret, vec);
	return ret;
}

static float ToXMFLOAT(const XMVECTOR& vec)
{
	float ret;
	XMStoreFloat(&ret, vec);
	return ret;
}

static XMFLOAT4 RaiseToXMFLOAT4(const XMFLOAT3& vec, const float w = 0.0f)
{
	return XMFLOAT4{ vec.x, vec.y, vec.z, w };
}

static XMFLOAT4 RaiseToXMFLOAT4(const XMFLOAT2& vec, const float z = 0.0f, const float w = 0.0f)
{
	return XMFLOAT4{ vec.x, vec.y, z, w };
}

static XMFLOAT4 RaiseToXMFLOAT4(const float& vec, const float y = 0.0f, const float z = 0.0f, const float w = 0.0f)
{
	return XMFLOAT4{ vec, y, z, w };
}

static XMFLOAT3 RaiseToXMFLOAT3(const XMFLOAT2& vec, const float z = 0.0f)
{
	return XMFLOAT3{ vec.x, vec.y, z };
}

static XMFLOAT2 LowerToXMFLOAT2(const XMFLOAT4& vec)
{
	return XMFLOAT2{ vec.x, vec.y };
}

static XMFLOAT2 LowerToXMFLOAT2(const XMFLOAT3& vec)
{
	return XMFLOAT2{ vec.x, vec.y };
}

static XMFLOAT3 LowerToXMFLOAT3(const XMFLOAT4& vec)
{
	return XMFLOAT3{ vec.x, vec.y, vec.z };
}



// 初期化

static XMFLOAT2 XMFLOAT2Zero(void)
{
	return XMFLOAT2{ 0.0f, 0.0f };
}

static XMFLOAT3 XMFLOAT3Zero(void)
{
	return XMFLOAT3{ 0.0f, 0.0f, 0.0f };
}

static XMFLOAT4 XMFLOAT4Zero(void)
{
	return XMFLOAT4{ 0.0f, 0.0f, 0.0f, 0.0f };
}



// イージング


// 線形補間
static float XMFLOATLerp(const float start, const float end, const float t)
{
	return start + (end - start) * t;
}

static XMFLOAT2 XMFLOAT2Lerp(const XMFLOAT2 start, const XMFLOAT2 end, const float t)
{
	return XMFLOAT2{ start.x + (end.x - start.x) * t, start.y + (end.y - start.y) * t };
}

static XMFLOAT3 XMFLOAT3Lerp(const XMFLOAT3 start, const XMFLOAT3 end, const float t)
{
	return XMFLOAT3{ start.x + (end.x - start.x) * t, start.y + (end.y - start.y) * t, start.z + (end.z - start.z) * t };
}

static XMFLOAT4 XMFLOAT4Lerp(const XMFLOAT4 start, const XMFLOAT4 end, const float t)
{
	return XMFLOAT4{ start.x + (end.x - start.x) * t, start.y + (end.y - start.y) * t, start.z + (end.z - start.z) * t, start.w + (end.w - start.w) * t };
}


// Ease-In
static float XMFLOATEaseIn(const float start, const float end, const float t)
{
	return XMFLOATLerp(start, end, t * t);
}

static XMFLOAT2 XMFLOAT2EaseIn(const XMFLOAT2 start, const XMFLOAT2 end, const float t)
{
	return XMFLOAT2Lerp(start, end, t * t);
}

static XMFLOAT3 XMFLOAT3EaseIn(const XMFLOAT3 start, const XMFLOAT3 end, const float t)
{
	return XMFLOAT3Lerp(start, end, t * t);
}

static XMFLOAT4 XMFLOAT4EaseIn(const XMFLOAT4 start, const XMFLOAT4 end, const float t)
{
	return XMFLOAT4Lerp(start, end, t * t);
}


// Ease-Out
static float XMFLOATEaseOut(const float start, const float end, const float t)
{
	return XMFLOATLerp(start, end, t * (2.0f - t));
}

static XMFLOAT2 XMFLOAT2EaseOut(const XMFLOAT2 start, const XMFLOAT2 end, const float t)
{
	return XMFLOAT2Lerp(start, end, t * (2.0f - t));
}

static XMFLOAT3 XMFLOAT3EaseOut(const XMFLOAT3 start, const XMFLOAT3 end, const float t)
{
	return XMFLOAT3Lerp(start, end, t * (2.0f - t));
}

static XMFLOAT4 XMFLOAT4EaseOut(const XMFLOAT4 start, const XMFLOAT4 end, const float t)
{
	return XMFLOAT4Lerp(start, end, t * (2.0f - t));
}


// Cubic（初速０、終速０）
static float XMFLOATEaseInOut(const float start, const float end, const float t)
{
	float ret;
	float a = (start - end) * 2.0f;
	float b = (end - start) * 3.0f;

	ret = a;
	ret = ret * t;
	ret += b;
	ret = ret * t;
	ret = ret * t;		// c=0なので何も足さない
	ret += start;

	return ret;
}

static XMFLOAT2 XMFLOAT2EaseInOut(const XMFLOAT2 start, const XMFLOAT2 end, const float t)
{
	XMFLOAT2 ret;
	XMFLOAT2 a = (start - end) * 2.0f;
	XMFLOAT2 b = (end - start) * 3.0f;

	ret = a;
	ret = ret * t;
	ret += b;
	ret = ret * t;
	ret = ret * t;		// c=0なので何も足さない
	ret += start;

	return ret;
}

static XMFLOAT3 XMFLOAT3EaseInOut(const XMFLOAT3 start, const XMFLOAT3 end, const float t)
{
	XMFLOAT3 ret;
	XMFLOAT3 a = (start - end) * 2.0f;
	XMFLOAT3 b = (end - start) * 3.0f;

	ret = a;
	ret = ret * t;
	ret += b;
	ret = ret * t;
	ret = ret * t;		// c=0なので何も足さない
	ret += start;

	return ret;
}

static XMFLOAT4 XMFLOAT4EaseInOut(const XMFLOAT4 start, const XMFLOAT4 end, const float t)
{
	XMFLOAT4 ret;
	XMFLOAT4 a = (start - end) * 2.0f;
	XMFLOAT4 b = (end - start) * 3.0f;

	ret = a;
	ret = ret * t;
	ret += b;
	ret = ret * t;
	ret = ret * t;		// c=0なので何も足さない
	ret += start;

	return ret;
}

//
//// Bezier
//static float XMFLOATBezier(XMFLOAT3 *points, const int num, const float t)
//{
//	XMFLOAT3 p = CalcBezier(&points[0], num, t);
//	return p.x;
//}
//
//static XMFLOAT2 XMFLOAT2Bezier(XMFLOAT3 *points, const int num, const float t)
//{
//	XMFLOAT3 p = CalcBezier(&points[0], num, t);
//	return LowerToXMFLOAT2(p);
//}
//
//static XMFLOAT3 XMFLOAT3Bezier(XMFLOAT3 *points, const int num, const float t)
//{
//	XMFLOAT3 p = CalcBezier(&points[0], num, t);
//	return p;
//}


// Ease-OutBack
static float XMFLOATEaseInBack(const float start, const float end, const float t)
{
	const float c1 = 1.70158f;
	const float c3 = c1 + 1.0f;

	float a = c3 * t * t * t - c1 * t * t;

	return XMFLOATLerp(start, end, t);
}


// 内積・外積

/**
* @brief 内積
*/
static float Dot(XMVECTOR *v1, XMVECTOR *v2)
{
	XMVECTOR temp = XMVector3Dot(*v1, *v2);
	float ans = 0.0f;
	XMStoreFloat(&ans, temp);

	//float ans = v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;

	return(ans);
}


/**
* @brief 外積
*/
static void Cross(XMVECTOR *ret, XMVECTOR *v1, XMVECTOR *v2)
{
	*ret = XMVector3Cross(*v1, *v2);

	//ret->x = v1->y * v2->z - v1->z * v2->y;
	//ret->y = v1->z * v2->x - v1->x * v2->z;
	//ret->z = v1->x * v2->y - v1->y * v2->x;
}

static float XMFLOAT3Dot(const XMFLOAT3 v1, const XMFLOAT3 v2)
{
	return Dot(&ToXMVECTOR(v1), &ToXMVECTOR(v2));
}

static XMFLOAT3 XMFLOAT3Cross(const XMFLOAT3 v1, const XMFLOAT3 v2)
{
	XMVECTOR ret;
	Cross(&ret, &ToXMVECTOR(v1), &ToXMVECTOR(v2));
	return ToXMFLOAT3(ret);
}


// 距離
static float XMFLOAT3LengthSq(const XMFLOAT3 v)
{
	return (v.x * v.x + v.y * v.y + v.z * v.z);
}

static float XMFLOAT3Length(const XMFLOAT3 v)
{
	return sqrtf(XMFLOAT3LengthSq(v));
}



// クオータニオン

// クオータニオンの掛け算
static XMFLOAT4 QuaternionMultiply(XMFLOAT4 quat1, XMFLOAT4 quat2)
{
	XMFLOAT3 q1, q2, ans;
	XMVECTOR Q1, Q2, Q3;

	q1 = XMFLOAT3(quat1.x, quat1.y, quat1.z);
	q2 = XMFLOAT3(quat2.x, quat2.y, quat2.z);

	Q1 = XMLoadFloat3(&q1);
	Q2 = XMLoadFloat3(&q2);

	Cross(&Q3, &Q1, &Q2);

	XMStoreFloat3(&ans, (Q1 * quat2.w + Q2 * quat1.w + Q3));

	return XMFLOAT4(ans.x, ans.y, ans.z, (quat1.w * quat2.w - Dot(&Q1, &Q2)));
}


// ベクトルの回転
static XMFLOAT3 VectorRotation(XMFLOAT3 vec, XMFLOAT4 quat)
{
	XMFLOAT4 qinv = XMFLOAT4(-quat.x, -quat.y, -quat.z, quat.w);

	XMFLOAT4 v;

	v = QuaternionMultiply(quat, QuaternionMultiply(XMFLOAT4(vec.x, vec.y, vec.z, 0.0f), qinv));

	return XMFLOAT3(v.x, v.y, v.z);
}


// 回転からクオータニオンを生成
static XMFLOAT4 RotationToQuaternion(XMFLOAT3 rot)
{
	XMFLOAT3 axis;
	float angle;

	XMStoreFloat(&angle, XMVector3Length(XMLoadFloat3(&rot)));
	XMStoreFloat3(&axis, XMVector3Normalize(XMLoadFloat3(&rot)));

	XMFLOAT4 ans = XMFLOAT4(axis.x * sinf(angle * 0.5f), axis.y * sinf(angle * 0.5f), axis.z * sinf(angle * 0.5f), cosf(angle * 0.5f));

	return ans;
}


// クオータニオンから回転を生成
static XMFLOAT3 QuaternionToRotation(XMFLOAT4 quat)
{
	float angle = acosf(quat.w) * 2.0f;

	if (fabsf(angle) < 0.001f)
	{
		return XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	XMFLOAT3 axis = XMFLOAT3(quat.x / sinf(angle * 0.5f), quat.y / sinf(angle * 0.5f), quat.z / sinf(angle * 0.5f));

	return XMFLOAT3(axis.x * angle, axis.y * angle, axis.z * angle);
}


// クオータニオンの回転
static XMFLOAT4 QuaternionRotation(XMFLOAT4 q, XMFLOAT4 quat)
{
	XMFLOAT4 q1;

	q1 = QuaternionMultiply(q, quat);

	XMVECTOR Q = XMLoadFloat4(&q1);
	Q = XMVector4Normalize(Q);
	XMStoreFloat4(&q1, Q);

	return q1;

}

// クオータニオンの回転
static XMFLOAT4 QuaternionRotation(XMFLOAT3 axis, float angle, XMFLOAT4 quat)
{
	XMFLOAT4 q1, q2;
	XMVECTOR Q2;

	q1 = XMFLOAT4(axis.x * sinf(angle * 0.5f), axis.y * sinf(angle * 0.5f),
		axis.z * sinf(angle * 0.5f), cosf(angle * 0.5f));

	q2 = QuaternionMultiply(q1, quat);

	Q2 = XMVector4Normalize(XMLoadFloat4(&q2));
	XMStoreFloat4(&q2, Q2);

	return q2;
}

// 回転軸と回転角からクォータニオンを生成する
static XMFLOAT4 CreateQuaternion(XMFLOAT3 axis, float angle)
{
	XMFLOAT4 q1;

	q1 = XMFLOAT4(axis.x * sinf(angle * 0.5f), axis.y * sinf(angle * 0.5f), axis.z * sinf(angle * 0.5f), cosf(angle * 0.5f));

	return q1;

}
