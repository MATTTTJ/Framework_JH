#pragma once
namespace Engine
{
	typedef struct EffectParticleDesc
	{
		_float3 vMinPos; //최소 위치
		_float3 vMaxPos; //최대 위치

		_float fMaxSpeed; //최대 속력
		_float fMinSpeed; //최소 속력
		_float3 vMaxDir; //방향(노말라이즈)
		_float3 vMinDir; //방향(노말라이즈)
		_float3 vAcceleration; //속도 변화량

		_float3 vMinScale; //최소 크기
		_float3 vMaxScale; //최대 크기
		_float3 vScaleVariation; //크기 변화량

		_float4 vMinColor; //최소 칼라
		_float4 vMaxColor; //최대 칼라
		_float4 vColorVariation; //색 변화량

		_float fMinLifeTime; //최소 생존시간
		_float fMaxLifeTime; //최대 생존시간

		_float fVemitRate; //파티클 생성 비율(초당 파티클 수)
		_float fGenerationTime; //파티클 생성 시간(몇초까지 생성할 것인지)
		_uint iTotalCnt;//전체 파티클 갯수

	}PARTICLEDESC;

	typedef struct Particle
	{
		_float3 vPos;
		_float4	vColor;
		_float3	vSize;
		_float3 vVelocity;
		_float fLifeTime;
		_float fAge; //현재 파티클 생존시간
		_uint iNum; //인스탄스 번호
	}PARTICLE;

	typedef struct tagGraphicDesc
	{
		enum WINMODE { WINMODE_FULL, WINMODE_WIN, WINMODE_END };

		WINMODE			eWindowMode;
		unsigned int	iViewportSizeX;
		unsigned int	iViewportSizeY;
		HINSTANCE		hInst;
		HWND			hWnd;
	}GRAPHIC_DESC;


	typedef struct tagFaceIndices16
	{
		unsigned short			_0, _1, _2;
	}FACEINDICES16;

	typedef struct tagFaceIndices32
	{
		unsigned long			_0, _1, _2;
	}FACEINDICES32;

	/* 방향성광원, 점광원 : 방향벡터가 필요하다. */
	typedef struct tagLightDesc
	{
		enum TYPE { LIGHT_DIRECTIONAL, LIGHT_POINT, LIGHT_FOV, LIGHTTYPE_END };

		TYPE				eType = LIGHTTYPE_END;

		_bool				isEnable;

		XMFLOAT4			vDirection;

		XMFLOAT4			vPosition;
		_float				fRange;
		XMFLOAT4			vDiffuse;
		XMFLOAT4			vAmbient;
		XMFLOAT4			vSpecular;
	}LIGHTDESC;

	typedef struct tagKeyframe
	{
		double		dTime;
		XMFLOAT3	vScale;
		XMFLOAT4	vRotation;
		XMFLOAT3	vPosition;
	}KEYFRAME;

	typedef struct tagModelMaterial
	{
		class CTexture*		pTexture[AI_TEXTURE_TYPE_MAX];
	}MODELMATERIAL;

	typedef struct tagVertexPositionTexture
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexUV;
	}VTXTEX;

	typedef struct tagVertexPosition
	{
		XMFLOAT3		vPosition;
	}VTXPOS;

	typedef struct tagVertexPoint
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vPSize;
	}VTXPOINT;

	typedef struct tagVertex_Instance
	{
		XMFLOAT4			vRight;
		XMFLOAT4			vUp;
		XMFLOAT4			vLook;
		XMFLOAT4			vTranslation;
		XMFLOAT4			vColor;
	}VTXINSTANCE;

	typedef struct tagVertexMatrix
	{
		XMFLOAT4			vRight;
		XMFLOAT4			vUp;
		XMFLOAT4			vLook;
		XMFLOAT4			vPosition;
	}VTXMATRIX;

	typedef struct tagVertexPositionNormalTexture
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexUV;
	}VTXNORTEX;

	typedef struct tagVertexPositionCubeTexture
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vTexUV;
	}VTXCUBETEX;

	typedef struct tagVertexModel
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexUV;
		XMFLOAT3		vTangent;
	}VTXMODEL;

	typedef struct tagVertexAnimModel
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexUV;
		XMFLOAT3		vTangent;
		XMUINT4			vBlendIndex; /* 이 정점이 사용해야하는 뼈의 인덱스(전체뼈 중의 순서(x), 이 정점이 소속된 메시에 영향을 주는 뼈중의 순서(o))  */
		XMFLOAT4		vBlendWeight;
	}VTXANIMMODEL;

	
	typedef struct ENGINE_DLL tagVertexPosition_Declaration
	{
		static const unsigned int		iNumElements = 1;
		static D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXPOS_DECLARATION;

	typedef struct ENGINE_DLL tagVertexPositionTexture_Declaration
	{
		static const unsigned int			iNumElements = 2;
		static D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXTEX_DECLARATION;

	typedef struct ENGINE_DLL tagVertexPositionCubeTexture_Declaration
	{
		static const unsigned int			iNumElements = 2;
		static D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXCUBETEX_DECLARATION;

	typedef struct ENGINE_DLL tagVertexPositionNormalTexture_Declaration
	{
		static const unsigned int		iNumElements = 3;
		static D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXNORTEX_DECLARATION;

	typedef struct ENGINE_DLL tagVertexModel_Declaration
	{
		static const unsigned int			iNumElements = 4;
		static D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXMODEL_DECLARATION;

	typedef struct ENGINE_DLL tagVertexAnimModel_Declaration
	{
		static const unsigned int			iNumElements = 6;
		static D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXANIMMODEL_DECLARATION;

	typedef struct ENGINE_DLL tagVertexPointInstance_Declaration
	{
		static const unsigned int		iNumElements = 6;
		static D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];
	}VTXPOINTINSTANCE_DECLARATION;


	typedef struct ENGINE_DLL tagVertexRectInstance_Declaration
	{
		static const unsigned int		iNumElements = 6;
		static D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXRECTINSTANCE_DECLARATION;





	

	

}