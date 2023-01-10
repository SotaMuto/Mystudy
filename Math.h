#pragma once

/**
* @brief ���w�p���C�u����
*/

namespace Math
{
	/**
	* @brief float���ق�0.0f����Ԃ�
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
	* @brief �l���ŏ��E�ő�l�Ŋۂ߂�
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
	* @brief �l���ŏ��E�ő�l�Ń��[�v������
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
	* @brief �w��͈͂̃����_���Ȓl��Ԃ�
	*/
	inline int RandomRange(int min, int max)
	{
		return rand() % (max - min + 1) + min;
	}

	/**
	* @brief �w��͈͂̃����_���Ȓl��Ԃ�
	*/
	inline float RandomRange(float min, float max)
	{
		return min + (float)(rand()) / ((float)(RAND_MAX / (max - min)));
	}


	/**
	* @brief �w��m����true��Ԃ�
	* @param prob �m���i���j
	*/
	inline bool RandomBool(float prob)
	{
		return ((float)rand() / (float)RAND_MAX < prob);
	}


	/**
	* @brief �z�񂩂�d�݂Â����I���s��
	* @param pAry �d�݂�int�^�ɂ����z��
	* @param len �z��̗v�f��
	* @return ���I���ꂽ�z��̗v�f�ԍ�
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

			// ���̑Ώۂ𒲂ׂ�
			rnd -= pAry[i];
		}

		return pick;
	}





}