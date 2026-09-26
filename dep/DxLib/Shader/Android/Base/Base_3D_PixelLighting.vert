attribute	highp	vec3	aPos ;			// 座標( ローカル空間 )
attribute	mediump	vec3	aNrm ;			// 法線
attribute	mediump	vec4	aDif ;			// ディフューズカラー
attribute	mediump	vec4	aSpc ;			// スペキュラカラー
attribute	mediump	vec2	aTexUV0 ;		// テクスチャ座標０
attribute	mediump	vec2	aTexUV1 ;		// テクスチャ座標１

#ifdef BUMPMAP
// バンプマップ
attribute	mediump	vec3	aTan ;			// 接線( ローカル空間 )
attribute	mediump	vec3	aBin ;			// 従法線( ローカル空間 )
#endif // BUMPMAP

#ifdef SKINMESH
	// スキニングメッシュ
	attribute			ivec4	aBlendInd0 ;		// ボーン処理用 Float型定数配列インデックス０
	attribute	lowp	vec4	aBlendWeight0 ;		// ボーン処理用ウエイト値０

	#ifdef BONE8

		attribute			ivec4	aBlendInd1 ;		// ボーン処理用 Float型定数配列インデックス１
		attribute	lowp	vec4	aBlendWeight1 ;		// ボーン処理用ウエイト値１

	#endif // BONE8

#endif // SKINMESH

// 頂点シェーダーの出力
varying		mediump	vec4	vDif ;				// ディフューズカラー
varying		mediump	vec4	vSpc ;				// スペキュラカラー
varying		mediump	vec4	vTexUV0_1 ;			// xy:テクスチャ座標 zw:サブテクスチャ座標
varying		highp	vec3	vVPos ;				// 座標( ビュー空間 )
varying		mediump	vec3	vVNrm ;				// 法線( ビュー空間 )
#ifdef BUMPMAP
varying		mediump	vec3	vVTan ;				// 接線( ビュー空間 )
varying		mediump	vec3	vVBin ;				// 従法線( ビュー空間 )
#endif // BUMPMAP
varying		highp	vec2	vFog_SM2PosZ ;		// フォグパラメータ( x )とシャドウマップ３のライト座標( y )

#if SHADOWMAP != 0
varying		highp	vec4	vSM0Pos_SM2PosX ;	// シャドウマップ１のライト座標( x, y, z )とシャドウマップ３のライトX座標( w )
varying		highp	vec4	vSM1Pos_SM2PosY ;	// シャドウマップ２のライト座標( x, y, z )とシャドウマップ３のライトY座標( w )
#endif // SHADOWMAP


uniform		mediump	vec4	uMatDif ;			// ディフューズカラー
uniform		mediump	vec4	uMatSpc ;			// スペキュラカラー

// C++ 側で設定する定数の定義
uniform		highp	vec4	uProjMat[ 4 ] ;		// ビュー　→　プロジェクション行列
uniform		highp	vec4	uViewMat[ 3 ] ;		// ワールド　→　ビュー行列
uniform		highp	vec4	uFog ;				// フォグ用パラメータ( x:end/(end - start)  y:-1/(end - start)  z:density  w:自然対数の低 )

uniform		highp	vec4	uToonOutLineSize ;	// トゥーンの輪郭線の大きさ
uniform		lowp	vec4	uDifSrc_SpeSrc_MulSpeCol ;	// x:ディフューズカラー( 0.0:マテリアル  1.0:頂点 )  y:スペキュラカラー(   0.0:マテリアル  1.0:頂点 )  z:スペキュラカラー値に乗算する値( スペキュラ無効処理で使用 )

uniform		highp	vec4	uSMLVPMat[ 12 /*  [ 3 ][ 4 ]  */ ] ;	// シャドウマップ用のライトビュー行列とライト射影行列を乗算したもの

uniform		mediump	vec4	uTexMat[ 6  /*  [ DX_VS_CONSTF_TEXTURE_MATRIX_NUM ][ 2 ]  */ ] ;	// テクスチャ座標操作用行列
uniform		highp	vec4	uLWMat[ 162 /*  DX_VS_CONSTF_WORLD_MAT_NUM      * 3  */ ] ;	// ローカル　→　ワールド行列





// main関数
void main( void )
{
	ivec4			lBoneIndex ;
	highp	vec4	lLWMat[ 3 ] ;
	highp	vec4	lLocalPos ;
	highp	vec4	lWorldPos ;
	highp	vec4	lViewPos ;
	mediump	vec3	lWorldNrm ;
	mediump	vec3	lWorldTan ;
	mediump	vec3	lWorldBin ;
	mediump	vec3	lViewNrm ;
	mediump	vec3	lViewTan ;
	mediump	vec3	lViewBin ;
	mediump	vec4	lTexUVTemp ;
#if SHADOWMAP != 0
	highp	vec4	lLViewPos ;
#endif


	// 頂点座標変換 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	#ifdef SKINMESH

		// スキンメッシュ

		// ブレンド行列の作成
		lBoneIndex = aBlendInd0 ;
		lLWMat[ 0 ]  = uLWMat[ lBoneIndex.x + 0 ] * aBlendWeight0.xxxx;
		lLWMat[ 1 ]  = uLWMat[ lBoneIndex.x + 1 ] * aBlendWeight0.xxxx;
		lLWMat[ 2 ]  = uLWMat[ lBoneIndex.x + 2 ] * aBlendWeight0.xxxx;

		lLWMat[ 0 ] += uLWMat[ lBoneIndex.y + 0 ] * aBlendWeight0.yyyy;
		lLWMat[ 1 ] += uLWMat[ lBoneIndex.y + 1 ] * aBlendWeight0.yyyy;
		lLWMat[ 2 ] += uLWMat[ lBoneIndex.y + 2 ] * aBlendWeight0.yyyy;

		lLWMat[ 0 ] += uLWMat[ lBoneIndex.z + 0 ] * aBlendWeight0.zzzz;
		lLWMat[ 1 ] += uLWMat[ lBoneIndex.z + 1 ] * aBlendWeight0.zzzz;
		lLWMat[ 2 ] += uLWMat[ lBoneIndex.z + 2 ] * aBlendWeight0.zzzz;

		lLWMat[ 0 ] += uLWMat[ lBoneIndex.w + 0 ] * aBlendWeight0.wwww;
		lLWMat[ 1 ] += uLWMat[ lBoneIndex.w + 1 ] * aBlendWeight0.wwww;
		lLWMat[ 2 ] += uLWMat[ lBoneIndex.w + 2 ] * aBlendWeight0.wwww;

		#ifdef BONE8

			lBoneIndex = aBlendInd1 ;
			lLWMat[ 0 ] += uLWMat[ lBoneIndex.x + 0 ] * aBlendWeight1.xxxx;
			lLWMat[ 1 ] += uLWMat[ lBoneIndex.x + 1 ] * aBlendWeight1.xxxx;
			lLWMat[ 2 ] += uLWMat[ lBoneIndex.x + 2 ] * aBlendWeight1.xxxx;

			lLWMat[ 0 ] += uLWMat[ lBoneIndex.y + 0 ] * aBlendWeight1.yyyy;
			lLWMat[ 1 ] += uLWMat[ lBoneIndex.y + 1 ] * aBlendWeight1.yyyy;
			lLWMat[ 2 ] += uLWMat[ lBoneIndex.y + 2 ] * aBlendWeight1.yyyy;

			lLWMat[ 0 ] += uLWMat[ lBoneIndex.z + 0 ] * aBlendWeight1.zzzz;
			lLWMat[ 1 ] += uLWMat[ lBoneIndex.z + 1 ] * aBlendWeight1.zzzz;
			lLWMat[ 2 ] += uLWMat[ lBoneIndex.z + 2 ] * aBlendWeight1.zzzz;

			lLWMat[ 0 ] += uLWMat[ lBoneIndex.w + 0 ] * aBlendWeight1.wwww;
			lLWMat[ 1 ] += uLWMat[ lBoneIndex.w + 1 ] * aBlendWeight1.wwww;
			lLWMat[ 2 ] += uLWMat[ lBoneIndex.w + 2 ] * aBlendWeight1.wwww;

		#endif // BONE8

	#endif	// SKINMESH

	// ローカル座標のセット
	lLocalPos.xyz = aPos ;
	lLocalPos.w = 1.0 ;

	// 座標計算( ローカル→ビュー→プロジェクション )
	#ifdef SKINMESH
		lWorldPos.x = dot( lLocalPos, lLWMat[ 0 ] ) ;
		lWorldPos.y = dot( lLocalPos, lLWMat[ 1 ] ) ;
		lWorldPos.z = dot( lLocalPos, lLWMat[ 2 ] ) ;
	#else
		lWorldPos.x = dot( lLocalPos, uLWMat[ 0 ] ) ;
		lWorldPos.y = dot( lLocalPos, uLWMat[ 1 ] ) ;
		lWorldPos.z = dot( lLocalPos, uLWMat[ 2 ] ) ;
	#endif
	lWorldPos.w = 1.0 ;

	lViewPos.x = dot( lWorldPos, uViewMat[ 0 ] ) ;
	lViewPos.y = dot( lWorldPos, uViewMat[ 1 ] ) ;
	lViewPos.z = dot( lWorldPos, uViewMat[ 2 ] ) ;
	lViewPos.w = 1.0 ;

	gl_Position.x = dot( lViewPos, uProjMat[ 0 ] ) ;
	gl_Position.y = dot( lViewPos, uProjMat[ 1 ] ) ;
	gl_Position.z = dot( lViewPos, uProjMat[ 2 ] ) ;
	gl_Position.w = dot( lViewPos, uProjMat[ 3 ] ) ;
	
	// 座標( ビュー空間 )を保存
	vVPos = lViewPos.xyz;
	
	// 法線を計算
	#ifdef SKINMESH
		lWorldNrm.x = dot( aNrm, lLWMat[ 0 ].xyz ) ;
		lWorldNrm.y = dot( aNrm, lLWMat[ 1 ].xyz ) ;
		lWorldNrm.z = dot( aNrm, lLWMat[ 2 ].xyz ) ;
	#else
		lWorldNrm.x = dot( aNrm, uLWMat[ 0 ].xyz ) ;
		lWorldNrm.y = dot( aNrm, uLWMat[ 1 ].xyz ) ;
		lWorldNrm.z = dot( aNrm, uLWMat[ 2 ].xyz ) ;
	#endif

	lViewNrm.x = dot( lWorldNrm, uViewMat[ 0 ].xyz ) ;
	lViewNrm.y = dot( lWorldNrm, uViewMat[ 1 ].xyz ) ;
	lViewNrm.z = dot( lWorldNrm, uViewMat[ 2 ].xyz ) ;

	// 法線( ビュー空間 )を保存
	vVNrm = lViewNrm;

	#ifdef BUMPMAP

		// 従法線、接線をビュー空間に投影する
		#ifdef SKINMESH
			lWorldTan.x = dot( aTan, lLWMat[ 0 ].xyz ) ;
			lWorldTan.y = dot( aTan, lLWMat[ 1 ].xyz ) ;
			lWorldTan.z = dot( aTan, lLWMat[ 2 ].xyz ) ;

			lWorldBin.x = dot( aBin, lLWMat[ 0 ].xyz ) ;
			lWorldBin.y = dot( aBin, lLWMat[ 1 ].xyz ) ;
			lWorldBin.z = dot( aBin, lLWMat[ 2 ].xyz ) ;
		#else
			lWorldTan.x = dot( aTan, uLWMat[ 0 ].xyz ) ;
			lWorldTan.y = dot( aTan, uLWMat[ 1 ].xyz ) ;
			lWorldTan.z = dot( aTan, uLWMat[ 2 ].xyz ) ;

			lWorldBin.x = dot( aBin, uLWMat[ 0 ].xyz ) ;
			lWorldBin.y = dot( aBin, uLWMat[ 1 ].xyz ) ;
			lWorldBin.z = dot( aBin, uLWMat[ 2 ].xyz ) ;
		#endif

		lViewTan.x = dot( lWorldTan, uViewMat[ 0 ].xyz ) ;
		lViewTan.y = dot( lWorldTan, uViewMat[ 1 ].xyz ) ;
		lViewTan.z = dot( lWorldTan, uViewMat[ 2 ].xyz ) ;

		lViewBin.x = dot( lWorldBin, uViewMat[ 0 ].xyz ) ;
		lViewBin.y = dot( lWorldBin, uViewMat[ 1 ].xyz ) ;
		lViewBin.z = dot( lWorldBin, uViewMat[ 2 ].xyz ) ;
		
		// 従法線、接線( ビュー空間 )を保存
		vVTan = lViewTan;
		vVBin = lViewBin;

	#endif	// BUMPMAP

	// ディフューズカラーをセット
	vDif  = uDifSrc_SpeSrc_MulSpeCol.x  > 0.5 ? aDif  : uMatDif ;
	
	// スペキュラカラーをセット
	vSpc = ( uDifSrc_SpeSrc_MulSpeCol.y > 0.5 ? aSpc : uMatSpc ) * uDifSrc_SpeSrc_MulSpeCol.z ;

	// 頂点座標変換 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )





	// フォグ計算 =============================================( 開始 )

	#if FOG_LINEAR != 0 || FOG_EXP != 0 || FOG_EXP2 != 0

		#ifdef FOG_LINEAR

			// 線形フォグ計算
			vFog_SM2PosZ.x = lViewPos.z * uFog.y + uFog.x ;

		#endif

		#ifdef FOG_EXP

			// 指数フォグ計算 1.0 / pow( e, 距離 * density )
			vFog_SM2PosZ.x = 1.0 / pow( abs( uFog.w ), lViewPos.z * uFog.z ) ;

		#endif

		#ifdef FOG_EXP2

			// 指数フォグ２計算 1.0 / pow( e, ( 距離 * density ) * ( 距離 * density ) )
			vFog_SM2PosZ.x = 1.0 / pow( abs( uFog.w ), ( lViewPos.z * uFog.z ) * ( lViewPos.z * uFog.z ) ) ;

		#endif

	#else // FOG_LINEAR || FOG_EXP || FOG_EXP2
	
		vFog_SM2PosZ.x = 1.0;

	#endif // FOG_LINEAR || FOG_EXP || FOG_EXP2

	// フォグ計算 =============================================( 終了 )

#if SHADOWMAP != 0
	// 深度影用のライトから見た射影座標を算出 =================( 開始 )

	// ワールド座標をシャドウマップ０のライト設定の射影座標に変換
	vSM0Pos_SM2PosX.x = dot( lWorldPos, uSMLVPMat[  0 ] ) ;
	vSM0Pos_SM2PosX.y = dot( lWorldPos, uSMLVPMat[  1 ] ) ;
	vSM0Pos_SM2PosX.z = dot( lWorldPos, uSMLVPMat[  2 ] ) ;

	// ワールド座標をシャドウマップ１のライト設定の射影座標に変換
	vSM1Pos_SM2PosY.x = dot( lWorldPos, uSMLVPMat[  4 ] ) ;
	vSM1Pos_SM2PosY.y = dot( lWorldPos, uSMLVPMat[  5 ] ) ;
	vSM1Pos_SM2PosY.z = dot( lWorldPos, uSMLVPMat[  6 ] ) ;

	// ワールド座標をシャドウマップ２のライト設定の射影座標に変換
	vSM0Pos_SM2PosX.w = dot( lWorldPos, uSMLVPMat[  8 ] ) ;
	vSM1Pos_SM2PosY.w = dot( lWorldPos, uSMLVPMat[  9 ] ) ;
	vFog_SM2PosZ.y    = dot( lWorldPos, uSMLVPMat[ 10 ] ) ;

	// 深度影用のライトから見た射影座標を算出 =================( 終了 )
#endif

	// テクスチャ座標のセット
	lTexUVTemp.z = 1.0 ;
	lTexUVTemp.w = 1.0 ;

	lTexUVTemp.xy = aTexUV0 ;
	vTexUV0_1.x = dot( lTexUVTemp, uTexMat[ 0 ] ) ;
	vTexUV0_1.y = dot( lTexUVTemp, uTexMat[ 1 ] ) ;

	lTexUVTemp.xy = aTexUV1 ;
	vTexUV0_1.z = dot( lTexUVTemp, uTexMat[ 2 ] ) ;
	vTexUV0_1.w = dot( lTexUVTemp, uTexMat[ 3 ] ) ;
}

