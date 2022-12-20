#include "stdafx.h"
#include "..\public\Channel.h"

#include "Bone.h"
#include "Model.h"


CChannel::CChannel()
{
}

HRESULT CChannel::Save_Channel(HANDLE& hFile, DWORD& dwByte)
{
	_uint			iNameLength = (_uint)m_strName.length() + 1;
	const char*	pName = m_strName.c_str();
	WriteFile(hFile, &iNameLength, sizeof(_uint), &dwByte, nullptr);
	WriteFile(hFile, pName, sizeof(char) * iNameLength, &dwByte, nullptr);

	WriteFile(hFile, &m_iNumKeyframes, sizeof(_uint), &dwByte, nullptr);

	for (auto& tKeyFrame : m_vecKeyframes)
		WriteFile(hFile, &tKeyFrame, sizeof(KEYFRAME), &dwByte, nullptr);

	return S_OK;
}

HRESULT CChannel::Load_Channel(HANDLE& hFile, DWORD& dwByte)
{
	_uint			iNameLength = 0;
	ReadFile(hFile, &iNameLength, sizeof(_uint), &dwByte, nullptr);

	char*			pName = new char[iNameLength];
	ReadFile(hFile, pName, sizeof(char) * iNameLength, &dwByte, nullptr);

	m_strName = pName;

	m_pBone = m_pModel->Get_BonePtr(m_strName);
	Safe_AddRef(m_pBone);

	Safe_Delete_Array(pName);

	ReadFile(hFile, &m_iNumKeyframes, sizeof(_uint), &dwByte, nullptr);
	m_vecKeyframes.reserve(m_iNumKeyframes);

	for (_uint i = 0; i < m_iNumKeyframes; ++i)
	{
		KEYFRAME		tKeyFrame;
		ZeroMemory(&tKeyFrame, sizeof(KEYFRAME));

		ReadFile(hFile, &tKeyFrame, sizeof(KEYFRAME), &dwByte, nullptr);

		m_vecKeyframes.push_back(tKeyFrame);
	}

	return S_OK;
}

// Ư�� �ִϸ��̼ǿ��� ���Ǵ� ������ ������
HRESULT CChannel::Initialize(aiNodeAnim* pAIChannel, CModel* pModel)
{
	m_pModel = pModel;

	if (pAIChannel == nullptr)
		return S_OK;

	m_strName = pAIChannel->mNodeName.data;

	m_pBone = pModel->Get_BonePtr(m_strName); // �𵨿��� �����ϴ� ���� ä�ο��� �ҷ��� ���� �̸��� ���� ��� �ּҸ� �Ѱ��ش�.
	Safe_AddRef(m_pBone);
	// ���� �������� Ű�������� ���� ���Ƽ�, �� ������ ���� ã�� ���� �ԾƳ��� �۾�
	m_iNumKeyframes = max(pAIChannel->mNumScalingKeys, pAIChannel->mNumRotationKeys);
	m_iNumKeyframes = max(m_iNumKeyframes, pAIChannel->mNumPositionKeys);

	m_vecKeyframes.reserve(m_iNumKeyframes);

	_float3		vScale;
	_float4		vRotation;
	_float3		vPosition;

	for(_uint i =0 ; i< m_iNumKeyframes; ++i)
	{
		KEYFRAME	KeyFrame;
		ZeroMemory(&KeyFrame, sizeof(KEYFRAME));

		if (i < pAIChannel->mNumScalingKeys)
		{
			memcpy(&vScale, &pAIChannel->mScalingKeys[i].mValue, sizeof(_float3));
			KeyFrame.dTime = pAIChannel->mScalingKeys[i].mTime;
		}

		if (i < pAIChannel->mNumRotationKeys)
		{
			vRotation.x = pAIChannel->mRotationKeys[i].mValue.x;
			vRotation.y = pAIChannel->mRotationKeys[i].mValue.y;
			vRotation.z = pAIChannel->mRotationKeys[i].mValue.z;
			vRotation.w = pAIChannel->mRotationKeys[i].mValue.w;
			KeyFrame.dTime = pAIChannel->mRotationKeys[i].mTime;
		}

		if(i< pAIChannel->mNumPositionKeys)
		{
			memcpy(&vPosition, &pAIChannel->mPositionKeys[i].mValue, sizeof(_float3));
			KeyFrame.dTime = pAIChannel->mPositionKeys[i].mTime;
		}
		KeyFrame.vScale = vScale;
		KeyFrame.vRotation = vRotation;
		KeyFrame.vPosition = vPosition;

		m_vecKeyframes.push_back(KeyFrame);
	}
	return S_OK;
}

void CChannel::Update_TransformMatrix(_double dPlayTime)
{
	// �ִϸ��̼��� ����ϴ� Ư�� ������ ���� ����� �������ִ� �Լ�

	_vector vScale;
	_vector vRotation;
	_vector vPosition;
	_matrix TransformMatrix;

	

	// ���� ����� �ð��� �� �ڿ��ִ� Ű �������� �ð����� Ŀ���� (�ԾƳ� Ű ������)
	// ������ Ű������ �����͸� ���ܵα����� �۾�
	if(dPlayTime >= m_vecKeyframes.back().dTime)
	{
		m_vLastScale = vScale =	XMLoadFloat3(&m_vecKeyframes.back().vScale);
		m_vLastRotation = vRotation = XMLoadFloat4(&m_vecKeyframes.back().vRotation);
		m_vLastPosition = vPosition = XMLoadFloat3(&m_vecKeyframes.back().vPosition);
		m_vLastPosition = vPosition = XMVectorSetW(vPosition, 1.f);
	}
	else
	{
		while (dPlayTime >= m_vecKeyframes[m_iCurrentKeyframeIndex + 1].dTime)
		{
			// �ִϸ��̼� ����ð��� Ű������ ���̿� ������ ��
			// �÷���Ÿ�� �ð��� ���� Ű������ �ð����� Ŭ �� ���� Ű������ �ε����� ����
			++m_iCurrentKeyframeIndex;
		}
		// ���� �ð� - ���� Ű������ �ε��� �ð� / ���� Ű������ �ε��� �ð� - ���� �ð� 
		_double	dRatio = (dPlayTime - m_vecKeyframes[m_iCurrentKeyframeIndex].dTime)
			/ (m_vecKeyframes[m_iCurrentKeyframeIndex + 1].dTime - m_vecKeyframes[m_iCurrentKeyframeIndex].dTime);
		// �� ������ Ű ������ ������ ������ ���� ���� �������ֱ� ���� �����̴�.

		vScale = XMVectorLerp(XMLoadFloat3(&m_vecKeyframes[m_iCurrentKeyframeIndex].vScale), XMLoadFloat3(&m_vecKeyframes[m_iCurrentKeyframeIndex + 1].vScale), (_float)dRatio);
		vRotation = XMQuaternionSlerp(XMLoadFloat4(&m_vecKeyframes[m_iCurrentKeyframeIndex].vRotation), XMLoadFloat4(&m_vecKeyframes[m_iCurrentKeyframeIndex + 1].vRotation), (_float)dRatio);
		vPosition = XMVectorLerp(XMLoadFloat3(&m_vecKeyframes[m_iCurrentKeyframeIndex].vPosition), XMLoadFloat3(&m_vecKeyframes[m_iCurrentKeyframeIndex + 1].vPosition), (_float)dRatio);
		vPosition = XMVectorSetW(vPosition, 1.f);
		if ("104_Teammate" == m_strName)
		{
			vPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);
		}
		if("Bip001 Footsteps001" == m_strName)
		{
			// CBone* dest = m_pModel->Get_BonePtr("Bip001 Prop1");
			// if (dest == nullptr)
			// 	return;
			// _matrix tmp = dest->Get_TransformMatrix();
			// _vector vS, vR, vP;
			// XMMatrixDecompose(&vS, &vR, &vP, tmp);
			// vPosition = vP ;
			vPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);

		}
		// if ("Bip001" == m_strName)
		// {
		// 	CBone*	dest = m_pModel->Get_BonePtr("Bip001001");
		// 	if (dest == nullptr)
		// 		return;
		// 	_matrix tmp = dest->Get_TransformMatrix();
		//
		// 	_vector vS, vR, vP;
		// 	XMMatrixDecompose(&vS, &vR, &vP, tmp);
		// 	vPosition = vP;
		// }
	}
	// ���͵��� �޾Ƽ� ��ķ� ������ִ� �Լ�. 2��° ���ڴ� �������ε� ������ ����.
	TransformMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

	m_pBone->Set_TransformMatrix(TransformMatrix);
}

_bool CChannel::Update_TransformLerpMatrix(_double dPlayTime, CChannel* CurrentChannel, CChannel* NextChannel,_double LerpSpeed, _bool bFinish)
{
	_vector vScale;
	_vector vRotation;
	_vector vPosition;
	_matrix TransformMatrix;

	if(-1 == m_iLerpFrameIndex) // ���� ���� �� ���� �ε��� ���� ������ �ʱ�ȭ 
	{
		while(dPlayTime >= CurrentChannel->m_vecKeyframes[m_iLerpFrameIndex + 1].dTime)
		{
			m_iLerpFrameIndex++;

			if((_int)m_iNumKeyframes <= m_iLerpFrameIndex +1)
			{
				m_iLerpFrameIndex -= 1;
				break;
			}
		}
	}

	_uint iSour = 0;

	if(bFinish)
	{
		iSour = m_iNumKeyframes - 1;
	}
	else
	{
		iSour = m_iLerpFrameIndex;
	}

	_uint iDest = 0;

	if (CurrentChannel->m_vecKeyframes.size() <= iSour)
		iSour = (_uint)CurrentChannel->m_vecKeyframes.size() - 1;

	m_dLerpRatio += LerpSpeed;

	_vector	vLastScale, vCurScale;
	_vector vLastRotation, vCurRotation;
	_vector vLastPosition, vCurPosition;

	vLastScale = XMLoadFloat3(&CurrentChannel->m_vecKeyframes[iSour].vScale);
	vCurScale = XMLoadFloat3(&NextChannel->m_vecKeyframes[iDest].vScale);

	vLastRotation = XMLoadFloat4(&CurrentChannel->m_vecKeyframes[iSour].vRotation);
	vCurRotation = XMLoadFloat4(&NextChannel->m_vecKeyframes[iDest].vRotation);

	vLastPosition = XMLoadFloat3(&CurrentChannel->m_vecKeyframes[iSour].vPosition);
	vCurPosition = XMLoadFloat3(&NextChannel->m_vecKeyframes[iDest].vPosition);

	vScale = XMVectorLerp(vLastScale, vCurScale, static_cast<_float>(m_dLerpRatio));
	vRotation = XMQuaternionSlerp(vLastRotation, vCurRotation, static_cast<_float>(m_dLerpRatio));
	vPosition = XMVectorLerp(vLastPosition, vCurPosition, static_cast<_float>(m_dLerpRatio));
	vPosition = XMVectorSetW(vPosition, 1.f);
	if ("104_Teammate" == m_strName)
	{
		vPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	}
	// if ("Bip001 Footsteps" == m_strName)
	// {
	// 	CBone*	dest = m_pModel->Get_BonePtr("Bip001 Footsteps001");
	// 	if (dest == nullptr)
	// 		return true;
	//
	// 	_matrix tmp = dest->Get_TransformMatrix();
	//
	// 	_vector vS, vR, vP;
	// 	XMMatrixDecompose(&vS, &vR, &vP, tmp);
	// 	vPosition = vP;
	// }
	// if ("1205_Bone001" == m_strName)
	// {
	// 	CBone* dest = m_pModel->Get_BonePtr("Bip001 R Finger11");
	// 	if (dest == nullptr)
	// 		return true;
	// 	_matrix tmp = dest->Get_TransformMatrix();
	// 	_vector vS, vR, vP;
	// 	XMMatrixDecompose(&vS, &vR, &vP, tmp);
	// 	vPosition = vP;
	// }
	if ("Bip001 Footsteps001" == m_strName)
	{
		// CBone* dest = m_pModel->Get_BonePtr("Bip001 Prop1");
		// if (dest == nullptr)
		// 	return true;
		// _matrix tmp = dest->Get_TransformMatrix();
		// _vector vS, vR, vP;
		// XMMatrixDecompose(&vS, &vR, &vP, tmp);
		vPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);
		return true;
	}
	TransformMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

	m_pBone->Set_TransformMatrix(TransformMatrix);

	if (1.f <= m_dLerpRatio)
	{
		m_dLerpRatio = 0.0;
		m_iLerpFrameIndex = -1;
		return true;
	}

	return false;
}

void CChannel::Update_Blend(_double dPlayTime, _float fRatio)
{
	_vector	vBaseScale, vBaseRotation, vBasePosition;
	_vector	vScale, vRotation, vPosition;
	_matrix	matTransform = m_pBone->Get_TransformMatrix();

	XMMatrixDecompose(&vBaseScale, &vBaseRotation, &vBasePosition, matTransform);

	if (dPlayTime >= m_vecKeyframes.back().dTime)
	{
		vScale = XMLoadFloat3(&m_vecKeyframes.back().vScale);
		vRotation = XMLoadFloat4(&m_vecKeyframes.back().vRotation);
		vPosition = XMLoadFloat3(&m_vecKeyframes.back().vPosition);
		vPosition = XMVectorSetW(vPosition, 1.f);
	}
	else
	{
		while (dPlayTime >= m_vecKeyframes[m_iCurrentKeyframeIndex + 1].dTime)
			++m_iCurrentKeyframeIndex;

		_float fTmp = _float((dPlayTime - m_vecKeyframes[m_iCurrentKeyframeIndex].dTime)
			/ (m_vecKeyframes[m_iCurrentKeyframeIndex + 1].dTime - m_vecKeyframes[m_iCurrentKeyframeIndex].dTime));

		vScale = XMVectorLerp(XMLoadFloat3(&m_vecKeyframes[m_iCurrentKeyframeIndex].vScale), XMLoadFloat3(&m_vecKeyframes[m_iCurrentKeyframeIndex + 1].vScale), fTmp);
		vRotation = XMQuaternionSlerp(XMLoadFloat4(&m_vecKeyframes[m_iCurrentKeyframeIndex].vRotation), XMLoadFloat4(&m_vecKeyframes[m_iCurrentKeyframeIndex + 1].vRotation), fTmp);
		vPosition = XMVectorLerp(XMLoadFloat3(&m_vecKeyframes[m_iCurrentKeyframeIndex].vPosition), XMLoadFloat3(&m_vecKeyframes[m_iCurrentKeyframeIndex + 1].vPosition), fTmp);
		vPosition = XMVectorSetW(vPosition, 1.f);

		if ("104_Teammate" == m_strName)
		{
			vPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);
		}
		// if ("Bip001 Footsteps" == m_strName)
		// {
		// 	vPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);
		// }
		// if ("Bip001 Footsteps.001" == m_strName)
		// {
		// 	vPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);
		// }
	}

	vScale = XMVectorLerp(vBaseScale, vScale, fRatio);
	vRotation = XMQuaternionSlerp(vBaseRotation, vRotation, fRatio);
	vPosition = XMVectorLerp(vBasePosition, vPosition, fRatio);
	vPosition = XMVectorSetW(vPosition, 1.f);


	matTransform = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

	m_pBone->Set_TransformMatrix(matTransform);
}

void CChannel::Update_Additive(_double dPlayTime, _float fRatio)
{
	_vector vBaseScale, vBaseRot, vBasePos;
	XMMatrixDecompose(&vBaseScale, &vBaseRot, &vBasePos, m_pBone->Get_TransformMatrix());

	_vector			vScale;
	_vector			vRotation;
	_vector			vPosition;

	/* ���� ����� �ð��� ������ Ű�����ӽð����� Ŀ����.�� */
	if (dPlayTime >= m_vecKeyframes.back().dTime)
	{
		vScale = XMLoadFloat3(&m_vecKeyframes.back().vScale);
		vRotation = XMLoadFloat4(&m_vecKeyframes.back().vRotation);
		vPosition = XMLoadFloat3(&m_vecKeyframes.back().vPosition);
		vPosition = XMVectorSetW(vPosition, 1.f);
	}
	else
	{
		while (dPlayTime >= m_vecKeyframes[m_iCurrentKeyframeIndex + 1].dTime)
		{
			++m_iCurrentKeyframeIndex;
		}

		_double			Ratio = (dPlayTime - m_vecKeyframes[m_iCurrentKeyframeIndex].dTime) /
			(m_vecKeyframes[m_iCurrentKeyframeIndex + 1].dTime - m_vecKeyframes[m_iCurrentKeyframeIndex].dTime);

		_vector			vSourScale, vDestScale;
		_vector			vSourRotation, vDestRotation;
		_vector			vSourPosition, vDestPosition;

		vSourScale = XMLoadFloat3(&m_vecKeyframes[m_iCurrentKeyframeIndex].vScale);
		vSourRotation = XMLoadFloat4(&m_vecKeyframes[m_iCurrentKeyframeIndex].vRotation);
		vSourPosition = XMLoadFloat3(&m_vecKeyframes[m_iCurrentKeyframeIndex].vPosition);

		vDestScale = XMLoadFloat3(&m_vecKeyframes[m_iCurrentKeyframeIndex + 1].vScale);
		vDestRotation = XMLoadFloat4(&m_vecKeyframes[m_iCurrentKeyframeIndex + 1].vRotation);
		vDestPosition = XMLoadFloat3(&m_vecKeyframes[m_iCurrentKeyframeIndex + 1].vPosition);

		vScale = XMVectorLerp(vSourScale, vDestScale, Ratio);
		vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, Ratio);
		vPosition = XMVectorLerp(vSourPosition, vDestPosition, Ratio);
		vPosition = XMVectorSetW(vPosition, 1.f);

		if ("104_Teammate" == m_strName)
		{
			vPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);
		}

		// if ("Bip001 Footsteps" == m_strName)
		// {
		// 	vPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);
		// }
		// if ("Bip001 Footsteps.001" == m_strName)
		// {
		// 	vPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);
		// }
	}


	vScale = XMVectorLerp(vBaseScale, vScale, fRatio);
	vRotation = XMQuaternionSlerp(vBaseRot, vRotation, fRatio);
	vRotation = XMQuaternionSlerp(XMQuaternionIdentity(), vRotation, fRatio);
	vRotation = XMQuaternionMultiply(vBaseRot, vRotation);

	vPosition = XMVectorLerp(vBasePos, vPosition, fRatio);
	vPosition = XMVectorSetW(vPosition, 1.f);

	_matrix TransformMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

	m_pBone->Set_TransformMatrix(TransformMatrix);
}


CChannel* CChannel::Create(aiNodeAnim* pAIChannel, CModel* pModel)
{
	CChannel*		pInstance = new CChannel();

	if (FAILED(pInstance->Initialize(pAIChannel, pModel)))
	{
		MSG_BOX("Failed to Created : CChannel");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CChannel::Free()
{
	Safe_Release(m_pBone);

	m_vecKeyframes.clear();
}
