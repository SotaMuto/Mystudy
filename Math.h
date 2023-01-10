#pragma once

/**
* @brief 数学用ライブラリ
*/

namespace Math
{
	/**
	* @brief floatがほぼ0.0fかを返す
	*/
	inline bool NearZero(float v, float epsilon = 0.001f)
	{
		if (fabsf(v) <= epsilon)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	/**
	* @brief 値を最小・最大値で丸める
	*/
	template <typename T>
	T Clamp(const T& v, const T& min, const T& max)
	{
		if (v < min)
		{
			return min;
		}
		else if (v > max)
		{
			return max;
		}	
		return v;
	}


	/**
	* @brief 値を最小・最大値でループさせる
	*/
	template <typename T>
	T Loop(const T& v, const T& min, const T& max)
	{
		if (v < min)
		{
			return max;
		}
		else if (v > max)
		{
			return min;
		}
		return v;
	}



	/**
	* @brief 指定範囲のランダムな値を返す
	*/
	inline int RandomRange(int min, int max)
	{
		return rand() % (max - min + 1) + min;
	}

	/**
	* @brief 指定範囲のランダムな値を返す
	*/
	inline float RandomRange(float min, float max)
	{
		return min + (float)(rand()) / ((float)(RAND_MAX / (max - min)));
	}


	/**
	* @brief 指定確率でtrueを返す
	* @param prob 確率（％）
	*/
	inline bool RandomBool(float prob)
	{
		return ((float)rand() / (float)RAND_MAX < prob);
	}


	/**
	* @brief 配列から重みづけ抽選を行う
	* @param pAry 重みをint型にした配列
	* @param len 配列の要素数
	* @return 抽選された配列の要素番号
	*/
	int WeightedPick(int *pAry, int len)
	{
		int totalWeight = 0;
		int pick = 0;

		for (int i = 0; i < len; i++)
		{
			totalWeight += pAry[i];
		}

		int rnd = rand() % totalWeight;

		for (int i = 0; i < len; i++)
		{
			if (rnd < pAry[i])
			{
				pick = i;
				break;
			}

			// 次の対象を調べる
			rnd -= pAry[i];
		}

		return pick;
	}





}