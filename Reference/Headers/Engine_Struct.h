#pragma once
namespace Engine
{
	typedef struct tagGraphicDesc
	{
		enum WINMODE { WINMODE_FULL, WINMODE_WIN, WINMODE_END };

		WINMODE			eWindowMode;
		unsigned int	iViewportSizeX;
		unsigned int	iViewportSizeY;
		HWND			hWnd;
	}GRAPHIC_DESC;


	/* 방향성광원, 점광원 : 방향벡터가 필요함*/
	typedef struct tagLightDesc
	{
		enum TYPE { TYPE_DIRECTIONAL, TYPE_POINT, TYPE_END };

		TYPE		eType;

		bool		isEnable; // 사용중인 조명인지?

		XMFLOAT4	vDirection;

		/* 모든 정점(픽셀)이 받는 빛의 방향벡터가 다 다르게 표현되어야함*/
		/* 셰이더내에서 빛의 방향벡터를 연산한다.*/
		XMFLOAT4			vPosition;
		float				fRange;			// 점광원의 범위
		XMFLOAT4			vDiffuse;		// 난반사 
		XMFLOAT4			vAmbient;		// 환경광(완전 어둡지 않게 최소한으로 받는 빛) 
		XMFLOAT4			vSpecular;		// 정반사(기존의 색을 날리고 완전히 하얀 빛)
	}LIGHTDESC;

	typedef struct tagFaceIndices16
	{
		unsigned short			_0, _1, _2;
	}FACEINDICES16;

	typedef struct tagFaceIndices32
	{
		unsigned long			_0, _1, _2;
	}FACEINDICES32;

	typedef struct tagVertexPositionTexture
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexUV;
	}VTXTEX;

	typedef struct ENGINE_DLL tagVertexPositionTexture_Declaration
	{
		static const unsigned int		iNumElements = 2;
		static D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXTEX_DECLARATION;

	typedef struct tagVertexPositionNormalTexture
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexUV;
	}VTXNORTEX;

	typedef struct ENGINE_DLL tagVertexPositionNormalTexture_Declaration
	{
		static const unsigned int		iNumElements = 3;
		static D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXNORTEX_DECLARATION;

}