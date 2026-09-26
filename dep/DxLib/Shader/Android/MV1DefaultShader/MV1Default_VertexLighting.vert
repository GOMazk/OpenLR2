// 頂点シェーダーの入力
attribute	highp	vec3	aPos ;			// 座標( ローカル空間 )
attribute	mediump	vec3	aNrm ;			// 法線
attribute	mediump	vec4	aDif ;			// ディフューズカラー
attribute	mediump	vec4	aSpc ;			// スペキュラカラー
attribute	mediump	vec4	aTexUV0F4 ;		// テクスチャ座標０
attribute	mediump	vec4	aTexUV1F4 ;		// テクスチャ座標１

#if BUMPMAP != 0 || BUMPMAP_VERTEXDATAONLY != 0
// バンプマップ
attribute	mediump	vec3	aTan ;			// 接線( ローカル空間 )
attribute	mediump	vec3	aBin ;			// 従法線( ローカル空間 )
#endif // BUMPMAP

#ifdef SKINMESH
	// スキニングメッシュ
	attribute	mediump	vec4	aBlendInd0 ;		// ボーン処理用 Float型定数配列インデックス０
	attribute	lowp	vec4	aBlendWeight0 ;		// ボーン処理用ウエイト値０

	#ifdef BONE8

		attribute	mediump	vec4	aBlendInd1 ;		// ボーン処理用 Float型定数配列インデックス１
		attribute	lowp	vec4	aBlendWeight1 ;		// ボーン処理用ウエイト値１

	#endif // BONE8

#endif // SKINMESH

// 頂点シェーダーの出力
varying		mediump	vec4	vDif ;				// ディフューズカラー
varying		mediump	vec4	vSpc ;				// スペキュラカラー
varying		mediump	vec4	vTexUV0_1 ;			// xy:テクスチャ座標 zw:サブテクスチャ座標

varying		highp	vec4	vVtoEyeSM2PosZ ;	// 頂点座標から視線へのベクトル( ビュー空間 )とシャドウマップ２のライトZ座標( w )
varying		highp	vec4	vNrmFog ;			// x.y.z:法線( ビュー空間 )  w:フォグパラメータ

#if LG0_USE != 0
varying		highp	vec4	vLight0DirGen ;		// xyz:ライト０の逆ベクトル( ビュー空間 ) w:ライト０減衰情報
#endif
#if LG1_USE != 0
varying		highp	vec4	vLight1DirGen ;		// xyz:ライト１の逆ベクトル( ビュー空間 ) w:ライト１減衰情報
#endif
#if LG2_USE != 0
varying		highp	vec4	vLight2DirGen ;		// xyz:ライト２の逆ベクトル( ビュー空間 ) w:ライト２減衰情報
#endif

#if SHADOWMAP != 0 || SHADOWMAP_DRAW != 0
varying		highp	vec4	vSM0Pos_SM2PosX ;	// シャドウマップ０のライト座標( x, y, z )とシャドウマップ２のライトX座標( w )
#endif
#if SHADOWMAP != 0
varying		highp	vec4	vSM1Pos_SM2PosY ;	// シャドウマップ１のライト座標( x, y, z )とシャドウマップ２のライトY座標( w )
#endif



uniform		mediump	vec4	uAmb_Emi ;			// エミッシブカラー＋アンビエントカラー( エミッシブカラー + マテリアルアンビエントカラー * グローバルアンビエントカラー )

uniform		mediump	vec4	uMatDif ;			// ディフューズカラー
uniform		mediump	vec4	uMatSpc ;			// スペキュラカラー
uniform		mediump	float	uMatPow ;			// スペキュラの強さ

uniform		highp	vec3	uLightPos[ 3 ] ;					// 座標( ビュー空間 )
uniform		mediump	vec3	uLightDir[ 3 ] ;					// 方向( ビュー空間 )
uniform		mediump	vec4	uLightDif[ 3 ] ;					// ディフューズカラー
uniform		mediump	vec4	uLightSpc[ 3 ] ;					// スペキュラカラー
uniform		mediump	vec4	uLightAmb[ 3 ] ;					// アンビエントカラー
uniform		highp	vec4	uLightRange_FallOff_AT0_AT1[ 3 ] ;	// x:有効距離  y:スポットライト用FllOff  z:距離による減衰処理用パラメータ０  w:距離による減衰処理用パラメータ１
uniform		highp	vec4	uLightAT2_SpotP0_SpotP1[ 3 ] ;		// x:距離による減衰処理用パラメータ２  y:スポットライト用パラメータ０( cos( Phi / 2.0 ) )  z:スポットライト用パラメータ１( 1.0 / ( cos( Theta / 2.0 ) - cos( Phi / 2.0 ) ) )

uniform		highp	vec4	uProjMat[ 4 ] ;		// ビュー　→　プロジェクション行列
uniform		highp	vec4	uViewMat[ 3 ] ;		// ワールド　→　ビュー行列
uniform		highp	vec4	uFog ;				// フォグ用パラメータ( x:end/(end - start)  y:-1/(end - start)  z:density  w:自然対数の低 )

uniform		highp	vec4	uToonOutLineSize ;	// トゥーンの輪郭線の大きさ
uniform		lowp	vec4	uDifSrc_SpeSrc_MulSpeCol ;	// x:ディフューズカラー( 0.0:マテリアル  1.0:頂点 )  y:スペキュラカラー(   0.0:マテリアル  1.0:頂点 )  z:スペキュラカラー値に乗算する値( スペキュラ無効処理で使用 )

uniform		highp	vec4	uSMLVPMat[ 12 /* [ 3 ][ 4 ] */ ] ;	// シャドウマップ用のライトビュー行列とライト射影行列を乗算したもの

uniform		mediump	vec4	uTexMat[ 6 /* [ DX_VS_CONSTF_TEXTURE_MATRIX_NUM ][ 2 ] */ ] ;	// テクスチャ座標操作用行列
uniform		highp	vec4	uLWMat[ 162 /* DX_VS_CONSTF_WORLD_MAT_NUM * 3 */ ] ;		// ローカル　→　ワールド行列


// main関数
void main( void )
{
			ivec4	lBoneIndex ;
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
	highp	vec3	lBumpTemp ;
	mediump	vec4	lTotalDif ;
	mediump	vec4	lTotalSpc ;
	mediump	vec3	lLightDir ;
	mediump	float	lTotalGen ;
	highp	vec3	lLightTemp ;
	highp	float	lLightDistPow2 ;
	mediump	float	lLightGen ;
	highp	float	lLightDirCosA ;
	highp	vec3	lLightHalfVec ;
	highp	vec4	lLightLitDest ;
	mediump	vec4	lDifColor;
#if USE_SPE != 0
	mediump	vec4	lSpeColor;
#endif
#if SHADOWMAP != 0
	highp	vec4	lLViewPos ;
#endif

	lDifColor = uDifSrc_SpeSrc_MulSpeCol.x  > 0.5 ? aDif  : uMatDif ;
	
#if USE_SPE != 0
	lSpeColor = ( uDifSrc_SpeSrc_MulSpeCol.y > 0.5 ? aSpc : uMatSpc ) * uDifSrc_SpeSrc_MulSpeCol.z ;
#endif

	vVtoEyeSM2PosZ.xyz  = vec3( 0.0, 0.0, 0.0 ) ;
	vSpc				= vec4( 0.0, 0.0, 0.0, 0.0 ) ;
	vNrmFog				= vec4( 0.0, 0.0, 0.0, 1.0 ) ;

	// 頂点座標変換 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	#ifdef SKINMESH

		// スキンメッシュ

		// ブレンド行列の作成
		lBoneIndex.x = int( aBlendInd0.x ) ;
		lBoneIndex.y = int( aBlendInd0.y ) ;
		lBoneIndex.z = int( aBlendInd0.z ) ;
		lBoneIndex.w = int( aBlendInd0.w ) ;
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

			lBoneIndex.x = int( aBlendInd1.x ) ;
			lBoneIndex.y = int( aBlendInd1.y ) ;
			lBoneIndex.z = int( aBlendInd1.z ) ;
			lBoneIndex.w = int( aBlendInd1.w ) ;
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

	#ifdef TOONOUTLINE

		// 座標計算( ローカル→ワールド )
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

		#ifdef SKINMESH
			lWorldNrm.x = dot( aNrm, lLWMat[ 0 ].xyz ) ;
			lWorldNrm.y = dot( aNrm, lLWMat[ 1 ].xyz ) ;
			lWorldNrm.z = dot( aNrm, lLWMat[ 2 ].xyz ) ;
		#else
			lWorldNrm.x = dot( aNrm, uLWMat[ 0 ].xyz ) ;
			lWorldNrm.y = dot( aNrm, uLWMat[ 1 ].xyz ) ;
			lWorldNrm.z = dot( aNrm, uLWMat[ 2 ].xyz ) ;
		#endif

		// 輪郭線の太さ分法線方向に移動する
		lWorldPos.xyz += lWorldNrm * uToonOutLineSize.x ;

		// ワールド→ビュー
		lViewPos.x = dot( lWorldPos, uViewMat[ 0 ] ) ;
		lViewPos.y = dot( lWorldPos, uViewMat[ 1 ] ) ;
		lViewPos.z = dot( lWorldPos, uViewMat[ 2 ] ) ;
		lViewPos.w = 1.0 ;

		// ビュー→プロジェクション
		gl_Position.x = dot( lViewPos, uProjMat[ 0 ] ) ;
		gl_Position.y = dot( lViewPos, uProjMat[ 1 ] ) ;
		gl_Position.z = dot( lViewPos, uProjMat[ 2 ] ) ;
		gl_Position.w = dot( lViewPos, uProjMat[ 3 ] ) ;

	#else  // TOONOUTLINE

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

	#endif // TOONOUTLINE

	#ifdef BUMPMAP

		// 従法線、接線、法線をビュー空間に投影する
		#ifdef SKINMESH
			lWorldTan.x = dot( aTan, lLWMat[ 0 ].xyz ) ;
			lWorldTan.y = dot( aTan, lLWMat[ 1 ].xyz ) ;
			lWorldTan.z = dot( aTan, lLWMat[ 2 ].xyz ) ;

			lWorldBin.x = dot( aBin, lLWMat[ 0 ].xyz ) ;
			lWorldBin.y = dot( aBin, lLWMat[ 1 ].xyz ) ;
			lWorldBin.z = dot( aBin, lLWMat[ 2 ].xyz ) ;

			lWorldNrm.x = dot( aNrm, lLWMat[ 0 ].xyz ) ;
			lWorldNrm.y = dot( aNrm, lLWMat[ 1 ].xyz ) ;
			lWorldNrm.z = dot( aNrm, lLWMat[ 2 ].xyz ) ;
		#else
			lWorldTan.x = dot( aTan, uLWMat[ 0 ].xyz ) ;
			lWorldTan.y = dot( aTan, uLWMat[ 1 ].xyz ) ;
			lWorldTan.z = dot( aTan, uLWMat[ 2 ].xyz ) ;

			lWorldBin.x = dot( aBin, uLWMat[ 0 ].xyz ) ;
			lWorldBin.y = dot( aBin, uLWMat[ 1 ].xyz ) ;
			lWorldBin.z = dot( aBin, uLWMat[ 2 ].xyz ) ;

			lWorldNrm.x = dot( aNrm, uLWMat[ 0 ].xyz ) ;
			lWorldNrm.y = dot( aNrm, uLWMat[ 1 ].xyz ) ;
			lWorldNrm.z = dot( aNrm, uLWMat[ 2 ].xyz ) ;
		#endif

		lViewTan.x = dot( lWorldTan, uViewMat[ 0 ].xyz ) ;
		lViewTan.y = dot( lWorldTan, uViewMat[ 1 ].xyz ) ;
		lViewTan.z = dot( lWorldTan, uViewMat[ 2 ].xyz ) ;

		lViewBin.x = dot( lWorldBin, uViewMat[ 0 ].xyz ) ;
		lViewBin.y = dot( lWorldBin, uViewMat[ 1 ].xyz ) ;
		lViewBin.z = dot( lWorldBin, uViewMat[ 2 ].xyz ) ;

		lViewNrm.x = dot( lWorldNrm, uViewMat[ 0 ].xyz ) ;
		lViewNrm.y = dot( lWorldNrm, uViewMat[ 1 ].xyz ) ;
		lViewNrm.z = dot( lWorldNrm, uViewMat[ 2 ].xyz ) ;

	#endif	// BUMPMAP


	// 頂点座標変換 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )

	#if LG_USE == 0 || BUMPMAP == 1 || PHONG == 1

		// ライトを使用しないか、バンプマップ又はフォンシェーディングの場合はマテリアル( or 頂点 )カラーをそのまま出力
		vDif  = lDifColor ;

		#if USE_SPE != 0
			vSpc = lSpeColor ;
		#endif
		
		#if BUMPMAP == 0 && PHONG != 0
		
			// 法線をビュー空間の角度に変換
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

			// 法線を保存
			vNrmFog.xyz = lViewNrm ;

		#endif // PHONG

	#endif // LG_USE == 0 || BUMPMAP == 1 || PHONG == 1





	// ライトの処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )
	#if LG_USE != 0

		#ifdef BUMPMAP

			#if USE_SPE != 0
				// 頂点座標から視点へのベクトルを接底空間に投影した後正規化して保存
				lBumpTemp.x = dot( lViewTan, -lViewPos.xyz ) ;
				lBumpTemp.y = dot( lViewBin, -lViewPos.xyz ) ;
				lBumpTemp.z = dot( lViewNrm, -lViewPos.xyz ) ;
				vVtoEyeSM2PosZ.xyz = normalize( lBumpTemp ) ;
			#endif

		#else // BUMPMAP 

			// 法線をビュー空間の角度に変換
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

			// ディフューズカラーとスペキュラカラーの蓄積値の初期化
			lTotalDif = vec4( 0, 0, 0, 0 ) ;
			lTotalSpc = vec4( 0, 0, 0, 0 ) ;

			#ifdef PHONG

				#if USE_SPE != 0
					// 頂点座標から視点へのベクトルを正規化して保存
					vVtoEyeSM2PosZ.xyz = normalize( -lViewPos.xyz ) ;
				#endif

				// 法線を保存
				vNrmFog.xyz = lViewNrm ;

			#endif // PHONG

		#endif // BUMPMAP







		// ライト0の処理 ***********************************************************************************************************************************( 開始 )
#if LG0_USE != 0

	// ディフューズカラー角度減衰計算用のライトの方向を事前計算 ======( 開始 )

	#if LG0_DIR != 0

		// ライトの方向計算
		lLightDir = uLightDir[ 0 ] ;

	#else  // LG0_DIR

		// ライト方向ベクトルの計算
		lLightDir = normalize( lViewPos.xyz - uLightPos[ 0 ] ) ;

	#endif  // LG0_DIR

	// ライトの情報を初期化
	vLight0DirGen = vec4( 0.0, 0.0, 0.0, 0.0 ) ;

	// ライトの向きの逆ベクトルを接地空間に変換
	#ifdef BUMPMAP

		lLightTemp.x = dot( lViewTan, -lLightDir ) ;
		lLightTemp.y = dot( lViewBin, -lLightDir ) ;
		lLightTemp.z = dot( lViewNrm, -lLightDir ) ;
		vLight0DirGen.xyz = normalize( lLightTemp.xyz ) ;

	#else // BUMPMAP
		#if PHONG != 0

			// ライトの向きの逆ベクトルを保存
			vLight0DirGen.xyz = -lLightDir ;

		#endif // PHONG
	#endif // BUMPMAP

	// ディフューズカラー角度減衰計算用のライトの方向を事前計算 ======( 終了 )


	// 距離・スポットライト減衰値計算 ==========================( 開始 )
	#if LG0_SPOT != 0 || LG0_POINT != 0

		// 距離減衰計算 ------------------

		// 頂点とライト位置との距離の二乗を求める lLightTemp.x = ( POS - L0_POS ) * ( POS - L0_POS )
		lLightTemp = lViewPos.xyz - uLightPos[ 0 ] ;
		lLightDistPow2 = dot( lLightTemp, lLightTemp ) ;

		// 減衰率の計算 lLightGen = 1 / ( 減衰値0 + 減衰値1 * 距離 + 減衰値2 * ( 距離 * 距離 ) )
		lLightGen = 1.0 / ( uLightRange_FallOff_AT0_AT1[ 0 ].z + uLightRange_FallOff_AT0_AT1[ 0 ].w * sqrt( lLightDistPow2 ) + uLightAT2_SpotP0_SpotP1[ 0 ].x * lLightDistPow2 ) ;

		// スポットライト減衰計算 --------

		#if LG0_SPOT != 0

			// ライト方向ベクトルとライト位置から頂点位置へのベクトルの内積( 即ち Cos a )を計算 
			lLightDirCosA = dot( lLightDir, uLightDir[ 0 ].xyz ) ;

			// スポットライト減衰計算  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
			lLightGen *= clamp( pow( abs( max( lLightDirCosA - uLightAT2_SpotP0_SpotP1[ 0 ].y, 0.0 ) * uLightAT2_SpotP0_SpotP1[ 0 ].z ), uLightRange_FallOff_AT0_AT1[ 0 ].y ), 0.0, 1.0 ) ;

		#endif // LG0_SPOT

		// 有効距離外だったら減衰率を最大にする処理
		lLightGen *= step( lLightDistPow2, uLightRange_FallOff_AT0_AT1[ 0 ].x ) ;

		// バンプマップ or フォンシェーディング用に減衰率を保存する
		#if PHONG != 0 || BUMPMAP != 0
			vLight0DirGen.w = lLightGen ;
		#endif // PHONG || BUMPMAP

	#endif // LG0_SPOT || LG0_POINT
	// 距離・スポットライト減衰値計算 =========================( 終了 )




	// ディフューズライトとスペキュラライトの角度減衰計算 =======( 開始 )
	#if BUMPMAP == 0 && PHONG == 0

		lLightLitDest.y = clamp( dot( lViewNrm, -lLightDir ), 0.0, 1.0 ) ;

		#ifdef USE_SPE

			// ハーフベクトルの計算 norm( ( norm( 頂点位置から視点へのベクトル ) + ライトの方向 ) )
			lLightHalfVec = normalize( normalize( -lViewPos.xyz ) - lLightDir ) ;

			// lLightLitDest.z = pow( max( 0.0, N * H ), uMatPow )
			lLightLitDest.z = pow( max( 0.0, dot( lViewNrm, lLightHalfVec ) ), uMatPow ) ;

		#endif // USE_SPE

	#endif // BUMPMAP == 0 && PHONG == 0
	// ディフューズライトとスペキュラライトの角度減衰計算 =======( 終了 )





	// ディフューズカラー計算 =======================================( 開始 )

	#if BUMPMAP == 0 && PHONG == 0

		#if LG0_SPOT != 0 || LG0_POINT != 0

			// ディフューズライト蓄積値 += 距離・スポットライト角度減衰値 * ( ディフューズ角度減衰計算結果 * ディフューズカラー * ライトのディフューズカラー + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの )
			lTotalDif.xyz += lLightGen * ( lLightLitDest.y * uLightDif[ 0 ] * lDifColor.xyz + uLightAmb[ 0 ].xyz ) ;

		#else  // LG0_SPOT || LG0_POINT

			// ディフューズライト蓄積値 += ディフューズ角度減衰計算結果 * ディフューズカラー * ライトのディフューズカラー + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの
			lTotalDif.xyz += lLightLitDest.y * uLightDif[ 0 ] * lDifColor.xyz + uLightAmb[ 0 ].xyz ;

		#endif // LG0_SPOT || LG0_POINT

	#endif // BUMPMAP == 0 && PHONG == 0

	// ディフューズカラー計算 =======================================( 終了 )





	// スペキュラカラー計算 =======================================( 開始 )

	#ifdef USE_SPE

		#if BUMPMAP == 0 && PHONG == 0

			#if LG0_SPOT != 0 || LG0_POINT != 0

				// スペキュラライト蓄積値 += スペキュラ角度減衰計算結果 * 距離・スポットライト減衰 * ライトのスペキュラカラー
				lTotalSpc.xyz += lLightGen * lLightLitDest.z * uLightSpc[ 0 ] ;

			#else	// LG0_SPOT || LG0_POINT

				// スペキュラライト蓄積値 += スペキュラ角度減衰計算結果 * ライトのスペキュラカラー
				lTotalSpc.xyz += lLightLitDest.z * uLightSpc[ 0 ] ;

			#endif	// LG0_SPOT || LG0_POINT

		#endif // BUMPMAP == 0 && PHONG == 0

	#endif // USE_SPE

	// スペキュラカラー計算 =======================================( 終了 )




#endif  // LG0_USE
// ライト0の処理 ***********************************************************************************************************************************( 終了 )








		// ライト1の処理 ***********************************************************************************************************************************( 開始 )
#if LG1_USE != 0

	// ディフューズカラー角度減衰計算用のライトの方向を事前計算 ======( 開始 )

	#if LG1_DIR != 0

		// ライトの方向計算
		lLightDir = uLightDir[ 1 ] ;

	#else  // LG1_DIR

		// ライト方向ベクトルの計算
		lLightDir = normalize( lViewPos.xyz - uLightPos[ 1 ] ) ;

	#endif  // LG1_DIR

	// ライトの情報を初期化
	vLight1DirGen = vec4( 0.0, 0.0, 0.0, 0.0 ) ;

	// ライトの向きの逆ベクトルを接地空間に変換
	#ifdef BUMPMAP

		lLightTemp.x = dot( lViewTan, -lLightDir ) ;
		lLightTemp.y = dot( lViewBin, -lLightDir ) ;
		lLightTemp.z = dot( lViewNrm, -lLightDir ) ;
		vLight1DirGen.xyz = normalize( lLightTemp.xyz ) ;

	#else // BUMPMAP
		#if PHONG != 0

			// ライトの向きの逆ベクトルを保存
			vLight1DirGen.xyz = -lLightDir ;

		#endif // PHONG
	#endif // BUMPMAP

	// ディフューズカラー角度減衰計算用のライトの方向を事前計算 ======( 終了 )


	// 距離・スポットライト減衰値計算 ==========================( 開始 )
	#if LG1_SPOT != 0 || LG1_POINT != 0

		// 距離減衰計算 ------------------

		// 頂点とライト位置との距離の二乗を求める lLightTemp.x = ( POS - L1_POS ) * ( POS - L1_POS )
		lLightTemp = lViewPos.xyz - uLightPos[ 1 ] ;
		lLightDistPow2 = dot( lLightTemp, lLightTemp ) ;

		// 減衰率の計算 lLightGen = 1 / ( 減衰値0 + 減衰値1 * 距離 + 減衰値2 * ( 距離 * 距離 ) )
		lLightGen = 1.0 / ( uLightRange_FallOff_AT0_AT1[ 1 ].z + uLightRange_FallOff_AT0_AT1[ 1 ].w * sqrt( lLightDistPow2 ) + uLightAT2_SpotP0_SpotP1[ 1 ].x * lLightDistPow2 ) ;

		// スポットライト減衰計算 --------

		#if LG1_SPOT != 0

			// ライト方向ベクトルとライト位置から頂点位置へのベクトルの内積( 即ち Cos a )を計算 
			lLightDirCosA = dot( lLightDir, uLightDir[ 1 ].xyz ) ;

			// スポットライト減衰計算  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
			lLightGen *= clamp( pow( abs( max( lLightDirCosA - uLightAT2_SpotP0_SpotP1[ 1 ].y, 0.0 ) * uLightAT2_SpotP0_SpotP1[ 1 ].z ), uLightRange_FallOff_AT0_AT1[ 1 ].y ), 0.0, 1.0 ) ;

		#endif // LG1_SPOT

		// 有効距離外だったら減衰率を最大にする処理
		lLightGen *= step( lLightDistPow2, uLightRange_FallOff_AT0_AT1[ 1 ].x ) ;

		// バンプマップ or フォンシェーディング用に減衰率を保存する
		#if PHONG != 0 || BUMPMAP != 0
			vLight1DirGen.w = lLightGen ;
		#endif // PHONG || BUMPMAP

	#endif // LG1_SPOT || LG1_POINT
	// 距離・スポットライト減衰値計算 =========================( 終了 )




	// ディフューズライトとスペキュラライトの角度減衰計算 =======( 開始 )
	#if BUMPMAP == 0 && PHONG == 0

		lLightLitDest.y = clamp( dot( lViewNrm, -lLightDir ), 0.0, 1.0 ) ;

		#ifdef USE_SPE

			// ハーフベクトルの計算 norm( ( norm( 頂点位置から視点へのベクトル ) + ライトの方向 ) )
			lLightHalfVec = normalize( normalize( -lViewPos.xyz ) - lLightDir ) ;

			// lLightLitDest.z = pow( max( 0.0, N * H ), uMatPow )
			lLightLitDest.z = pow( max( 0.0, dot( lViewNrm, lLightHalfVec ) ), uMatPow ) ;

		#endif // USE_SPE

	#endif // BUMPMAP == 0 && PHONG == 0
	// ディフューズライトとスペキュラライトの角度減衰計算 =======( 終了 )





	// ディフューズカラー計算 =======================================( 開始 )

	#if BUMPMAP == 0 && PHONG == 0

		#if LG1_SPOT != 0 || LG1_POINT != 0

			// ディフューズライト蓄積値 += 距離・スポットライト角度減衰値 * ( ディフューズ角度減衰計算結果 * ディフューズカラー * ライトのディフューズカラー + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの )
			lTotalDif.xyz += lLightGen * ( lLightLitDest.y * uLightDif[ 1 ] * lDifColor.xyz + uLightAmb[ 1 ].xyz ) ;

		#else  // LG1_SPOT || LG1_POINT

			// ディフューズライト蓄積値 += ディフューズ角度減衰計算結果 * ディフューズカラー * ライトのディフューズカラー + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの
			lTotalDif.xyz += lLightLitDest.y * uLightDif[ 1 ] * lDifColor.xyz + uLightAmb[ 1 ].xyz ;

		#endif // LG1_SPOT || LG1_POINT

	#endif // BUMPMAP == 0 && PHONG == 0

	// ディフューズカラー計算 =======================================( 終了 )





	// スペキュラカラー計算 =======================================( 開始 )

	#ifdef USE_SPE

		#if BUMPMAP == 0 && PHONG == 0

			#if LG1_SPOT != 0 || LG1_POINT != 0

				// スペキュラライト蓄積値 += スペキュラ角度減衰計算結果 * 距離・スポットライト減衰 * ライトのスペキュラカラー
				lTotalSpc.xyz += lLightGen * lLightLitDest.z * uLightSpc[ 1 ] ;

			#else	// LG1_SPOT || LG1_POINT

				// スペキュラライト蓄積値 += スペキュラ角度減衰計算結果 * ライトのスペキュラカラー
				lTotalSpc.xyz += lLightLitDest.z * uLightSpc[ 1 ] ;

			#endif	// LG1_SPOT || LG1_POINT

		#endif // BUMPMAP == 0 && PHONG == 0

	#endif // USE_SPE

	// スペキュラカラー計算 =======================================( 終了 )




#endif  // LG1_USE
// ライト1の処理 ***********************************************************************************************************************************( 終了 )








		// ライト2の処理 ***********************************************************************************************************************************( 開始 )
#if LG2_USE != 0

	// ディフューズカラー角度減衰計算用のライトの方向を事前計算 ======( 開始 )

	#if LG2_DIR != 0

		// ライトの方向計算
		lLightDir = uLightDir[ 2 ] ;

	#else  // LG2_DIR

		// ライト方向ベクトルの計算
		lLightDir = normalize( lViewPos.xyz - uLightPos[ 2 ] ) ;

	#endif  // LG2_DIR

	// ライトの情報を初期化
	vLight2DirGen = vec4( 0.0, 0.0, 0.0, 0.0 ) ;

	// ライトの向きの逆ベクトルを接地空間に変換
	#ifdef BUMPMAP

		lLightTemp.x = dot( lViewTan, -lLightDir ) ;
		lLightTemp.y = dot( lViewBin, -lLightDir ) ;
		lLightTemp.z = dot( lViewNrm, -lLightDir ) ;
		vLight2DirGen.xyz = normalize( lLightTemp.xyz ) ;

	#else // BUMPMAP
		#if PHONG != 0

			// ライトの向きの逆ベクトルを保存
			vLight2DirGen.xyz = -lLightDir ;

		#endif // PHONG
	#endif // BUMPMAP

	// ディフューズカラー角度減衰計算用のライトの方向を事前計算 ======( 終了 )


	// 距離・スポットライト減衰値計算 ==========================( 開始 )
	#if LG2_SPOT != 0 || LG2_POINT != 0

		// 距離減衰計算 ------------------

		// 頂点とライト位置との距離の二乗を求める lLightTemp.x = ( POS - L2_POS ) * ( POS - L2_POS )
		lLightTemp = lViewPos.xyz - uLightPos[ 2 ] ;
		lLightDistPow2 = dot( lLightTemp, lLightTemp ) ;

		// 減衰率の計算 lLightGen = 1 / ( 減衰値0 + 減衰値1 * 距離 + 減衰値2 * ( 距離 * 距離 ) )
		lLightGen = 1.0 / ( uLightRange_FallOff_AT0_AT1[ 2 ].z + uLightRange_FallOff_AT0_AT1[ 2 ].w * sqrt( lLightDistPow2 ) + uLightAT2_SpotP0_SpotP1[ 2 ].x * lLightDistPow2 ) ;

		// スポットライト減衰計算 --------

		#if LG2_SPOT != 0

			// ライト方向ベクトルとライト位置から頂点位置へのベクトルの内積( 即ち Cos a )を計算 
			lLightDirCosA = dot( lLightDir, uLightDir[ 2 ].xyz ) ;

			// スポットライト減衰計算  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
			lLightGen *= clamp( pow( abs( max( lLightDirCosA - uLightAT2_SpotP0_SpotP1[ 2 ].y, 0.0 ) * uLightAT2_SpotP0_SpotP1[ 2 ].z ), uLightRange_FallOff_AT0_AT1[ 2 ].y ), 0.0, 1.0 ) ;

		#endif // LG2_SPOT

		// 有効距離外だったら減衰率を最大にする処理
		lLightGen *= step( lLightDistPow2, uLightRange_FallOff_AT0_AT1[ 2 ].x ) ;

		// バンプマップ or フォンシェーディング用に減衰率を保存する
		#if PHONG != 0 || BUMPMAP != 0
			vLight2DirGen.w = lLightGen ;
		#endif // PHONG || BUMPMAP

	#endif // LG2_SPOT || LG2_POINT
	// 距離・スポットライト減衰値計算 =========================( 終了 )




	// ディフューズライトとスペキュラライトの角度減衰計算 =======( 開始 )
	#if BUMPMAP == 0 && PHONG == 0

		lLightLitDest.y = clamp( dot( lViewNrm, -lLightDir ), 0.0, 1.0 ) ;

		#ifdef USE_SPE

			// ハーフベクトルの計算 norm( ( norm( 頂点位置から視点へのベクトル ) + ライトの方向 ) )
			lLightHalfVec = normalize( normalize( -lViewPos.xyz ) - lLightDir ) ;

			// lLightLitDest.z = pow( max( 0.0, N * H ), uMatPow )
			lLightLitDest.z = pow( max( 0.0, dot( lViewNrm, lLightHalfVec ) ), uMatPow ) ;

		#endif // USE_SPE

	#endif // BUMPMAP == 0 && PHONG == 0
	// ディフューズライトとスペキュラライトの角度減衰計算 =======( 終了 )





	// ディフューズカラー計算 =======================================( 開始 )

	#if BUMPMAP == 0 && PHONG == 0

		#if LG2_SPOT != 0 || LG2_POINT != 0

			// ディフューズライト蓄積値 += 距離・スポットライト角度減衰値 * ( ディフューズ角度減衰計算結果 * ディフューズカラー * ライトのディフューズカラー + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの )
			lTotalDif.xyz += lLightGen * ( lLightLitDest.y * uLightDif[ 2 ] * lDifColor.xyz + uLightAmb[ 2 ].xyz ) ;

		#else  // LG2_SPOT || LG2_POINT

			// ディフューズライト蓄積値 += ディフューズ角度減衰計算結果 * ディフューズカラー * ライトのディフューズカラー + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの
			lTotalDif.xyz += lLightLitDest.y * uLightDif[ 2 ] * lDifColor.xyz + uLightAmb[ 2 ].xyz ;

		#endif // LG2_SPOT || LG2_POINT

	#endif // BUMPMAP == 0 && PHONG == 0

	// ディフューズカラー計算 =======================================( 終了 )





	// スペキュラカラー計算 =======================================( 開始 )

	#ifdef USE_SPE

		#if BUMPMAP == 0 && PHONG == 0

			#if LG2_SPOT != 0 || LG2_POINT != 0

				// スペキュラライト蓄積値 += スペキュラ角度減衰計算結果 * 距離・スポットライト減衰 * ライトのスペキュラカラー
				lTotalSpc.xyz += lLightGen * lLightLitDest.z * uLightSpc[ 2 ] ;

			#else	// LG2_SPOT || LG2_POINT

				// スペキュラライト蓄積値 += スペキュラ角度減衰計算結果 * ライトのスペキュラカラー
				lTotalSpc.xyz += lLightLitDest.z * uLightSpc[ 2 ] ;

			#endif	// LG2_SPOT || LG2_POINT

		#endif // BUMPMAP == 0 && PHONG == 0

	#endif // USE_SPE

	// スペキュラカラー計算 =======================================( 終了 )




#endif  // LG2_USE
// ライト2の処理 ***********************************************************************************************************************************( 終了 )















		// 全てのディフューズ要素を合わせて出力レジスタにセット =====( 開始 )

		#if BUMPMAP == 0 && PHONG == 0

			// 出力ディフューズカラー = ライトディフューズカラー蓄積値 + ( マテリアルのアンビエントカラーとグローバルアンビエントカラーを乗算したものとマテリアルエミッシブカラーを加算したもの )
			vDif = lTotalDif + uAmb_Emi ;

			// アルファはディフューズカラーのアルファをそのまま使う
			vDif.w = lDifColor.w ;

		#endif // BUMPMAP == 0 && PHONG == 0

		// 全てのディフューズ要素を合わせて出力レジスタにセット =====( 終了 )





		// 全てのスペキュラ要素を合わせて出力レジスタにセット =====( 開始 )

		#if USE_SPE != 0 && BUMPMAP == 0 && PHONG == 0

			// 出力スペキュラカラー = ライトスペキュラカラー蓄積値 * スペキュラカラー
			vSpc = lTotalSpc * lSpeColor ;

		#endif // USE_SPE && BUMPMAP == 0 && PHONG == 0

		// 全てのスペキュラ要素を合わせて出力レジスタにセット =====( 終了 )






	#endif // LG_USE
	// ライトの処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )















	// フォグ計算 =============================================( 開始 )

	#if FOG_LINEAR != 0 || FOG_EXP != 0 || FOG_EXP2 != 0

		#ifdef FOG_LINEAR

			// 線形フォグ計算
			vNrmFog.w = lViewPos.z * uFog.y + uFog.x ;

		#endif

		#ifdef FOG_EXP

			// 指数フォグ計算 1.0 / pow( e, 距離 * density )
			vNrmFog.w = 1.0 / pow( abs( uFog.w ), lViewPos.z * uFog.z ) ;

		#endif

		#ifdef FOG_EXP2

			// 指数フォグ２計算 1.0 / pow( e, ( 距離 * density ) * ( 距離 * density ) )
			vNrmFog.w = 1.0 / pow( abs( uFog.w ), ( lViewPos.z * uFog.z ) * ( lViewPos.z * uFog.z ) ) ;

		#endif

	#endif // FOG_LINEAR || FOG_EXP || FOG_EXP2

	// フォグ計算 =============================================( 終了 )

#if SHADOWMAP_DRAW != 0
	// 射影座標をテクスチャ座標として出力する =================( 開始 )

	vSM0Pos_SM2PosX.xyz = gl_Position.xyz ;

	// 射影座標をテクスチャ座標として出力する =================( 終了 )
#endif


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
	vVtoEyeSM2PosZ.w  = dot( lWorldPos, uSMLVPMat[ 10 ] ) ;

	// 深度影用のライトから見た射影座標を算出 =================( 終了 )
#endif

	// テクスチャ座標のセット
	vTexUV0_1.x = dot( aTexUV0F4, uTexMat[ 0 ] ) ;
	vTexUV0_1.y = dot( aTexUV0F4, uTexMat[ 1 ] ) ;
	vTexUV0_1.z = dot( aTexUV1F4, uTexMat[ 2 ] ) ;
	vTexUV0_1.w = dot( aTexUV1F4, uTexMat[ 3 ] ) ;
}

