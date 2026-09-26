// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		物理演算処理
// 
// 				Ver 3.25a
// 
// -------------------------------------------------------------------------------

// ＤＸライブラリ作成時用定義
#define DX_MAKE

#include "DxCompileConfig.h"

#if !defined( DX_NON_MODEL ) && !defined( DX_NON_BULLET_PHYSICS )

// インクルード ------------------------------------------------------------------
#include "DxUseCLib.h"
#include "DxLib.h"
#include "DxStatic.h"
#include "DxModel.h"
#include "DxModelRead.h"
#include "btBulletDynamicsCommon.h"
#include "DxModelLoader3.h"
#include "DxModelLoader4.h"

//namespace DxLib
//{

// マクロ定義 -----------------------------------

// データ型宣言 ---------------------------------

// Bullet の剛体を初期化する際に使用する情報
struct BULLET_RIGIDBODY_SETUP_INFO
{
	int					RigidBodyGroupIndex ;	// 剛体グループ番号
	DWORD				RigidBodyGroupTarget ;	// 剛体グループ対象
	int					ShapeType ;				// 形状( 0:球  1:箱  2:カプセル )
	float				ShapeW ;				// 幅
	float				ShapeH ;				// 高さ
	float				ShapeD ;				// 奥行
	VECTOR				Position ;				// 位置
	VECTOR				Rotation ;				// 回転
	float				RigidBodyWeight ;		// 質量
	float				RigidBodyPosDim ;		// 移動減
	float				RigidBodyRotDim ;		// 回転減
	float				RigidBodyRecoil ;		// 反発力
	float				RigidBodyFriction ;		// 摩擦力
	int					RigidBodyType ;			// 剛体タイプ( 0:Bone追従  1:物理演算  2:物理演算(Bone位置合わせ) )
	MATRIX				*InitializeMatrix ;		// 行列の初期値
} ;

// Bullet の剛体ジョイントを初期化する際に使用する情報
struct BULLET_JOINT_SETUP_INFO
{
	D_btRigidBody		*RigidBodyA ;			// 接続先剛体Ａ
	D_btRigidBody		*RigidBodyB ;			// 接続先剛体Ｂ
	VECTOR				Position ;				// 位置
	VECTOR				Rotation ;				// 回転( ラジアン )
	VECTOR				ConstrainPosition1 ;	// 移動制限値１
	VECTOR				ConstrainPosition2 ;	// 移動制限値２
	VECTOR				ConstrainRotation1 ;	// 回転制限値１
	VECTOR				ConstrainRotation2 ;	// 回転制限値２
	VECTOR				SpringPosition ;		// ばね移動値
	VECTOR				SpringRotation ;		// ばね回転値
} ;

// Bullet の剛体処理で使用するデータ
struct BULLET_RIGIDBODY_INFO
{
	int					DisableFlag ;			// 無効フラグ

	D_btCollisionShape	*btColShape ;			// 形状データ
	D_btRigidBody		*btRigdBody ;			// 剛体データ

	D_btTransform		bttrBoneOffset ;		// ボーンのオフセット
	D_btTransform		bttrInvBoneOffset ;		// ボーンの逆オフセット
	struct D_btKinematicMotionState *btMotionState ;
} ;

// Bullet の剛体ジョイント処理で使用するデータ
struct BULLET_JOINT_INFO
{
	int									DisableFlag ;			// 無効フラグ
	D_btGeneric6DofSpringConstraint	*	btcConstraint ;
} ;

// Bullet の Kinematic用モーションステート
struct D_btKinematicMotionState : public D_btMotionState
{
	D_btTransform			GraphicsWorldTrans ;
/*
	D_btTransform			BoneOffset ;
//	D_btTransform			InvBoneOffset ;
	PMD_READ_BONE_INFO		*Bone ;
	PMD_READ_PHYSICS_INFO	*Physics ;

	D_btKinematicMotionState(
		const D_btTransform	&boneOffset,
		PMD_READ_BONE_INFO	*pBone,
		PMD_READ_PHYSICS_INFO *pPhysics )
		:
		BoneOffset( boneOffset ),
		Bone( pBone ),
		Physics( pPhysics )
	{
//		InvBoneOffset = BoneOffset.inverse() ;
//		Flush( true ) ;
	}
*/
	D_btKinematicMotionState(){};

#if 0
	void Flush(bool reset)
	{
		if( Physics->RigidBodyType == 0 || Physics->NoCopyToBone || reset)
		{
			D_btTransform temp ;

			temp.setFromOpenGLMatrix( ( D_btScalar * )&Bone->LocalWorldMatrix ) ;
			GraphicsWorldTrans = temp * BoneOffset ;
		}
/*		else
		{
			D_btTransform temp2 ;

			temp2 = GraphicsWorldTrans * InvBoneOffset ;

			temp2.getOpenGLMatrix( ( D_btScalar * )&Bone->LocalWorldMatrix ) ;
		}
*/	}
#endif

	///synchronizes world transform from user to physics
	virtual void getWorldTransform( D_btTransform &centerOfMassWorldTrans ) const
	{
		centerOfMassWorldTrans = GraphicsWorldTrans ;
	}

	///synchronizes world transform from physics to user
	///Bullet only calls the update of worldtransform for active objects
	virtual void setWorldTransform( const D_btTransform &centerOfMassWorldTrans )
	{
		GraphicsWorldTrans = centerOfMassWorldTrans ;
	}
} ;

// bullet physics の処理に必要なオブジェクトを詰めた構造体
struct BULLET_PHYSICS
{
	D_btDefaultCollisionConfiguration		*CollisionConfig ;			// コリジョンコンフィグ
	D_btCollisionDispatcher					*CollisionDispatcher ;		// コリジョンディスパッチャ
	D_btAxisSweep3							*OverlappingPairCache ;		// コリジョンワールドの範囲
	D_btSequentialImpulseConstraintSolver	*Solver ;					// 高速計算ソルバ
	D_btDiscreteDynamicsWorld				*World ;					// ワールド
//	D_btContinuousDynamicsWorld				*World ;					// ワールド

	D_btCollisionShape						*GroundShape ;				// 床コリジョン
} ;

// 関数宣言 -------------------------------------

static int BulletPhysics_Initialize( BULLET_PHYSICS *BulletPhysicsData, VECTOR Gravity ) ;		// BulletPhysics の初期化
static int BulletPhysics_Terminate( BULLET_PHYSICS *BulletPhysicsData ) ;						// BulletPhysics の後始末

static int BulletPhysics_SetupRigidBody(	BULLET_PHYSICS *BulletPhysicsData,  BULLET_RIGIDBODY_INFO *RigidBodyInfo,	BULLET_RIGIDBODY_SETUP_INFO	*SetupInfo ) ;
static int BulletPhysics_SetupJoint(		BULLET_PHYSICS *BulletPhysicsData,  BULLET_JOINT_INFO	  *JointInfo,		BULLET_JOINT_SETUP_INFO		*SetupInfo ) ;
static int BulletPhysics_ReleaseRigidBody(	BULLET_RIGIDBODY_INFO *RigidBodyInfo ) ;
static int BulletPhysics_ReleaseJoint(		BULLET_JOINT_INFO	  *JointInfo ) ;
static int BulleyPhysics_ResetRigidBody(	BULLET_RIGIDBODY_INFO *RigidBodyInfo, MATRIX *Matrix ) ;

// 条件を限定した少し高速な逆行列計算
static void ModelLoader3_InverseMatrix( MATRIX &InMatrix, MATRIX &OutMatrix ) ;

// PMD用の剛体にボーンの状態を反映する関数
static void PMD_PhysicsMotionState_Flush( D_btKinematicMotionState *pbtMotionState, PMD_READ_PHYSICS_INFO *pPhysics ) ;

// モデルの剛体にボーンの状態を反映する関数
static void Model_PhysicsMotionState_Flush( D_btKinematicMotionState *pbtMotionState, MV1_PHYSICS_RIGIDBODY *pRigidBody ) ;

// プログラム -----------------------------------

// BulletPhysics の初期化
static int BulletPhysics_Initialize( BULLET_PHYSICS *BulletPhysicsData, VECTOR Gravity )
{
	D_btTransform trGroundTransform ;

	// ゼロ初期化
	_MEMSET( BulletPhysicsData, 0, sizeof( *BulletPhysicsData ) ) ;

	// コリジョンコンフィグを作成する
	BulletPhysicsData->CollisionConfig = new D_btDefaultCollisionConfiguration() ;

	// コリジョンディスパッチャを作成する
	BulletPhysicsData->CollisionDispatcher = new D_btCollisionDispatcher( BulletPhysicsData->CollisionConfig ) ;

	// コリジョンワールドの最大サイズを指定する
	BulletPhysicsData->OverlappingPairCache = new D_btAxisSweep3( D_btVector3( -50000.0f, -50000.0f, -50000.0f ), D_btVector3(  50000.0f,  50000.0f,  50000.0f ), 2048 ) ;

	// 拘束計算ソルバを作成する
	BulletPhysicsData->Solver = new D_btSequentialImpulseConstraintSolver() ;

	// ワールドの作成
	BulletPhysicsData->World = new D_btDiscreteDynamicsWorld( BulletPhysicsData->CollisionDispatcher, BulletPhysicsData->OverlappingPairCache, BulletPhysicsData->Solver, BulletPhysicsData->CollisionConfig ) ;
//	BulletPhysicsData->World = new D_btContinuousDynamicsWorld( BulletPhysicsData->CollisionDispatcher, BulletPhysicsData->OverlappingPairCache, BulletPhysicsData->Solver, BulletPhysicsData->CollisionConfig ) ;

	// 重力設定
	BulletPhysicsData->World->setGravity( D_btVector3( Gravity.x, Gravity.y, Gravity.z ) ) ;

	//-----------------------------------------------------
	// 床用として無限平面を作成
//	BulletPhysicsData->GroundShape = new D_btStaticPlaneShape( D_btVector3( 0.0f, 1.0f, 0.0f ), 0.0f ) ;

	// 床のトランスフォームを設定
//	trGroundTransform.setIdentity() ;

	// MotionStateを作成する。剛体の姿勢制御をするもの
//	D_btMotionState *pMotionState = new D_btDefaultMotionState( trGroundTransform ) ;

	// 剛体を作成する
	// 質量 0.0、慣性テンソル 0.0 ならこの剛体は動かない
//	D_btRigidBody::D_btRigidBodyConstructionInfo rbInfo( 0.0f, pMotionState, BulletPhysicsData->GroundShape, D_btVector3( 0.0f, 0.0f, 0.0f ) ) ;

	// 物理ワールドに床を追加
//	BulletPhysicsData->World->addRigidBody( new D_btRigidBody( rbInfo ) ) ;

	return 0 ;
}

// BulletPhysics の後始末
static int BulletPhysics_Terminate( BULLET_PHYSICS *BulletPhysicsData )
{
	int i ;
	D_btCollisionObject *pObj ;
	D_btTypedConstraint *pConst ;
	D_btRigidBody *pRigidBody ;

	if( BulletPhysicsData->World == NULL )
		return -1 ;

	for( i = BulletPhysicsData->World->getNumConstraints() - 1 ; i >= 0 ; i -- )
	{
		pConst = BulletPhysicsData->World->getConstraint( i ) ;
		BulletPhysicsData->World->removeConstraint( pConst ) ;

		delete pConst ;
	}

	for( i = BulletPhysicsData->World->getNumCollisionObjects() - 1 ; i >= 0 ; i -- )
	{
		pObj = BulletPhysicsData->World->getCollisionObjectArray()[ i ] ;
		pRigidBody = D_btRigidBody::upcast( pObj ) ;

		if( pRigidBody && pRigidBody->getMotionState() )
			delete pRigidBody->getMotionState() ;

		BulletPhysicsData->World->removeCollisionObject( pObj ) ;

		delete pObj ;
	}

	if( BulletPhysicsData->GroundShape )
	{
		delete BulletPhysicsData->GroundShape ;
		BulletPhysicsData->GroundShape = NULL ;
	}

	if( BulletPhysicsData->World )
	{
		delete BulletPhysicsData->World ;
		BulletPhysicsData->World = NULL ;
	}

	if( BulletPhysicsData->Solver )
	{
		delete BulletPhysicsData->Solver ;
		BulletPhysicsData->Solver = NULL ;
	}

	if( BulletPhysicsData->OverlappingPairCache )
	{
		delete BulletPhysicsData->OverlappingPairCache ;
		BulletPhysicsData->OverlappingPairCache = NULL ;
	}

	if( BulletPhysicsData->CollisionDispatcher )
	{
		delete BulletPhysicsData->CollisionDispatcher ;
		BulletPhysicsData->CollisionDispatcher = NULL ;
	}

	if( BulletPhysicsData->CollisionConfig )
	{
		delete BulletPhysicsData->CollisionConfig ;
		BulletPhysicsData->CollisionConfig = NULL ;
	}

	return 0 ;
}

static int BulletPhysics_SetupRigidBody( BULLET_PHYSICS *BulletPhysicsData, BULLET_RIGIDBODY_INFO *RigidBodyInfo, BULLET_RIGIDBODY_SETUP_INFO *SetupInfo )
{
	RigidBodyInfo->DisableFlag = FALSE ;

	// ボーンオフセット用トランスフォーム作成
	D_btMatrix3x3	btmRotationMat ;
	btmRotationMat.setEulerZYX( SetupInfo->Rotation.x, SetupInfo->Rotation.y, SetupInfo->Rotation.z ) ;
	RigidBodyInfo->bttrBoneOffset.setIdentity() ;
	RigidBodyInfo->bttrBoneOffset.setOrigin( D_btVector3( SetupInfo->Position.x, SetupInfo->Position.y, SetupInfo->Position.z ) ) ;
	RigidBodyInfo->bttrBoneOffset.setBasis( btmRotationMat ) ;
	RigidBodyInfo->bttrInvBoneOffset = RigidBodyInfo->bttrBoneOffset.inverse() ;

	// シェイプの作成
	switch( SetupInfo->ShapeType )
	{
	case 0 : RigidBodyInfo->btColShape = new D_btSphereShape( SetupInfo->ShapeW ) ; break ;														// 球
	case 1 : RigidBodyInfo->btColShape = new D_btBoxShape( D_btVector3( SetupInfo->ShapeW, SetupInfo->ShapeH, SetupInfo->ShapeD ) ) ; break ;	// 箱
	case 2 : RigidBodyInfo->btColShape = new D_btCapsuleShape( SetupInfo->ShapeW, SetupInfo->ShapeH ) ; break ;									// カプセル
	}

	// 質量と慣性テンソルの設定
	D_btScalar	btsMass( 0.0f ) ;
	D_btVector3	btv3LocalInertia( 0.0f, 0.0f ,0.0f ) ;

	// ボーン追従でない場合は質量を設定
	if( SetupInfo->RigidBodyType != 0 )
		btsMass = SetupInfo->RigidBodyWeight ;

	// 慣性テンソルの計算
	if( SetupInfo->RigidBodyType != 0 )
		RigidBodyInfo->btColShape->calculateLocalInertia( btsMass, btv3LocalInertia ) ;

	// MotionStateの作成
	D_btMotionState *pbtMotionState ;
	pbtMotionState = new D_btKinematicMotionState() ;
	RigidBodyInfo->btMotionState = ( D_btKinematicMotionState * )pbtMotionState ;

	// モーションステートの中の行列を初期化
	D_btTransform temp ;
	temp.setFromOpenGLMatrix( ( D_btScalar * )SetupInfo->InitializeMatrix ) ;
	RigidBodyInfo->btMotionState->GraphicsWorldTrans = temp * RigidBodyInfo->bttrBoneOffset ;

	// 剛体のパラメータの設定
	D_btRigidBody::D_btRigidBodyConstructionInfo btRbInfo( btsMass, pbtMotionState, RigidBodyInfo->btColShape, btv3LocalInertia ) ;
	btRbInfo.m_linearDamping  = SetupInfo->RigidBodyPosDim ;	// 移動減
	btRbInfo.m_angularDamping = SetupInfo->RigidBodyRotDim ;	// 回転減
	btRbInfo.m_restitution    = SetupInfo->RigidBodyRecoil ;	// 反発力
	btRbInfo.m_friction       = SetupInfo->RigidBodyFriction ;	// 摩擦力
	btRbInfo.m_additionalDamping = true ;

	// 剛体の作成
	RigidBodyInfo->btRigdBody = new D_btRigidBody( btRbInfo ) ;

	// Kinematic設定
	if( SetupInfo->RigidBodyType == 0 )
	{
		RigidBodyInfo->btRigdBody->setCollisionFlags( RigidBodyInfo->btRigdBody->getCollisionFlags() | D_btCollisionObject::D_CF_KINEMATIC_OBJECT ) ;
		RigidBodyInfo->btRigdBody->setActivationState( D_DISABLE_DEACTIVATION ) ;
	}
	RigidBodyInfo->btRigdBody->setSleepingThresholds( 0.0f, 0.0f ) ;

	// 剛体をシミュレーションワールドに追加
	BulletPhysicsData->World->addRigidBody( RigidBodyInfo->btRigdBody, ( short )( 0x0001 << SetupInfo->RigidBodyGroupIndex ), ( short )SetupInfo->RigidBodyGroupTarget ) ;

	// 終了
	return 0 ;
}

static int BulletPhysics_SetupJoint( BULLET_PHYSICS *BulletPhysicsData,  BULLET_JOINT_INFO *JointInfo, BULLET_JOINT_SETUP_INFO *SetupInfo )
{
	JointInfo->DisableFlag = FALSE ;

	// 回転行列作成
	D_btMatrix3x3	btmRotationMat ;
	btmRotationMat.setEulerZYX( SetupInfo->Rotation.x, SetupInfo->Rotation.y, SetupInfo->Rotation.z ) ;

	// コンストレイントのトランスフォームを作成
	D_btTransform bttrTransform ;
	bttrTransform.setIdentity() ;
	bttrTransform.setOrigin( D_btVector3( SetupInfo->Position.x, SetupInfo->Position.y, SetupInfo->Position.z ) ) ;
	bttrTransform.setBasis( btmRotationMat ) ;

	// 剛体A,Bから見たコンストレイントを作成 
	JointInfo->btcConstraint = new D_btGeneric6DofSpringConstraint(
		*SetupInfo->RigidBodyA,
		*SetupInfo->RigidBodyB,
		SetupInfo->RigidBodyA->getWorldTransform().inverse() * bttrTransform,
		SetupInfo->RigidBodyB->getWorldTransform().inverse() * bttrTransform, true ) ;

	// 各種制限パラメータのセット
	JointInfo->btcConstraint->setLinearLowerLimit( D_btVector3( SetupInfo->ConstrainPosition1.x, SetupInfo->ConstrainPosition1.y, SetupInfo->ConstrainPosition1.z ) ) ;
	JointInfo->btcConstraint->setLinearUpperLimit( D_btVector3( SetupInfo->ConstrainPosition2.x, SetupInfo->ConstrainPosition2.y, SetupInfo->ConstrainPosition2.z ) ) ;

	JointInfo->btcConstraint->setAngularLowerLimit( D_btVector3( SetupInfo->ConstrainRotation1.x, SetupInfo->ConstrainRotation1.y, SetupInfo->ConstrainRotation1.z ) ) ;
	JointInfo->btcConstraint->setAngularUpperLimit( D_btVector3( SetupInfo->ConstrainRotation2.x, SetupInfo->ConstrainRotation2.y, SetupInfo->ConstrainRotation2.z ) ) ;

	// 0 : translation X
	if( SetupInfo->SpringPosition.x != 0.0f )
	{
		JointInfo->btcConstraint->enableSpring( 0, true ) ;
		JointInfo->btcConstraint->setStiffness( 0, SetupInfo->SpringPosition.x ) ;
	}

	// 1 : translation Y
	if( SetupInfo->SpringPosition.y != 0.0f )
	{
		JointInfo->btcConstraint->enableSpring( 1, true ) ;
		JointInfo->btcConstraint->setStiffness( 1, SetupInfo->SpringPosition.y ) ;
	}

	// 2 : translation Z
	if( SetupInfo->SpringPosition.z != 0.0f )
	{
		JointInfo->btcConstraint->enableSpring( 2, true ) ;
		JointInfo->btcConstraint->setStiffness( 2, SetupInfo->SpringPosition.z ) ;
	}

	// 3 : rotation X (3rd Euler rotational around new position of X axis, range [-PI+epsilon, PI-epsilon] )
	// 4 : rotation Y (2nd Euler rotational around new position of Y axis, range [-PI/2+epsilon, PI/2-epsilon] )
	// 5 : rotation Z (1st Euler rotational around Z axis, range [-PI+epsilon, PI-epsilon] )
	JointInfo->btcConstraint->enableSpring( 3, true ) ; JointInfo->btcConstraint->setStiffness( 3, SetupInfo->SpringRotation.x ) ;
	JointInfo->btcConstraint->enableSpring( 4, true ) ; JointInfo->btcConstraint->setStiffness( 4, SetupInfo->SpringRotation.y ) ;
	JointInfo->btcConstraint->enableSpring( 5, true ) ; JointInfo->btcConstraint->setStiffness( 5, SetupInfo->SpringRotation.z ) ;

	JointInfo->btcConstraint->calculateTransforms() ;
	JointInfo->btcConstraint->setEquilibriumPoint() ;

	// シミュレーションワールドに追加
	BulletPhysicsData->World->addConstraint( JointInfo->btcConstraint ) ;

	// 終了
	return 0 ;
}

static int BulletPhysics_ReleaseRigidBody( BULLET_RIGIDBODY_INFO *RigidBodyInfo )
{
	if( RigidBodyInfo->DisableFlag )
	{
		return 0 ;
	}

	if( RigidBodyInfo->btColShape )
	{
		delete RigidBodyInfo->btColShape ;
		RigidBodyInfo->btColShape = NULL ;
	}

	RigidBodyInfo->btMotionState = NULL ;
	RigidBodyInfo->btRigdBody = NULL ;

	// 終了
	return 0 ;
}

static int BulletPhysics_ReleaseJoint( BULLET_JOINT_INFO *JointInfo )
{
	if( JointInfo->DisableFlag )
	{
		return 0 ;
	}

	JointInfo->btcConstraint = NULL ;

	// 終了
	return 0 ;
}

static int BulleyPhysics_ResetRigidBody( BULLET_RIGIDBODY_INFO *RigidBodyInfo, MATRIX *Matrix )
{
	D_btTransform	bttrRbTransform ;

	if( RigidBodyInfo->DisableFlag )
	{
		return 0 ;
	}

	bttrRbTransform.setFromOpenGLMatrix( ( D_btScalar * )Matrix ) ;
	RigidBodyInfo->btMotionState->GraphicsWorldTrans = bttrRbTransform * RigidBodyInfo->bttrBoneOffset ;
	RigidBodyInfo->btRigdBody->setCenterOfMassTransform( RigidBodyInfo->btMotionState->GraphicsWorldTrans ) ;

	RigidBodyInfo->btRigdBody->setInterpolationWorldTransform( RigidBodyInfo->btRigdBody->getCenterOfMassTransform() ) ;
	RigidBodyInfo->btRigdBody->setLinearVelocity(               D_btVector3( 0.0f, 0.0f, 0.0f ) ) ;
	RigidBodyInfo->btRigdBody->setAngularVelocity(              D_btVector3( 0.0f, 0.0f, 0.0f ) ) ;
	RigidBodyInfo->btRigdBody->setInterpolationLinearVelocity(  D_btVector3( 0.0f, 0.0f, 0.0f ) ) ;
	RigidBodyInfo->btRigdBody->setInterpolationAngularVelocity( D_btVector3( 0.0f, 0.0f, 0.0f ) ) ;
	RigidBodyInfo->btRigdBody->clearForces() ;

	// 終了
	return 0 ;
}


// 条件を限定した少し高速な逆行列計算
static void ModelLoader3_InverseMatrix( MATRIX &InMatrix, MATRIX &OutMatrix )
{
	OutMatrix.m[ 0 ][ 0 ] = InMatrix.m[ 0 ][ 0 ] ;
	OutMatrix.m[ 0 ][ 1 ] = InMatrix.m[ 1 ][ 0 ] ;
	OutMatrix.m[ 0 ][ 2 ] = InMatrix.m[ 2 ][ 0 ] ;
	OutMatrix.m[ 0 ][ 3 ] = 0.0f ;
	OutMatrix.m[ 1 ][ 0 ] = InMatrix.m[ 0 ][ 1 ] ;
	OutMatrix.m[ 1 ][ 1 ] = InMatrix.m[ 1 ][ 1 ] ;
	OutMatrix.m[ 1 ][ 2 ] = InMatrix.m[ 2 ][ 1 ] ;
	OutMatrix.m[ 1 ][ 3 ] = 0.0f ;
	OutMatrix.m[ 2 ][ 0 ] = InMatrix.m[ 0 ][ 2 ] ;
	OutMatrix.m[ 2 ][ 1 ] = InMatrix.m[ 1 ][ 2 ] ;
	OutMatrix.m[ 2 ][ 2 ] = InMatrix.m[ 2 ][ 2 ] ;
	OutMatrix.m[ 2 ][ 3 ] = 0.0f ;

	OutMatrix.m[ 3 ][ 0 ] = -( InMatrix.m[ 3 ][ 0 ] * OutMatrix.m[0][0] + InMatrix.m[ 3 ][ 1 ] * OutMatrix.m[1][0] + InMatrix.m[ 3 ][ 2 ] * OutMatrix.m[2][0] ) ;
	OutMatrix.m[ 3 ][ 1 ] = -( InMatrix.m[ 3 ][ 0 ] * OutMatrix.m[0][1] + InMatrix.m[ 3 ][ 1 ] * OutMatrix.m[1][1] + InMatrix.m[ 3 ][ 2 ] * OutMatrix.m[2][1] ) ;
	OutMatrix.m[ 3 ][ 2 ] = -( InMatrix.m[ 3 ][ 0 ] * OutMatrix.m[0][2] + InMatrix.m[ 3 ][ 1 ] * OutMatrix.m[1][2] + InMatrix.m[ 3 ][ 2 ] * OutMatrix.m[2][2] ) ;
	OutMatrix.m[ 3 ][ 3 ] = 1.0f ;
}

// PMD用の剛体にボーンの状態を反映する関数
static void PMD_PhysicsMotionState_Flush( bool Reset, D_btKinematicMotionState *pbtMotionState, PMD_READ_PHYSICS_INFO *pPhysics )
{
	BULLET_RIGIDBODY_INFO *BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )pPhysics->BulletInfo ;

	if( pPhysics->RigidBodyType == 0 || pPhysics->NoCopyToBone || Reset )
	{
		D_btTransform temp ;

		temp.setFromOpenGLMatrix( ( D_btScalar * )&pPhysics->Bone->LocalWorldMatrix ) ;
		pbtMotionState->GraphicsWorldTrans = temp * BulletRigidBodyInfo->bttrBoneOffset ;
	}
/*	else
	{
		D_btTransform temp2 ;

		temp2 = GraphicsWorldTrans * InvBoneOffset ;

		temp2.getOpenGLMatrix( ( D_btScalar * )&pPhysics->Bone->LocalWorldMatrix ) ;
	}
*/
}

// PMX用の剛体にボーンの状態を反映する関数
static void PMX_PhysicsMotionState_Flush( bool Reset, D_btKinematicMotionState *pbtMotionState, PMX_READ_PHYSICS_INFO *pPhysics )
{
	BULLET_RIGIDBODY_INFO *BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )pPhysics->BulletInfo ;

	if( pPhysics->Base->RigidBodyType == 0 || pPhysics->NoCopyToBone || Reset )
	{
		D_btTransform temp ;

		temp.setFromOpenGLMatrix( ( D_btScalar * )&pPhysics->Bone->LocalWorldMatrix ) ;
		pbtMotionState->GraphicsWorldTrans = temp * BulletRigidBodyInfo->bttrBoneOffset ;
	}
/*	else
	{
		D_btTransform temp2 ;

		temp2 = GraphicsWorldTrans * InvBoneOffset ;

		temp2.getOpenGLMatrix( ( D_btScalar * )&pPhysics->Bone->LocalWorldMatrix ) ;
	}
*/
}

// モデルの剛体にボーンの状態を反映する関数
static void Model_PhysicsMotionState_Flush( D_btKinematicMotionState *pbtMotionState, MV1_PHYSICS_RIGIDBODY *pRigidBody )
{
	BULLET_RIGIDBODY_INFO *BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )pRigidBody->BulletInfo ;
	MATRIX tempMatrix ;

	if( pRigidBody->BaseData->RigidBodyType == 0 || pRigidBody->BaseData->NoCopyToBone )
	{
		D_btTransform temp ;

		ConvertMatrix4x4cToMatrixF( &tempMatrix, &pRigidBody->TargetFrame->LocalWorldMatrix ) ;
		temp.setFromOpenGLMatrix( ( D_btScalar * )&tempMatrix ) ;
		pbtMotionState->GraphicsWorldTrans = temp * BulletRigidBodyInfo->bttrBoneOffset ;
	}
/*	else
	{
		D_btTransform temp2 ;

		temp2 = GraphicsWorldTrans * InvBoneOffset ;

		temp2.getOpenGLMatrix( ( D_btScalar * )&pPhysics->Bone->LocalWorldMatrix ) ;
	}
*/
}


extern int SetupPhysicsObject_ModelPhysicsInfo( MV1_MODEL *Model )
{
	int i ;
	MV1_PHYSICS_RIGIDBODY *RigidBody ;
	MV1_PHYSICS_JOINT *Joint ;
	MV1_PHYSICS_RIGIDBODY_BASE *RigidBodyBase ;
	MV1_PHYSICS_JOINT_BASE *JointBase ;
	BULLET_RIGIDBODY_INFO *BulletRigidBodyInfo ;
	BULLET_JOINT_INFO *BulletJointInfo ;
	BULLET_PHYSICS *Bullet = ( BULLET_PHYSICS * )Model->BulletPhysicsDataBuffer ;
	BULLET_RIGIDBODY_SETUP_INFO RigidBodySetupInfo ;
	BULLET_JOINT_SETUP_INFO JointSetupInfo ;
	MATRIX InitializeMatrix ;

	// BulletPhysics の初期化
	BulletPhysics_Initialize( Bullet, VGet( 0.0f, Model->PhysicsGravity, 0.0f ) ) ;

	// 剛体オブジェクトのセットアップ
	RigidBody = Model->PhysicsRigidBody ;
	for( i = 0 ; i < Model->BaseData->PhysicsRigidBodyNum ; i ++, RigidBody ++ )
	{
		BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )RigidBody->BulletInfo ;
		RigidBodyBase = RigidBody->BaseData ;

		ConvertMatrix4x4cToMatrixF( &InitializeMatrix, &RigidBody->TargetFrame->LocalWorldMatrix ) ;

		// 剛体の初期化
		RigidBodySetupInfo.RigidBodyGroupIndex = RigidBodyBase->RigidBodyGroupIndex ;
		RigidBodySetupInfo.RigidBodyGroupTarget = RigidBodyBase->RigidBodyGroupTarget ;
		RigidBodySetupInfo.ShapeType = RigidBodyBase->ShapeType ;
		RigidBodySetupInfo.ShapeW = RigidBodyBase->ShapeW ;
		RigidBodySetupInfo.ShapeH = RigidBodyBase->ShapeH ;
		RigidBodySetupInfo.ShapeD = RigidBodyBase->ShapeD ;
		RigidBodySetupInfo.Position = RigidBodyBase->Position ;
		RigidBodySetupInfo.Rotation = RigidBodyBase->Rotation ;
		RigidBodySetupInfo.RigidBodyWeight = RigidBodyBase->RigidBodyWeight ;
		RigidBodySetupInfo.RigidBodyPosDim = RigidBodyBase->RigidBodyPosDim ;
		RigidBodySetupInfo.RigidBodyRotDim = RigidBodyBase->RigidBodyRotDim ;
		RigidBodySetupInfo.RigidBodyRecoil = RigidBodyBase->RigidBodyRecoil ;
		RigidBodySetupInfo.RigidBodyFriction = RigidBodyBase->RigidBodyFriction ;
		RigidBodySetupInfo.RigidBodyType = RigidBodyBase->RigidBodyType ;
		RigidBodySetupInfo.InitializeMatrix = &InitializeMatrix ;
		BulletPhysics_SetupRigidBody( Bullet, BulletRigidBodyInfo, &RigidBodySetupInfo ) ;
	}

	// ジョイントオブジェクトのセットアップ
	Joint = Model->PhysicsJoint ;
	for( i = 0 ; i < Model->BaseData->PhysicsJointNum ; i ++, Joint ++ )
	{
		BulletJointInfo = ( BULLET_JOINT_INFO * )Joint->BulletInfo ;

		JointBase = Joint->BaseData ;

		// ジョイントの初期化
		JointSetupInfo.RigidBodyA = ( ( BULLET_RIGIDBODY_INFO * )Model->PhysicsRigidBody[ JointBase->RigidBodyA->Index ].BulletInfo )->btRigdBody ;
		JointSetupInfo.RigidBodyB = ( ( BULLET_RIGIDBODY_INFO * )Model->PhysicsRigidBody[ JointBase->RigidBodyB->Index ].BulletInfo )->btRigdBody ;
		JointSetupInfo.Position = JointBase->Position ;
		JointSetupInfo.Rotation = JointBase->Rotation ;
		JointSetupInfo.ConstrainPosition1 = JointBase->ConstrainPosition1 ;
		JointSetupInfo.ConstrainPosition2 = JointBase->ConstrainPosition2 ;
		JointSetupInfo.ConstrainRotation1 = JointBase->ConstrainRotation1 ;
		JointSetupInfo.ConstrainRotation2 = JointBase->ConstrainRotation2 ;
		JointSetupInfo.SpringPosition = JointBase->SpringPosition ;
		JointSetupInfo.SpringRotation = JointBase->SpringRotation ;
		BulletPhysics_SetupJoint( Bullet, BulletJointInfo, &JointSetupInfo ) ;
	}

	// 終了
	return 0 ;
}

extern int ReleasePhysicsObject_ModelPhysicsInfo( MV1_MODEL *Model )
{
	int i ;
	BULLET_RIGIDBODY_INFO *BulletRigidBodyInfo ;
	BULLET_JOINT_INFO *BulletJointInfo ;
	MV1_PHYSICS_RIGIDBODY *RigidBody ;
	MV1_PHYSICS_JOINT *Joint ;
	BULLET_PHYSICS *Bullet = ( BULLET_PHYSICS * )Model->BulletPhysicsDataBuffer ;

	// 剛体オブジェクトの後始末
	RigidBody = Model->PhysicsRigidBody ;
	for( i = 0 ; i < Model->BaseData->PhysicsRigidBodyNum ; i ++, RigidBody ++ )
	{
		BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )RigidBody->BulletInfo ;

		BulletPhysics_ReleaseRigidBody( BulletRigidBodyInfo ) ;
	}

	// ジョイントオブジェクトの後始末
	Joint = Model->PhysicsJoint ;
	for( i = 0 ; i < Model->BaseData->PhysicsJointNum ; i ++, Joint ++ )
	{
		BulletJointInfo = ( BULLET_JOINT_INFO * )Joint->BulletInfo ;

		BulletPhysics_ReleaseJoint( BulletJointInfo ) ;
	}

	// BulletPhysics の後始末
	BulletPhysics_Terminate( Bullet ) ;

	// 終了
	return 0 ;
}

extern int ResetState_ModelPhysicsInfo( MV1_MODEL *Model )
{
	BULLET_RIGIDBODY_INFO *BulletRigidBodyInfo ;
	MV1_PHYSICS_RIGIDBODY *PhysicsRigidBody ;
	int i ;

	PhysicsRigidBody = Model->PhysicsRigidBody ;
	for( i = 0 ; i < Model->BaseData->PhysicsRigidBodyNum ; i ++, PhysicsRigidBody ++ )
	{
		BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )PhysicsRigidBody->BulletInfo ;

		if( BulletRigidBodyInfo->DisableFlag )
		{
			continue ;
		}

		if( PhysicsRigidBody->TargetFrame->ValidLocalWorldMatrixNM == false )
		{
			PhysicsRigidBody->TargetFrame->ValidLocalWorldMatrixNM = true ;
			ConvertMatrix4x4cToMatrixF( &PhysicsRigidBody->TargetFrame->LocalWorldMatrixNM, &PhysicsRigidBody->TargetFrame->LocalWorldMatrix ) ;
		}

		BulleyPhysics_ResetRigidBody( BulletRigidBodyInfo, &PhysicsRigidBody->TargetFrame->LocalWorldMatrixNM ) ;
	}

	Model->PhysicsResetRequestFlag = TRUE ;

	// 終了
	return 0 ;
}

extern int StepSimulation_ModelPhysicsInfo( MV1_MODEL *Model, float TimeStep )
{
	BULLET_RIGIDBODY_INFO *BulletRigidBodyInfo ;
	MV1_PHYSICS_RIGIDBODY *PhysicsRigidBody ;
	BULLET_PHYSICS *Bullet = ( BULLET_PHYSICS * )Model->BulletPhysicsDataBuffer ;
	MATRIX tempMatrix ;
	int i ;
	float FixedTimeStep ;

	// リセットリクエストがある場合は最初にボーンの位置合わせ
	if( Model->PhysicsResetRequestFlag )
	{
		Model->PhysicsResetRequestFlag = FALSE ;

		// ボーン位置あわせ
		PhysicsRigidBody = Model->PhysicsRigidBody ;
		for( i = 0 ; i < Model->BaseData->PhysicsRigidBodyNum ; i ++, PhysicsRigidBody ++ )
		{
			BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )PhysicsRigidBody->BulletInfo ;

			Model_PhysicsMotionState_Flush( BulletRigidBodyInfo->btMotionState, PhysicsRigidBody ) ;
		}
	}

	// 物理処理を実行する
	FixedTimeStep = TimeStep < 1.0f / 60.0f ? TimeStep : 1.0f / 60.0f ;
	Bullet->World->stepSimulation( TimeStep, 1, FixedTimeStep ) ;

	// ボーン位置あわせ
	PhysicsRigidBody = Model->PhysicsRigidBody ;
	for( i = 0 ; i < Model->BaseData->PhysicsRigidBodyNum ; i ++, PhysicsRigidBody ++ )
	{
		BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )PhysicsRigidBody->BulletInfo ;

		Model_PhysicsMotionState_Flush( BulletRigidBodyInfo->btMotionState, PhysicsRigidBody ) ;
	}

	// 現在の剛体の状態を表す行列を取得する
	PhysicsRigidBody = Model->PhysicsRigidBody ;
	for( i = 0 ; i < Model->BaseData->PhysicsRigidBodyNum ; i ++, PhysicsRigidBody ++ )
	{
		if( PhysicsRigidBody->BaseData->RigidBodyType == 0 || PhysicsRigidBody->BaseData->NoCopyToBone ) continue ;

		BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )PhysicsRigidBody->BulletInfo ;

		( BulletRigidBodyInfo->btRigdBody->getCenterOfMassTransform() * BulletRigidBodyInfo->bttrInvBoneOffset ).getOpenGLMatrix( ( float * )&tempMatrix );
		ConvertMatrixFToMatrix4x4c( &PhysicsRigidBody->TargetFrame->LocalWorldMatrix, &tempMatrix ) ;
	}

	// 終了
	return 0 ;
}

extern int SetWorldGravity_ModelPhysiceInfo( MV1_MODEL *Model, VECTOR Gravity )
{
	BULLET_PHYSICS *Bullet = ( BULLET_PHYSICS * )Model->BulletPhysicsDataBuffer ;

	Bullet->World->setGravity( D_btVector3( Gravity.x, Gravity.y, Gravity.z ) ) ;

	// 終了
	return 0 ;
}

extern int SetupPhysicsObject_PMDPhysicsInfo(
	DX_MODELLOADER3_PMD_PHYSICS_INFO *MLPhysicsInfo,
	int DisablePhysicsFile
)
{
	int i ;
	PMD_READ_PHYSICS_INFO *PhysicsInfo ;
	PMD_READ_PHYSICS_JOINT_INFO *JointInfo ;
	BULLET_RIGIDBODY_INFO *BulletRigidBodyInfo ;
	BULLET_JOINT_INFO *BulletJointInfo ;
	PMD_PHYSICS_JOINT *PmdPJoint ;
	BULLET_PHYSICS *Bullet = ( BULLET_PHYSICS * )MLPhysicsInfo->BulletPhysicsDataBuffer ;
	BULLET_RIGIDBODY_SETUP_INFO RigidBodySetupInfo ;
	BULLET_JOINT_SETUP_INFO JointSetupInfo ;

	// BulletPhysics の初期化
	BulletPhysics_Initialize( Bullet, MLPhysicsInfo->Gravity ) ;

	// 剛体オブジェクトのセットアップ
	PhysicsInfo = MLPhysicsInfo->PmdPhysicsInfoDim ;
	for( i = 0 ; i < MLPhysicsInfo->PmdPhysicsNum ; i ++, PhysicsInfo ++ )
	{
		BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )PhysicsInfo->BulletInfo ;

		// 無効化チェック
		BulletRigidBodyInfo->DisableFlag = MV1LoadModelDisablePhysicsNameCheck_ShiftJIS( PhysicsInfo->Name, DisablePhysicsFile ) ;

		// 無効の場合は初期化処理を行わない
		if( BulletRigidBodyInfo->DisableFlag )
		{
			continue ;
		}

		// 剛体の初期化
		RigidBodySetupInfo.RigidBodyGroupIndex = ( int )PhysicsInfo->Base->Data[ 22 ] ;
		RigidBodySetupInfo.RigidBodyGroupTarget = *( ( WORD * )&PhysicsInfo->Base->Data[ 23 ] ) ;
		RigidBodySetupInfo.ShapeType = PhysicsInfo->ShapeType ;
		RigidBodySetupInfo.ShapeW = *( ( float * )&PhysicsInfo->Base->Data[ 26 ] ) ;
		RigidBodySetupInfo.ShapeH = *( ( float * )&PhysicsInfo->Base->Data[ 30 ] ) ;
		RigidBodySetupInfo.ShapeD = *( ( float * )&PhysicsInfo->Base->Data[ 34 ] ) ;
		RigidBodySetupInfo.Position.x = *( ( float * )&PhysicsInfo->Base->Data[ 38 ] ) ;
		RigidBodySetupInfo.Position.y = *( ( float * )&PhysicsInfo->Base->Data[ 42 ] ) ;
		RigidBodySetupInfo.Position.z = *( ( float * )&PhysicsInfo->Base->Data[ 46 ] ) ;
		RigidBodySetupInfo.Rotation.x = *( ( float * )&PhysicsInfo->Base->Data[ 50 ] ) ;
		RigidBodySetupInfo.Rotation.y = *( ( float * )&PhysicsInfo->Base->Data[ 54 ] ) ;
		RigidBodySetupInfo.Rotation.z = *( ( float * )&PhysicsInfo->Base->Data[ 58 ] ) ;
		RigidBodySetupInfo.RigidBodyWeight = *( ( float * )&PhysicsInfo->Base->Data[ 62 ] ) ;
		RigidBodySetupInfo.RigidBodyPosDim = *( ( float * )&PhysicsInfo->Base->Data[ 66 ] ) ;
		RigidBodySetupInfo.RigidBodyRotDim = *( ( float * )&PhysicsInfo->Base->Data[ 70 ] ) ;
		RigidBodySetupInfo.RigidBodyRecoil = *( ( float * )&PhysicsInfo->Base->Data[ 74 ] ) ;
		RigidBodySetupInfo.RigidBodyFriction = *( ( float * )&PhysicsInfo->Base->Data[ 78 ] ) ;
		RigidBodySetupInfo.RigidBodyType = PhysicsInfo->RigidBodyType ;
		RigidBodySetupInfo.InitializeMatrix = &PhysicsInfo->Bone->LocalWorldMatrix ;
		BulletPhysics_SetupRigidBody( Bullet, BulletRigidBodyInfo, &RigidBodySetupInfo ) ;
	}

	// ジョイントオブジェクトのセットアップ
	JointInfo = MLPhysicsInfo->PmdPhysicsJointInfoDim ;
	for( i = 0 ; i < MLPhysicsInfo->PmdPhysicsJointNum ; i ++, JointInfo ++ )
	{
		BULLET_RIGIDBODY_INFO *BulletRigidBodyInfoA ;
		BULLET_RIGIDBODY_INFO *BulletRigidBodyInfoB ;

		BulletJointInfo = ( BULLET_JOINT_INFO * )JointInfo->BulletInfo ;

		PmdPJoint = &JointInfo->Base ;

		// どちらかの剛体が無効かチェック
		BulletRigidBodyInfoA = ( BULLET_RIGIDBODY_INFO * )MLPhysicsInfo->PmdPhysicsInfoDim[ PmdPJoint->RigidBodyA ].BulletInfo ;
		BulletRigidBodyInfoB = ( BULLET_RIGIDBODY_INFO * )MLPhysicsInfo->PmdPhysicsInfoDim[ PmdPJoint->RigidBodyB ].BulletInfo ;
		if( BulletRigidBodyInfoA->DisableFlag ||
			BulletRigidBodyInfoB->DisableFlag )
		{
			BulletJointInfo->DisableFlag = TRUE ;
		}
		else
		{
			BulletJointInfo->DisableFlag = FALSE ;
		}

		// 無効の場合は初期化処理を行わない
		if( BulletJointInfo->DisableFlag )
		{
			continue ;
		}

		// ジョイントの初期化
		JointSetupInfo.RigidBodyA = BulletRigidBodyInfoA->btRigdBody ;
		JointSetupInfo.RigidBodyB = BulletRigidBodyInfoB->btRigdBody ;
		JointSetupInfo.Position.x = PmdPJoint->Position[ 0 ] ;
		JointSetupInfo.Position.y = PmdPJoint->Position[ 1 ] ;
		JointSetupInfo.Position.z = PmdPJoint->Position[ 2 ] ;
		JointSetupInfo.Rotation.x = PmdPJoint->Rotation[ 0 ] ;
		JointSetupInfo.Rotation.y = PmdPJoint->Rotation[ 1 ] ;
		JointSetupInfo.Rotation.z = PmdPJoint->Rotation[ 2 ] ;
		JointSetupInfo.ConstrainPosition1.x = PmdPJoint->ConstrainPosition1[ 0 ] ;
		JointSetupInfo.ConstrainPosition1.y = PmdPJoint->ConstrainPosition1[ 1 ] ;
		JointSetupInfo.ConstrainPosition1.z = PmdPJoint->ConstrainPosition1[ 2 ] ;
		JointSetupInfo.ConstrainPosition2.x = PmdPJoint->ConstrainPosition2[ 0 ] ;
		JointSetupInfo.ConstrainPosition2.y = PmdPJoint->ConstrainPosition2[ 1 ] ;
		JointSetupInfo.ConstrainPosition2.z = PmdPJoint->ConstrainPosition2[ 2 ] ;
		JointSetupInfo.ConstrainRotation1.x = PmdPJoint->ConstrainRotation1[ 0 ] ;
		JointSetupInfo.ConstrainRotation1.y = PmdPJoint->ConstrainRotation1[ 1 ] ;
		JointSetupInfo.ConstrainRotation1.z = PmdPJoint->ConstrainRotation1[ 2 ] ;
		JointSetupInfo.ConstrainRotation2.x = PmdPJoint->ConstrainRotation2[ 0 ] ;
		JointSetupInfo.ConstrainRotation2.y = PmdPJoint->ConstrainRotation2[ 1 ] ;
		JointSetupInfo.ConstrainRotation2.z = PmdPJoint->ConstrainRotation2[ 2 ] ;
		JointSetupInfo.SpringPosition.x = PmdPJoint->SpringPosition[ 0 ] ;
		JointSetupInfo.SpringPosition.y = PmdPJoint->SpringPosition[ 1 ] ;
		JointSetupInfo.SpringPosition.z = PmdPJoint->SpringPosition[ 2 ] ;
		JointSetupInfo.SpringRotation.x = PmdPJoint->SpringRotation[ 0 ] * DX_PI_F / 180.0f ;
		JointSetupInfo.SpringRotation.y = PmdPJoint->SpringRotation[ 1 ] * DX_PI_F / 180.0f ;
		JointSetupInfo.SpringRotation.z = PmdPJoint->SpringRotation[ 2 ] * DX_PI_F / 180.0f ;
		BulletPhysics_SetupJoint( Bullet, BulletJointInfo, &JointSetupInfo ) ;
	}

	// 終了
	return 0 ;
}

extern int ReleasePhysicsObject_PMDPhysicsInfo(
	DX_MODELLOADER3_PMD_PHYSICS_INFO *MLPhysicsInfo
)
{
	int i ;
	BULLET_RIGIDBODY_INFO *BulletRigidBodyInfo ;
	BULLET_JOINT_INFO *BulletJointInfo ;
	PMD_READ_PHYSICS_INFO *PhysicsInfo ;
	PMD_READ_PHYSICS_JOINT_INFO *JointInfo ;
	BULLET_PHYSICS *Bullet = ( BULLET_PHYSICS * )MLPhysicsInfo->BulletPhysicsDataBuffer ;

	// 剛体オブジェクトの後始末
	PhysicsInfo = MLPhysicsInfo->PmdPhysicsInfoDim ;
	for( i = 0 ; i < MLPhysicsInfo->PmdPhysicsNum ; i ++, PhysicsInfo ++ )
	{
		BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )PhysicsInfo->BulletInfo ;

		BulletPhysics_ReleaseRigidBody( BulletRigidBodyInfo ) ;
	}

	// ジョイントオブジェクトの後始末
	JointInfo = MLPhysicsInfo->PmdPhysicsJointInfoDim ;
	for( i = 0 ; i < MLPhysicsInfo->PmdPhysicsJointNum ; i ++, JointInfo ++ )
	{
		BulletJointInfo = ( BULLET_JOINT_INFO * )JointInfo->BulletInfo ;

		BulletPhysics_ReleaseJoint( BulletJointInfo ) ;
	}

	// BulletPhysics の後始末
	BulletPhysics_Terminate( Bullet ) ;

	// 終了
	return 0 ;
}

extern int OneFrameProcess_PMDPhysicsInfo(
	DX_MODELLOADER3_PMD_PHYSICS_INFO *MLPhysicsInfo,
	int FrameNo,
	int LoopNo,
	bool FPS60,
	int ValidNextRate,
	int TimeDivNum
)
{
	BULLET_RIGIDBODY_INFO *BulletRigidBodyInfo ;
	BULLET_PHYSICS *Bullet = ( BULLET_PHYSICS * )MLPhysicsInfo->BulletPhysicsDataBuffer ;
	PMD_READ_PHYSICS_INFO *PhysicsInfo ;
	float UnitTime ;
	int j ;
	int k ;

	UnitTime = ( 1 / 60.0f ) / ( float )TimeDivNum ;

	// 最初のフレームで初期位置にセット
	if( FrameNo == 0 && LoopNo == 0 )
	{
		PhysicsInfo = MLPhysicsInfo->PmdPhysicsInfoDim ;
		for( j = 0 ; j < MLPhysicsInfo->PmdPhysicsNum ; j ++, PhysicsInfo ++ )
		{
			BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )PhysicsInfo->BulletInfo ;

			if( BulletRigidBodyInfo->DisableFlag )
			{
				continue ;
			}

			BulleyPhysics_ResetRigidBody( BulletRigidBodyInfo, &PhysicsInfo->Bone->LocalWorldMatrix ) ;

			PMD_PhysicsMotionState_Flush( false, BulletRigidBodyInfo->btMotionState, PhysicsInfo ) ;
		}

		// 最初のフレームは状態で落ち着かせるために３秒分回す
		{
			int   LoopNum ;

			LoopNum  = 180 * TimeDivNum ;
			for( j = 0 ; j < LoopNum ; j ++ )
			{
				Bullet->World->stepSimulation( UnitTime, 1, UnitTime ) ;

				// ボーン位置あわせ
				PhysicsInfo = MLPhysicsInfo->PmdPhysicsInfoDim ;
				for( k = 0 ; k < MLPhysicsInfo->PmdPhysicsNum ; k ++, PhysicsInfo ++ )
				{
					BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )PhysicsInfo->BulletInfo ;

					if( BulletRigidBodyInfo->DisableFlag )
					{
						continue ;
					}

					PMD_PhysicsMotionState_Flush( false, BulletRigidBodyInfo->btMotionState, PhysicsInfo ) ;
				}
			}
		}
	}
	else
	{
		// 物理処理を１フレーム分実行する
		Bullet->World->stepSimulation( UnitTime, 1, UnitTime ) ;

		// ボーン位置あわせ
		PhysicsInfo = MLPhysicsInfo->PmdPhysicsInfoDim ;
		for( j = 0 ; j < MLPhysicsInfo->PmdPhysicsNum ; j ++, PhysicsInfo ++ )
		{
			BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )PhysicsInfo->BulletInfo ;

			if( BulletRigidBodyInfo->DisableFlag )
			{
				continue ;
			}

			PMD_PhysicsMotionState_Flush( false, BulletRigidBodyInfo->btMotionState, PhysicsInfo ) ;
//			BulletRigidBodyInfo->btMotionState->Flush( false ) ;					
		}
	}

	// 現在の剛体の状態を表す行列を取得する
	PhysicsInfo = MLPhysicsInfo->PmdPhysicsInfoDim ;
	for( j = 0 ; j < MLPhysicsInfo->PmdPhysicsNum ; j ++, PhysicsInfo ++ )
	{
		if( PhysicsInfo->RigidBodyType == 0 || PhysicsInfo->NoCopyToBone ) continue ;

		BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )PhysicsInfo->BulletInfo ;

		if( BulletRigidBodyInfo->DisableFlag )
		{
			continue ;
		}

		PhysicsInfo->Bone->BackupLocalWorldMatrix = PhysicsInfo->Bone->LocalWorldMatrix ;
		PhysicsInfo->Bone->BackupLocalWorldMatrix_Valid = TRUE ;

		( BulletRigidBodyInfo->btRigdBody->getCenterOfMassTransform() * BulletRigidBodyInfo->bttrInvBoneOffset ).getOpenGLMatrix( ( float * )&PhysicsInfo->Bone->LocalWorldMatrix );
	}

	// 物理演算を行うボーンの行列キーを算出する
	if( MLPhysicsInfo->LoopMotionFlag == FALSE || LoopNo >= 2 )
	{
		if( ValidNextRate == FALSE && ( FPS60 || ( FPS60 == false && FrameNo % 2 == 0 ) ) )
		{
			int DestIndex ;
			MATRIX TempMatrix ;
//			float MulParam = 1.0f ;
			MATRIX *DestMatrix ;

			DestIndex = FPS60 ? FrameNo : FrameNo / 2 ;
//			if( MLPhysicsInfo->LoopMotionFlag == TRUE )
//			{
//				MulParam = 1.0f / ( MLPhysicsInfo->LoopMotionNum - 1 ) ;
//			}

			PhysicsInfo = MLPhysicsInfo->PmdPhysicsInfoDim ;
			for( j = 0 ; j < MLPhysicsInfo->PmdPhysicsNum ; j ++, PhysicsInfo ++ )
			{
				BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )PhysicsInfo->BulletInfo ;

				if( BulletRigidBodyInfo->DisableFlag )
				{
					continue ;
				}

				if( PhysicsInfo->RigidBodyType == 0 || PhysicsInfo->NoCopyToBone ) continue ;

				DestMatrix = &PhysicsInfo->Bone->KeyMatrix[ DestIndex ] ;

				if( PhysicsInfo->Bone->Frame->Parent )
				{
					MATRIX InvParentBoneLWM ;

					// 物理演算から得られる行列はワールド行列なので、それをボーンのローカル行列に変換する
					ModelLoader3_InverseMatrix( ( ( PMD_READ_BONE_INFO * )PhysicsInfo->Bone->Frame->Parent->UserData )->LocalWorldMatrix, InvParentBoneLWM ) ;
					CreateMultiplyMatrix( &TempMatrix, &PhysicsInfo->Bone->LocalWorldMatrix, &InvParentBoneLWM ) ;
				}
				else
				{
					TempMatrix = PhysicsInfo->Bone->LocalWorldMatrix ;
				}

				// 物理無効が有効になっていた場合は通常の計算行列とブレンドする
				if( PhysicsInfo->Bone->DisablePhysics >= 0.0001f )
				{
					MATRIX TempMatrix2 ;
					float DisableRate = PhysicsInfo->Bone->DisablePhysics ;

					if( PhysicsInfo->Bone->Frame->Parent )
					{
						MATRIX InvParentBoneLWM ;
						PMD_READ_BONE_INFO *ParentBoneInfo ;
						MATRIX *ParentLocalWorldMatrix ;
						MATRIX *LocalWorldMatrix ;

						ParentBoneInfo = ( PMD_READ_BONE_INFO * )PhysicsInfo->Bone->Frame->Parent->UserData ;
						ParentLocalWorldMatrix = ParentBoneInfo->BackupLocalWorldMatrix_Valid ? &ParentBoneInfo->BackupLocalWorldMatrix : &ParentBoneInfo->LocalWorldMatrix ;

						LocalWorldMatrix = PhysicsInfo->Bone->BackupLocalWorldMatrix_Valid ? &PhysicsInfo->Bone->BackupLocalWorldMatrix : &PhysicsInfo->Bone->LocalWorldMatrix ;

						// 物理演算から得られる行列はワールド行列なので、それをボーンのローカル行列に変換する
						ModelLoader3_InverseMatrix( *ParentLocalWorldMatrix, InvParentBoneLWM ) ;
						CreateMultiplyMatrix( &TempMatrix2, LocalWorldMatrix, &InvParentBoneLWM ) ;
					}
					else
					{
						TempMatrix2 = PhysicsInfo->Bone->BackupLocalWorldMatrix_Valid ? PhysicsInfo->Bone->BackupLocalWorldMatrix : PhysicsInfo->Bone->LocalWorldMatrix ;
					}

					TempMatrix.m[ 0 ][ 0 ] += ( TempMatrix2.m[ 0 ][ 0 ] - TempMatrix.m[ 0 ][ 0 ] ) * DisableRate ;
					TempMatrix.m[ 0 ][ 1 ] += ( TempMatrix2.m[ 0 ][ 1 ] - TempMatrix.m[ 0 ][ 1 ] ) * DisableRate ;
					TempMatrix.m[ 0 ][ 2 ] += ( TempMatrix2.m[ 0 ][ 2 ] - TempMatrix.m[ 0 ][ 2 ] ) * DisableRate ;
					TempMatrix.m[ 0 ][ 3 ] += ( TempMatrix2.m[ 0 ][ 3 ] - TempMatrix.m[ 0 ][ 3 ] ) * DisableRate ;

					TempMatrix.m[ 1 ][ 0 ] += ( TempMatrix2.m[ 1 ][ 0 ] - TempMatrix.m[ 1 ][ 0 ] ) * DisableRate ;
					TempMatrix.m[ 1 ][ 1 ] += ( TempMatrix2.m[ 1 ][ 1 ] - TempMatrix.m[ 1 ][ 1 ] ) * DisableRate ;
					TempMatrix.m[ 1 ][ 2 ] += ( TempMatrix2.m[ 1 ][ 2 ] - TempMatrix.m[ 1 ][ 2 ] ) * DisableRate ;
					TempMatrix.m[ 1 ][ 3 ] += ( TempMatrix2.m[ 1 ][ 3 ] - TempMatrix.m[ 1 ][ 3 ] ) * DisableRate ;

					TempMatrix.m[ 2 ][ 0 ] += ( TempMatrix2.m[ 2 ][ 0 ] - TempMatrix.m[ 2 ][ 0 ] ) * DisableRate ;
					TempMatrix.m[ 2 ][ 1 ] += ( TempMatrix2.m[ 2 ][ 1 ] - TempMatrix.m[ 2 ][ 1 ] ) * DisableRate ;
					TempMatrix.m[ 2 ][ 2 ] += ( TempMatrix2.m[ 2 ][ 2 ] - TempMatrix.m[ 2 ][ 2 ] ) * DisableRate ;
					TempMatrix.m[ 2 ][ 3 ] += ( TempMatrix2.m[ 2 ][ 3 ] - TempMatrix.m[ 2 ][ 3 ] ) * DisableRate ;

					TempMatrix.m[ 3 ][ 0 ] += ( TempMatrix2.m[ 3 ][ 0 ] - TempMatrix.m[ 3 ][ 0 ] ) * DisableRate ;
					TempMatrix.m[ 3 ][ 1 ] += ( TempMatrix2.m[ 3 ][ 1 ] - TempMatrix.m[ 3 ][ 1 ] ) * DisableRate ;
					TempMatrix.m[ 3 ][ 2 ] += ( TempMatrix2.m[ 3 ][ 2 ] - TempMatrix.m[ 3 ][ 2 ] ) * DisableRate ;
					TempMatrix.m[ 3 ][ 3 ] += ( TempMatrix2.m[ 3 ][ 3 ] - TempMatrix.m[ 3 ][ 3 ] ) * DisableRate ;
				}

				if( MLPhysicsInfo->LoopMotionFlag == FALSE )
				{
					*DestMatrix = TempMatrix ;
				}
				else
				{
					if( MLPhysicsInfo->MotionTotalFrameNum >= 6 && FrameNo > MLPhysicsInfo->MotionTotalFrameNum - 6 )
					{
						float Rate ;
						MATRIX *FirstMatrix ;

						Rate = ( float )( FrameNo - ( MLPhysicsInfo->MotionTotalFrameNum - 6 ) ) / 6.0f ;
						FirstMatrix = &PhysicsInfo->Bone->KeyMatrix[ 0 ] ;

						DestMatrix->m[ 0 ][ 0 ] = ( FirstMatrix->m[ 0 ][ 0 ] - TempMatrix.m[ 0 ][ 0 ] ) * Rate + TempMatrix.m[ 0 ][ 0 ] ;
						DestMatrix->m[ 0 ][ 1 ] = ( FirstMatrix->m[ 0 ][ 1 ] - TempMatrix.m[ 0 ][ 1 ] ) * Rate + TempMatrix.m[ 0 ][ 1 ] ;
						DestMatrix->m[ 0 ][ 2 ] = ( FirstMatrix->m[ 0 ][ 2 ] - TempMatrix.m[ 0 ][ 2 ] ) * Rate + TempMatrix.m[ 0 ][ 2 ] ;
						DestMatrix->m[ 0 ][ 3 ] = ( FirstMatrix->m[ 0 ][ 3 ] - TempMatrix.m[ 0 ][ 3 ] ) * Rate + TempMatrix.m[ 0 ][ 3 ] ;
						DestMatrix->m[ 1 ][ 0 ] = ( FirstMatrix->m[ 1 ][ 0 ] - TempMatrix.m[ 1 ][ 0 ] ) * Rate + TempMatrix.m[ 1 ][ 0 ] ;
						DestMatrix->m[ 1 ][ 1 ] = ( FirstMatrix->m[ 1 ][ 1 ] - TempMatrix.m[ 1 ][ 1 ] ) * Rate + TempMatrix.m[ 1 ][ 1 ] ;
						DestMatrix->m[ 1 ][ 2 ] = ( FirstMatrix->m[ 1 ][ 2 ] - TempMatrix.m[ 1 ][ 2 ] ) * Rate + TempMatrix.m[ 1 ][ 2 ] ;
						DestMatrix->m[ 1 ][ 3 ] = ( FirstMatrix->m[ 1 ][ 3 ] - TempMatrix.m[ 1 ][ 3 ] ) * Rate + TempMatrix.m[ 1 ][ 3 ] ;
						DestMatrix->m[ 2 ][ 0 ] = ( FirstMatrix->m[ 2 ][ 0 ] - TempMatrix.m[ 2 ][ 0 ] ) * Rate + TempMatrix.m[ 2 ][ 0 ] ;
						DestMatrix->m[ 2 ][ 1 ] = ( FirstMatrix->m[ 2 ][ 1 ] - TempMatrix.m[ 2 ][ 1 ] ) * Rate + TempMatrix.m[ 2 ][ 1 ] ;
						DestMatrix->m[ 2 ][ 2 ] = ( FirstMatrix->m[ 2 ][ 2 ] - TempMatrix.m[ 2 ][ 2 ] ) * Rate + TempMatrix.m[ 2 ][ 2 ] ;
						DestMatrix->m[ 2 ][ 3 ] = ( FirstMatrix->m[ 2 ][ 3 ] - TempMatrix.m[ 2 ][ 3 ] ) * Rate + TempMatrix.m[ 2 ][ 3 ] ;
						DestMatrix->m[ 3 ][ 0 ] = ( FirstMatrix->m[ 3 ][ 0 ] - TempMatrix.m[ 3 ][ 0 ] ) * Rate + TempMatrix.m[ 3 ][ 0 ] ;
						DestMatrix->m[ 3 ][ 1 ] = ( FirstMatrix->m[ 3 ][ 1 ] - TempMatrix.m[ 3 ][ 1 ] ) * Rate + TempMatrix.m[ 3 ][ 1 ] ;
						DestMatrix->m[ 3 ][ 2 ] = ( FirstMatrix->m[ 3 ][ 2 ] - TempMatrix.m[ 3 ][ 2 ] ) * Rate + TempMatrix.m[ 3 ][ 2 ] ;
						DestMatrix->m[ 3 ][ 3 ] = ( FirstMatrix->m[ 3 ][ 3 ] - TempMatrix.m[ 3 ][ 3 ] ) * Rate + TempMatrix.m[ 3 ][ 3 ] ;
					}
					else
					{
						*DestMatrix = TempMatrix ;
					}
				}
			}
		}
	}

	// 終了
	return 0 ;
}

extern int CheckDisablePhysicsAnim_PMDPhysicsInfo(
	DX_MODELLOADER3_PMD_PHYSICS_INFO *MLPhysicsInfo,
	int PhysicsIndex
)
{
	PMD_READ_PHYSICS_INFO *PhysicsInfo = &MLPhysicsInfo->PmdPhysicsInfoDim[ PhysicsIndex ] ;
	BULLET_RIGIDBODY_INFO *BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )PhysicsInfo->BulletInfo ;

	return BulletRigidBodyInfo->DisableFlag || PhysicsInfo->RigidBodyType == 0 || PhysicsInfo->NoCopyToBone ;
}










extern int SetupPhysicsObject_PMXPhysicsInfo(
	DX_MODELLOADER3_PMX_PHYSICS_INFO *MLPhysicsInfo,
	int DisablePhysicsFile
)
{
	int i ;
	PMX_READ_PHYSICS_INFO *PhysicsInfo ;
	PMX_READ_PHYSICS_JOINT_INFO *JointInfo ;
	BULLET_RIGIDBODY_INFO *BulletRigidBodyInfo ;
	BULLET_JOINT_INFO *BulletJointInfo ;
	PMX_JOINT *PmxPJoint ;
	BULLET_PHYSICS *Bullet = ( BULLET_PHYSICS * )MLPhysicsInfo->BulletPhysicsDataBuffer ;
	BULLET_RIGIDBODY_SETUP_INFO RigidBodySetupInfo ;
	BULLET_JOINT_SETUP_INFO JointSetupInfo ;

	// BulletPhysics の初期化
	BulletPhysics_Initialize( Bullet, MLPhysicsInfo->Gravity ) ;

	// 剛体オブジェクトのセットアップ
	PhysicsInfo = MLPhysicsInfo->PmxPhysicsInfoDim ;
	for( i = 0 ; i < MLPhysicsInfo->PmxPhysicsNum ; i ++, PhysicsInfo ++ )
	{
		BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )PhysicsInfo->BulletInfo ;

		// 無効化チェック
		BulletRigidBodyInfo->DisableFlag = MV1LoadModelDisablePhysicsNameCheck_WCHAR_T( ( WORD * )PhysicsInfo->Base->Name, DisablePhysicsFile ) ;

		// 無効の場合は初期化処理を行わない
		if( BulletRigidBodyInfo->DisableFlag )
		{
			continue ;
		}

		// 剛体の初期化
		RigidBodySetupInfo.RigidBodyGroupIndex = PhysicsInfo->Base->RigidBodyGroupIndex ;
		RigidBodySetupInfo.RigidBodyGroupTarget = PhysicsInfo->Base->RigidBodyGroupTarget ;
		RigidBodySetupInfo.ShapeType = PhysicsInfo->Base->ShapeType ;
		RigidBodySetupInfo.ShapeW = PhysicsInfo->Base->ShapeW ;
		RigidBodySetupInfo.ShapeH = PhysicsInfo->Base->ShapeH ;
		RigidBodySetupInfo.ShapeD = PhysicsInfo->Base->ShapeD ;
		RigidBodySetupInfo.Position.x = PhysicsInfo->Base->Position[ 0 ] - PhysicsInfo->Bone->LocalWorldMatrix.m[ 3 ][ 0 ] ;
		RigidBodySetupInfo.Position.y = PhysicsInfo->Base->Position[ 1 ] - PhysicsInfo->Bone->LocalWorldMatrix.m[ 3 ][ 1 ] ;
		RigidBodySetupInfo.Position.z = PhysicsInfo->Base->Position[ 2 ] - PhysicsInfo->Bone->LocalWorldMatrix.m[ 3 ][ 2 ] ;
		RigidBodySetupInfo.Rotation.x = PhysicsInfo->Base->Rotation[ 0 ] ;
		RigidBodySetupInfo.Rotation.y = PhysicsInfo->Base->Rotation[ 1 ] ;
		RigidBodySetupInfo.Rotation.z = PhysicsInfo->Base->Rotation[ 2 ] ;
		RigidBodySetupInfo.RigidBodyWeight = PhysicsInfo->Base->RigidBodyWeight ;
		RigidBodySetupInfo.RigidBodyPosDim = PhysicsInfo->Base->RigidBodyPosDim ;
		RigidBodySetupInfo.RigidBodyRotDim = PhysicsInfo->Base->RigidBodyRotDim ;
		RigidBodySetupInfo.RigidBodyRecoil = PhysicsInfo->Base->RigidBodyRecoil ;
		RigidBodySetupInfo.RigidBodyFriction = PhysicsInfo->Base->RigidBodyFriction ;
		RigidBodySetupInfo.RigidBodyType = PhysicsInfo->Base->RigidBodyType ;
		RigidBodySetupInfo.InitializeMatrix = &PhysicsInfo->Bone->LocalWorldMatrix ;
		BulletPhysics_SetupRigidBody( Bullet, BulletRigidBodyInfo, &RigidBodySetupInfo ) ;
	}

	// ジョイントオブジェクトのセットアップ
	JointInfo = MLPhysicsInfo->PmxPhysicsJointInfoDim ;
	for( i = 0 ; i < MLPhysicsInfo->PmxPhysicsJointNum ; i ++, JointInfo ++ )
	{
		BULLET_RIGIDBODY_INFO *BulletRigidBodyInfoA ;
		BULLET_RIGIDBODY_INFO *BulletRigidBodyInfoB ;

		BulletJointInfo = ( BULLET_JOINT_INFO * )JointInfo->BulletInfo ;

		PmxPJoint = JointInfo->Base ;

		// どちらかの剛体が無効かチェック
		BulletRigidBodyInfoA = ( BULLET_RIGIDBODY_INFO * )MLPhysicsInfo->PmxPhysicsInfoDim[ PmxPJoint->RigidBodyAIndex ].BulletInfo ;
		BulletRigidBodyInfoB = ( BULLET_RIGIDBODY_INFO * )MLPhysicsInfo->PmxPhysicsInfoDim[ PmxPJoint->RigidBodyBIndex ].BulletInfo ;
		if( BulletRigidBodyInfoA->DisableFlag ||
			BulletRigidBodyInfoB->DisableFlag )
		{
			BulletJointInfo->DisableFlag = TRUE ;
		}
		else
		{
			BulletJointInfo->DisableFlag = FALSE ;
		}

		// 無効の場合は初期化処理を行わない
		if( BulletJointInfo->DisableFlag )
		{
			continue ;
		}

		// ジョイントの初期化
		JointSetupInfo.RigidBodyA = BulletRigidBodyInfoA->btRigdBody ;
		JointSetupInfo.RigidBodyB = BulletRigidBodyInfoB->btRigdBody ;
		JointSetupInfo.Position.x = PmxPJoint->Position[ 0 ] ;
		JointSetupInfo.Position.y = PmxPJoint->Position[ 1 ] ;
		JointSetupInfo.Position.z = PmxPJoint->Position[ 2 ] ;
		JointSetupInfo.Rotation.x = PmxPJoint->Rotation[ 0 ] ;
		JointSetupInfo.Rotation.y = PmxPJoint->Rotation[ 1 ] ;
		JointSetupInfo.Rotation.z = PmxPJoint->Rotation[ 2 ] ;
		JointSetupInfo.ConstrainPosition1.x = PmxPJoint->ConstrainPositionMin[ 0 ] ;
		JointSetupInfo.ConstrainPosition1.y = PmxPJoint->ConstrainPositionMin[ 1 ] ;
		JointSetupInfo.ConstrainPosition1.z = PmxPJoint->ConstrainPositionMin[ 2 ] ;
		JointSetupInfo.ConstrainPosition2.x = PmxPJoint->ConstrainPositionMax[ 0 ] ;
		JointSetupInfo.ConstrainPosition2.y = PmxPJoint->ConstrainPositionMax[ 1 ] ;
		JointSetupInfo.ConstrainPosition2.z = PmxPJoint->ConstrainPositionMax[ 2 ] ;
		JointSetupInfo.ConstrainRotation1.x = PmxPJoint->ConstrainRotationMin[ 0 ] ;
		JointSetupInfo.ConstrainRotation1.y = PmxPJoint->ConstrainRotationMin[ 1 ] ;
		JointSetupInfo.ConstrainRotation1.z = PmxPJoint->ConstrainRotationMin[ 2 ] ;
		JointSetupInfo.ConstrainRotation2.x = PmxPJoint->ConstrainRotationMax[ 0 ] ;
		JointSetupInfo.ConstrainRotation2.y = PmxPJoint->ConstrainRotationMax[ 1 ] ;
		JointSetupInfo.ConstrainRotation2.z = PmxPJoint->ConstrainRotationMax[ 2 ] ;
		JointSetupInfo.SpringPosition.x = PmxPJoint->SpringPosition[ 0 ] ;
		JointSetupInfo.SpringPosition.y = PmxPJoint->SpringPosition[ 1 ] ;
		JointSetupInfo.SpringPosition.z = PmxPJoint->SpringPosition[ 2 ] ;
		JointSetupInfo.SpringRotation.x = PmxPJoint->SpringRotation[ 0 ] * DX_PI_F / 180.0f ;
		JointSetupInfo.SpringRotation.y = PmxPJoint->SpringRotation[ 1 ] * DX_PI_F / 180.0f ;
		JointSetupInfo.SpringRotation.z = PmxPJoint->SpringRotation[ 2 ] * DX_PI_F / 180.0f ;
		BulletPhysics_SetupJoint( Bullet, BulletJointInfo, &JointSetupInfo ) ;
	}

	// 終了
	return 0 ;
}

extern int ReleasePhysicsObject_PMXPhysicsInfo(
	DX_MODELLOADER3_PMX_PHYSICS_INFO *MLPhysicsInfo
)
{
	int i ;
	BULLET_RIGIDBODY_INFO *BulletRigidBodyInfo ;
	BULLET_JOINT_INFO *BulletJointInfo ;
	PMX_READ_PHYSICS_INFO *PhysicsInfo ;
	PMX_READ_PHYSICS_JOINT_INFO *JointInfo ;
	BULLET_PHYSICS *Bullet = ( BULLET_PHYSICS * )MLPhysicsInfo->BulletPhysicsDataBuffer ;

	// 剛体オブジェクトの後始末
	PhysicsInfo = MLPhysicsInfo->PmxPhysicsInfoDim ;
	for( i = 0 ; i < MLPhysicsInfo->PmxPhysicsNum ; i ++, PhysicsInfo ++ )
	{
		BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )PhysicsInfo->BulletInfo ;

		BulletPhysics_ReleaseRigidBody( BulletRigidBodyInfo ) ;
	}

	// ジョイントオブジェクトの後始末
	JointInfo = MLPhysicsInfo->PmxPhysicsJointInfoDim ;
	for( i = 0 ; i < MLPhysicsInfo->PmxPhysicsJointNum ; i ++, JointInfo ++ )
	{
		BulletJointInfo = ( BULLET_JOINT_INFO * )JointInfo->BulletInfo ;

		BulletPhysics_ReleaseJoint( BulletJointInfo ) ;
	}

	// BulletPhysics の後始末
	BulletPhysics_Terminate( Bullet ) ;

	// 終了
	return 0 ;
}

extern int OneFrameProcess_PMXPhysicsInfo(
	DX_MODELLOADER3_PMX_PHYSICS_INFO *MLPhysicsInfo,
	int FrameNo,
	int LoopNo,
	bool FPS60,
	int ValidNextRate,
	int TimeDivNum
)
{
	BULLET_RIGIDBODY_INFO *BulletRigidBodyInfo ;
	BULLET_PHYSICS *Bullet = ( BULLET_PHYSICS * )MLPhysicsInfo->BulletPhysicsDataBuffer ;
	PMX_READ_PHYSICS_INFO *PhysicsInfo ;
	float UnitTime ;
	int j ;
	int k ;

	UnitTime = ( 1 / 60.0f ) / ( float )TimeDivNum ;

	// 最初のフレームで初期位置にセット
	if( FrameNo == 0 && LoopNo == 0 )
	{
		PhysicsInfo = MLPhysicsInfo->PmxPhysicsInfoDim ;
		for( j = 0 ; j < MLPhysicsInfo->PmxPhysicsNum ; j ++, PhysicsInfo ++ )
		{
			BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )PhysicsInfo->BulletInfo ;

			if( BulletRigidBodyInfo->DisableFlag )
			{
				continue ;
			}

			BulleyPhysics_ResetRigidBody( BulletRigidBodyInfo, &PhysicsInfo->Bone->LocalWorldMatrix ) ;

			PMX_PhysicsMotionState_Flush( false, BulletRigidBodyInfo->btMotionState, PhysicsInfo ) ;
		}

		// 最初のフレームは状態で落ち着かせるために３秒分回す
		{
			int   LoopNum ;

			LoopNum  = 180 * TimeDivNum ;
			for( j = 0 ; j < LoopNum ; j ++ )
			{
				Bullet->World->stepSimulation( UnitTime, 1, UnitTime ) ;

				// ボーン位置あわせ
				PhysicsInfo = MLPhysicsInfo->PmxPhysicsInfoDim ;
				for( k = 0 ; k < MLPhysicsInfo->PmxPhysicsNum ; k ++, PhysicsInfo ++ )
				{
					BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )PhysicsInfo->BulletInfo ;

					if( BulletRigidBodyInfo->DisableFlag )
					{
						continue ;
					}

					PMX_PhysicsMotionState_Flush( false, BulletRigidBodyInfo->btMotionState, PhysicsInfo ) ;
				}
			}
		}
	}
	else
	{
		// 物理処理を１フレーム分実行する
		Bullet->World->stepSimulation( UnitTime, 1, UnitTime ) ;

		// ボーン位置あわせ
		PhysicsInfo = MLPhysicsInfo->PmxPhysicsInfoDim ;
		for( j = 0 ; j < MLPhysicsInfo->PmxPhysicsNum ; j ++, PhysicsInfo ++ )
		{
			BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )PhysicsInfo->BulletInfo ;

			if( BulletRigidBodyInfo->DisableFlag )
			{
				continue ;
			}

			PMX_PhysicsMotionState_Flush( false, BulletRigidBodyInfo->btMotionState, PhysicsInfo ) ;
//			BulletRigidBodyInfo->btMotionState->Flush( false ) ;					
		}
	}

	// 現在の剛体の状態を表す行列を取得する
	PhysicsInfo = MLPhysicsInfo->PmxPhysicsInfoDim ;
	for( j = 0 ; j < MLPhysicsInfo->PmxPhysicsNum ; j ++, PhysicsInfo ++ )
	{
		if( PhysicsInfo->Base->RigidBodyType == 0 || PhysicsInfo->NoCopyToBone ) continue ;

		BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )PhysicsInfo->BulletInfo ;

		if( BulletRigidBodyInfo->DisableFlag )
		{
			continue ;
		}

		PhysicsInfo->Bone->BackupLocalWorldMatrix = PhysicsInfo->Bone->LocalWorldMatrix ;
		PhysicsInfo->Bone->BackupLocalWorldMatrix_Valid = TRUE ;

		( BulletRigidBodyInfo->btRigdBody->getCenterOfMassTransform() * BulletRigidBodyInfo->bttrInvBoneOffset ).getOpenGLMatrix( ( float * )&PhysicsInfo->Bone->LocalWorldMatrix );
	}

	// 物理演算を行うボーンの行列キーを算出する
	if( MLPhysicsInfo->LoopMotionFlag == FALSE || LoopNo >= 2 )
	{
		if( ValidNextRate == FALSE && ( FPS60 || ( FPS60 == false && FrameNo % 2 == 0 ) ) )
		{
			int DestIndex ;
			MATRIX TempMatrix ;
//			float MulParam = 1.0f ;
			MATRIX *DestMatrix ;

			DestIndex = FPS60 ? FrameNo : FrameNo / 2 ;
//			if( MLPhysicsInfo->LoopMotionFlag == TRUE )
//			{
//				MulParam = 1.0f / ( MLPhysicsInfo->LoopMotionNum - 1 ) ;
//			}

			PhysicsInfo = MLPhysicsInfo->PmxPhysicsInfoDim ;
			for( j = 0 ; j < MLPhysicsInfo->PmxPhysicsNum ; j ++, PhysicsInfo ++ )
			{
				BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )PhysicsInfo->BulletInfo ;

				if( BulletRigidBodyInfo->DisableFlag )
				{
					continue ;
				}

				if( PhysicsInfo->Base->RigidBodyType == 0 || PhysicsInfo->NoCopyToBone ) continue ;

				DestMatrix = &PhysicsInfo->Bone->KeyMatrix[ DestIndex ] ;

				if( PhysicsInfo->Bone->Frame->Parent )
				{
					MATRIX InvParentBoneLWM ;

					// 物理演算から得られる行列はワールド行列なので、それをボーンのローカル行列に変換する
					ModelLoader3_InverseMatrix( ( ( PMX_READ_BONE_INFO * )PhysicsInfo->Bone->Frame->Parent->UserData )->LocalWorldMatrix, InvParentBoneLWM ) ;
					CreateMultiplyMatrix( &TempMatrix, &PhysicsInfo->Bone->LocalWorldMatrix, &InvParentBoneLWM ) ;
				}
				else
				{
					TempMatrix = PhysicsInfo->Bone->LocalWorldMatrix ;
				}

				// 物理無効が有効になっていた場合は通常の計算行列とブレンドする
				if( PhysicsInfo->Bone->DisablePhysics >= 0.0001f )
				{
					MATRIX TempMatrix2 ;
					float DisableRate = PhysicsInfo->Bone->DisablePhysics ;

					if( PhysicsInfo->Bone->Frame->Parent )
					{
						MATRIX InvParentBoneLWM ;
						PMX_READ_BONE_INFO *ParentBoneInfo ;
						MATRIX *ParentLocalWorldMatrix ;
						MATRIX *LocalWorldMatrix ;

						ParentBoneInfo = ( PMX_READ_BONE_INFO * )PhysicsInfo->Bone->Frame->Parent->UserData ;
						ParentLocalWorldMatrix = ParentBoneInfo->BackupLocalWorldMatrix_Valid ? &ParentBoneInfo->BackupLocalWorldMatrix : &ParentBoneInfo->LocalWorldMatrix ;

						LocalWorldMatrix = PhysicsInfo->Bone->BackupLocalWorldMatrix_Valid ? &PhysicsInfo->Bone->BackupLocalWorldMatrix : &PhysicsInfo->Bone->LocalWorldMatrix ;

						// 物理演算から得られる行列はワールド行列なので、それをボーンのローカル行列に変換する
						ModelLoader3_InverseMatrix( *ParentLocalWorldMatrix, InvParentBoneLWM ) ;
						CreateMultiplyMatrix( &TempMatrix2, LocalWorldMatrix, &InvParentBoneLWM ) ;
					}
					else
					{
						TempMatrix2 = PhysicsInfo->Bone->BackupLocalWorldMatrix_Valid ? PhysicsInfo->Bone->BackupLocalWorldMatrix : PhysicsInfo->Bone->LocalWorldMatrix ;
					}

					TempMatrix.m[ 0 ][ 0 ] += ( TempMatrix2.m[ 0 ][ 0 ] - TempMatrix.m[ 0 ][ 0 ] ) * DisableRate ;
					TempMatrix.m[ 0 ][ 1 ] += ( TempMatrix2.m[ 0 ][ 1 ] - TempMatrix.m[ 0 ][ 1 ] ) * DisableRate ;
					TempMatrix.m[ 0 ][ 2 ] += ( TempMatrix2.m[ 0 ][ 2 ] - TempMatrix.m[ 0 ][ 2 ] ) * DisableRate ;
					TempMatrix.m[ 0 ][ 3 ] += ( TempMatrix2.m[ 0 ][ 3 ] - TempMatrix.m[ 0 ][ 3 ] ) * DisableRate ;

					TempMatrix.m[ 1 ][ 0 ] += ( TempMatrix2.m[ 1 ][ 0 ] - TempMatrix.m[ 1 ][ 0 ] ) * DisableRate ;
					TempMatrix.m[ 1 ][ 1 ] += ( TempMatrix2.m[ 1 ][ 1 ] - TempMatrix.m[ 1 ][ 1 ] ) * DisableRate ;
					TempMatrix.m[ 1 ][ 2 ] += ( TempMatrix2.m[ 1 ][ 2 ] - TempMatrix.m[ 1 ][ 2 ] ) * DisableRate ;
					TempMatrix.m[ 1 ][ 3 ] += ( TempMatrix2.m[ 1 ][ 3 ] - TempMatrix.m[ 1 ][ 3 ] ) * DisableRate ;

					TempMatrix.m[ 2 ][ 0 ] += ( TempMatrix2.m[ 2 ][ 0 ] - TempMatrix.m[ 2 ][ 0 ] ) * DisableRate ;
					TempMatrix.m[ 2 ][ 1 ] += ( TempMatrix2.m[ 2 ][ 1 ] - TempMatrix.m[ 2 ][ 1 ] ) * DisableRate ;
					TempMatrix.m[ 2 ][ 2 ] += ( TempMatrix2.m[ 2 ][ 2 ] - TempMatrix.m[ 2 ][ 2 ] ) * DisableRate ;
					TempMatrix.m[ 2 ][ 3 ] += ( TempMatrix2.m[ 2 ][ 3 ] - TempMatrix.m[ 2 ][ 3 ] ) * DisableRate ;

					TempMatrix.m[ 3 ][ 0 ] += ( TempMatrix2.m[ 3 ][ 0 ] - TempMatrix.m[ 3 ][ 0 ] ) * DisableRate ;
					TempMatrix.m[ 3 ][ 1 ] += ( TempMatrix2.m[ 3 ][ 1 ] - TempMatrix.m[ 3 ][ 1 ] ) * DisableRate ;
					TempMatrix.m[ 3 ][ 2 ] += ( TempMatrix2.m[ 3 ][ 2 ] - TempMatrix.m[ 3 ][ 2 ] ) * DisableRate ;
					TempMatrix.m[ 3 ][ 3 ] += ( TempMatrix2.m[ 3 ][ 3 ] - TempMatrix.m[ 3 ][ 3 ] ) * DisableRate ;
				}

				if( MLPhysicsInfo->LoopMotionFlag == FALSE )
				{
					*DestMatrix = TempMatrix ;
				}
				else
				{
					if( MLPhysicsInfo->MotionTotalFrameNum >= 6 && FrameNo > MLPhysicsInfo->MotionTotalFrameNum - 6 )
					{
						float Rate ;
						MATRIX *FirstMatrix ;

						Rate = ( float )( FrameNo - ( MLPhysicsInfo->MotionTotalFrameNum - 6 ) ) / 6.0f ;
						FirstMatrix = &PhysicsInfo->Bone->KeyMatrix[ 0 ] ;

						DestMatrix->m[ 0 ][ 0 ] = ( FirstMatrix->m[ 0 ][ 0 ] - TempMatrix.m[ 0 ][ 0 ] ) * Rate + TempMatrix.m[ 0 ][ 0 ] ;
						DestMatrix->m[ 0 ][ 1 ] = ( FirstMatrix->m[ 0 ][ 1 ] - TempMatrix.m[ 0 ][ 1 ] ) * Rate + TempMatrix.m[ 0 ][ 1 ] ;
						DestMatrix->m[ 0 ][ 2 ] = ( FirstMatrix->m[ 0 ][ 2 ] - TempMatrix.m[ 0 ][ 2 ] ) * Rate + TempMatrix.m[ 0 ][ 2 ] ;
						DestMatrix->m[ 0 ][ 3 ] = ( FirstMatrix->m[ 0 ][ 3 ] - TempMatrix.m[ 0 ][ 3 ] ) * Rate + TempMatrix.m[ 0 ][ 3 ] ;
						DestMatrix->m[ 1 ][ 0 ] = ( FirstMatrix->m[ 1 ][ 0 ] - TempMatrix.m[ 1 ][ 0 ] ) * Rate + TempMatrix.m[ 1 ][ 0 ] ;
						DestMatrix->m[ 1 ][ 1 ] = ( FirstMatrix->m[ 1 ][ 1 ] - TempMatrix.m[ 1 ][ 1 ] ) * Rate + TempMatrix.m[ 1 ][ 1 ] ;
						DestMatrix->m[ 1 ][ 2 ] = ( FirstMatrix->m[ 1 ][ 2 ] - TempMatrix.m[ 1 ][ 2 ] ) * Rate + TempMatrix.m[ 1 ][ 2 ] ;
						DestMatrix->m[ 1 ][ 3 ] = ( FirstMatrix->m[ 1 ][ 3 ] - TempMatrix.m[ 1 ][ 3 ] ) * Rate + TempMatrix.m[ 1 ][ 3 ] ;
						DestMatrix->m[ 2 ][ 0 ] = ( FirstMatrix->m[ 2 ][ 0 ] - TempMatrix.m[ 2 ][ 0 ] ) * Rate + TempMatrix.m[ 2 ][ 0 ] ;
						DestMatrix->m[ 2 ][ 1 ] = ( FirstMatrix->m[ 2 ][ 1 ] - TempMatrix.m[ 2 ][ 1 ] ) * Rate + TempMatrix.m[ 2 ][ 1 ] ;
						DestMatrix->m[ 2 ][ 2 ] = ( FirstMatrix->m[ 2 ][ 2 ] - TempMatrix.m[ 2 ][ 2 ] ) * Rate + TempMatrix.m[ 2 ][ 2 ] ;
						DestMatrix->m[ 2 ][ 3 ] = ( FirstMatrix->m[ 2 ][ 3 ] - TempMatrix.m[ 2 ][ 3 ] ) * Rate + TempMatrix.m[ 2 ][ 3 ] ;
						DestMatrix->m[ 3 ][ 0 ] = ( FirstMatrix->m[ 3 ][ 0 ] - TempMatrix.m[ 3 ][ 0 ] ) * Rate + TempMatrix.m[ 3 ][ 0 ] ;
						DestMatrix->m[ 3 ][ 1 ] = ( FirstMatrix->m[ 3 ][ 1 ] - TempMatrix.m[ 3 ][ 1 ] ) * Rate + TempMatrix.m[ 3 ][ 1 ] ;
						DestMatrix->m[ 3 ][ 2 ] = ( FirstMatrix->m[ 3 ][ 2 ] - TempMatrix.m[ 3 ][ 2 ] ) * Rate + TempMatrix.m[ 3 ][ 2 ] ;
						DestMatrix->m[ 3 ][ 3 ] = ( FirstMatrix->m[ 3 ][ 3 ] - TempMatrix.m[ 3 ][ 3 ] ) * Rate + TempMatrix.m[ 3 ][ 3 ] ;
					}
					else
					{
						*DestMatrix = TempMatrix ;
					}

					/*
					DestMatrix->m[ 0 ][ 0 ] += TempMatrix.m[ 0 ][ 0 ] * MulParam ;
					DestMatrix->m[ 0 ][ 1 ] += TempMatrix.m[ 0 ][ 1 ] * MulParam ;
					DestMatrix->m[ 0 ][ 2 ] += TempMatrix.m[ 0 ][ 2 ] * MulParam ;
					DestMatrix->m[ 0 ][ 3 ] += TempMatrix.m[ 0 ][ 3 ] * MulParam ;
					DestMatrix->m[ 1 ][ 0 ] += TempMatrix.m[ 1 ][ 0 ] * MulParam ;
					DestMatrix->m[ 1 ][ 1 ] += TempMatrix.m[ 1 ][ 1 ] * MulParam ;
					DestMatrix->m[ 1 ][ 2 ] += TempMatrix.m[ 1 ][ 2 ] * MulParam ;
					DestMatrix->m[ 1 ][ 3 ] += TempMatrix.m[ 1 ][ 3 ] * MulParam ;
					DestMatrix->m[ 2 ][ 0 ] += TempMatrix.m[ 2 ][ 0 ] * MulParam ;
					DestMatrix->m[ 2 ][ 1 ] += TempMatrix.m[ 2 ][ 1 ] * MulParam ;
					DestMatrix->m[ 2 ][ 2 ] += TempMatrix.m[ 2 ][ 2 ] * MulParam ;
					DestMatrix->m[ 2 ][ 3 ] += TempMatrix.m[ 2 ][ 3 ] * MulParam ;
					DestMatrix->m[ 3 ][ 0 ] += TempMatrix.m[ 3 ][ 0 ] * MulParam ;
					DestMatrix->m[ 3 ][ 1 ] += TempMatrix.m[ 3 ][ 1 ] * MulParam ;
					DestMatrix->m[ 3 ][ 2 ] += TempMatrix.m[ 3 ][ 2 ] * MulParam ;
					DestMatrix->m[ 3 ][ 3 ] += TempMatrix.m[ 3 ][ 3 ] * MulParam ;
					*/
				}
/*
				if( PhysicsInfo->Bone->Frame->Parent )
				{
					MATRIX InvParentBoneLWM ;

					// 物理演算から得られる行列はワールド行列なので、それをボーンのローカル行列に変換する
					ModelLoader3_InverseMatrix( ( ( PMX_READ_BONE_INFO * )PhysicsInfo->Bone->Frame->Parent->UserData )->LocalWorldMatrix, InvParentBoneLWM ) ;
					CreateMultiplyMatrix( &PhysicsInfo->Bone->KeyMatrix[ DestIndex ], &PhysicsInfo->Bone->LocalWorldMatrix, &InvParentBoneLWM ) ;
				}
				else
				{
					PhysicsInfo->Bone->KeyMatrix[ DestIndex ] = PhysicsInfo->Bone->LocalWorldMatrix ;
				}
*/
			}
		}
	}

	// 終了
	return 0 ;
}

extern int CheckDisablePhysicsAnim_PMXPhysicsInfo(
	DX_MODELLOADER3_PMX_PHYSICS_INFO *MLPhysicsInfo,
	int PhysicsIndex
)
{
	PMX_READ_PHYSICS_INFO *PhysicsInfo = &MLPhysicsInfo->PmxPhysicsInfoDim[ PhysicsIndex ] ;
	BULLET_RIGIDBODY_INFO *BulletRigidBodyInfo = ( BULLET_RIGIDBODY_INFO * )PhysicsInfo->BulletInfo ;

	return BulletRigidBodyInfo->DisableFlag || PhysicsInfo->Base->RigidBodyType == 0 || PhysicsInfo->NoCopyToBone ;
}






//}

#endif // defined( DX_NON_MODEL ) && defined( DX_NON_BULLET_PHYSICS )





