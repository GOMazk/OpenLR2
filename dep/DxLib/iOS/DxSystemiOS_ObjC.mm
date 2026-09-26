// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		iOS用システムプログラム
// 
// 				Ver 3.25a
// 
// -------------------------------------------------------------------------------

// ＤＸライブラリ作成時用定義
#define DX_MAKE

// インクルード ------------------------------------------------------------------

#import <CoreText/CoreText.h>
#import <CoreMotion/CoreMotion.h>
#import <GameController/GameController.h>
#import <UIKit/UIKit.h>
#import <OpenAL/al.h>
#import <OpenAL/alc.h>
#import <QuartzCore/QuartzCore.h>
#import <AVFoundation/AVFoundation.h>
#import <AudioToolbox/AudioToolbox.h>
#import <MessageUI/MessageUI.h>
#import <MessageUI/MFMailComposeViewController.h>
#include "DxSystemiOS_ObjC.h"
#include "DxSystemiOS.h"
#include "DxGraphicsiOS.h"
#include "DxInputiOS.h"
#include "DxSoundiOS.h"
#include "../DxSound.h"
#include "../DxSystem.h"
#include "../DxInput.h"
#include "../DxMemory.h"
#include "../DxBaseFunc.h"

// マクロ定義 --------------------------------------------------------------------

#define MAX_TOUCH_NUM						(128)
#define TCHAR_TO_UTF8_TEMPSTRINGLENGTH		(1024)

// 構造体定義 --------------------------------------------------------------------

// フォント情報構造体
struct DXLIB_IOS_FONTINFO
{
	CGColorSpaceRef					ColorSpace ;
	CGContextRef					BitmapContext ;
	CGMutablePathRef				RectPath ;
	CTFontRef						Font ;
	float							FontAscent ;
	float							FontDescent ;
	float							FontLeading ;
	float							FontUnderlinePosition ;
	float							FontUnderlineThickness ;
	float							FontCapHeight ;
	float							FontXHeight ;
	void *  						ImageData ;
	int								ImageWidth ;
	int								ImagePitch ;
	int								ImageHeight ;
	int								ImageSizeX ;
	int								ImageSizeY ;
	int								ImageDrawX ;
	int								ImageDrawY ;
	int								ImageAddX ;
} ;

// スクリーンタッチ情報
struct DXLIB_IOS_TOUCH_INFO
{
	int								UseFlag ;
	int								TouchID ;
	float							PositionX ;
	float							PositionY ;
} ;

// Objective-C 用システム情報
struct DXLIB_IOS_SYSTEMINFO_OBJC
{
	UIViewController *				ViewController ;
	UIView *						DxLibGLView ;
	CAEAGLLayer *					AEAGLayer ;
	GLuint							ViewFrameBuffer ;
	GLuint							ViewRenderBuffer ;
	GLuint							ViewDepthBuffer ;
	int								ViewWidth ;
	int								ViewHeight ;

	ALCdevice *						ALCdeviceObject ;						// ALデバイスオブジェクト
	ALCcontext *					ALCcontectObject ;						// ALコンテキストオブジェクト

	CFMutableDictionaryRef			touchBeginPoints ;
	int								touchInfoNum ;
	struct DXLIB_IOS_TOUCH_INFO		touchInfos[ MAX_TOUCH_NUM ] ;
	int								NextTouchID ;

	NSArray *						GameControllerArray ;

	volatile int					RecordPermissionProcessStart ;
	volatile int					RecordPermissionProcessEnd ;

	volatile int					TextInputAlertRunRequestFlag ;
	volatile int					TextInputAlertRunFlag ;
	volatile int					TextInputAlertCancelFlag ;
	UIAlertView *					TextInputAlertView ;
	UIAlertController *				TextInputAlertController ;
	NSString *						TextInputAlertTitle ;
	NSString *						TextInputAlertMessage ;
	int								TextInputAlertInputButtonIndex ;
	TCHAR *							TextInputResultStringBuffer ;
	size_t							TextInputResultStringBufferBytes ;

	CMMotionManager *				MotionManager ;
	volatile DEVICEMOTIONINFO_IOS	MotionInfo ;

	CGFloat							Brightness ;
} ;


// 関数定義 ----------------------------------------------------------------------

static void UpdateTouchInputData( LONGLONG Time ) ;
static int SetupViewBuffer( void ) ;
static int TerminateViewBuffer( void ) ;

static NSString *ConvertTCHARtoNSString( const TCHAR *Source ) ;
static TCHAR *ConvertNSStringtoTCHAR( NSString *Source ) ;
static void ConvertNSStringtoTCHAR_NoAlloc( NSString *Source, TCHAR *DestBuffer, size_t DestBufferBytes ) ;

// クラス定義 --------------------------------------------------------------------

@interface DxLibAppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;

@end

@interface DxLibiCadeReaderView : UIView<UIKeyInput> 
{
    UIView* inputView ;
}

@property (nonatomic, assign) BOOL active;

- ( void )didEnterBackground ;
- ( void )didBecomeActive ;

@end

@interface DxLibGLView : UIView
{
    EAGLContext* mpGLContext ;
    
    GLuint mFrameBuffer ;
    GLuint mColorBuffer ;
}

- ( void )disableGesture ;
+ ( void )dummyFunction ;

@end

@interface DxLibViewController : UIViewController <MFMailComposeViewControllerDelegate>
{
    CADisplayLink* mpDisplayLink ;
}
@property ( nonatomic ) DxLibiCadeReaderView *icadeView ;
@property ( strong, nonatomic ) EAGLContext *context ;
@property ( nonatomic ) AudioUnit audioUnit ;

- ( void )setupControllers:( NSNotification * )notification ;
- ( bool )startInputString:( NSString * )TitleArg Message:( NSString * )MessageArg ;
- ( void )setupMotionManager;
+ ( void )dummyFunction;
- ( int )mailApp_Send:( NSString * )MailAddrArg MailCCAddr:( NSString * )MailCCAddrArg MailBCCAddr:( NSString * )MailBCCAddrArg Subject:( NSString * )SubjectArg Text:( NSString * )TextArg ;
- ( void )initializeAudioUnit ;
- ( void )terminateAudioUnit ;

@end

// テーブル-----------------------------------------------------------------------

// 内部大域変数宣言 --------------------------------------------------------------

DXLIB_IOS_SYSTEMINFO_OBJC g_iOSSysObjC ;

// 関数プロトタイプ宣言-----------------------------------------------------------

// プログラム --------------------------------------------------------------------

@implementation DxLibAppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    // Override point for customization after application launch.
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
	SoundSystem_iOS_ALBuffer_RePlay() ;
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}

@end












@implementation DxLibiCadeReaderView

@synthesize active;

- (id)initWithFrame:(CGRect)frame 
{
    self = [super initWithFrame:frame];
    inputView = [[UIView alloc] initWithFrame:CGRectZero];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(didEnterBackground) name:UIApplicationDidEnterBackgroundNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(didBecomeActive) name:UIApplicationDidBecomeActiveNotification object:nil];
    
    return self;
}

- (void)dealloc 
{
    [[NSNotificationCenter defaultCenter] removeObserver:self name:UIApplicationDidEnterBackgroundNotification object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:UIApplicationDidBecomeActiveNotification object:nil];
#if !__has_feature(objc_arc)
    [super dealloc];
#endif
}

- (void)didEnterBackground 
{
    if( self.active )
	{
        [self resignFirstResponder] ;
	}
}

- (void)didBecomeActive 
{
    if( self.active )
	{
        [self becomeFirstResponder] ;
	}
}

- (BOOL)canBecomeFirstResponder 
{
    return YES; 
}

- (void)setActive:(BOOL)value 
{
    if( active == value )
	{
		return ;
	}
    
    active = value ;
    if( active )
	{
        [self becomeFirstResponder] ;
    }
	else
	{
        [self resignFirstResponder] ;
    }
}

- (UIView*) inputView 
{
    return inputView;
}

- (BOOL)hasText 
{
    return NO;
}

- (void)insertText:(NSString *)text 
{
    iCadeInputSet( [ text characterAtIndex:0 ] ) ;

    static int cycleResponder = 0 ;
    if( ++cycleResponder > 20 )
	{
        // necessary to clear a buffer that accumulates internally
        cycleResponder = 0 ;
        [self resignFirstResponder] ;
        [self becomeFirstResponder] ;
    }
}

- (void)deleteBackward 
{
    // This space intentionally left blank to complete protocol
}

@end













@implementation DxLibViewController

@synthesize audioUnit ;

- (void)viewDidLoad
{
    [super viewDidLoad];

	// 複数タッチ許可
    self.view.multipleTouchEnabled = YES ;

	// Retinaディスプレイ対応
	self.view.contentScaleFactor = [[UIScreen mainScreen] scale] ;

	// ステータスバーを消す
	[ UIApplication sharedApplication ].statusBarHidden = YES ;

	// マイク許可確認処理開始フラグと完了フラグを倒す
	g_iOSSysObjC.RecordPermissionProcessStart = 0 ;
	g_iOSSysObjC.RecordPermissionProcessEnd = 0 ;

	// ビューコントローラのアドレスを保存
    g_iOSSysObjC.ViewController = self;

	g_iOSSysObjC.AEAGLayer = ( CAEAGLLayer * )[ self.view layer ] ;

	g_iOSSysObjC.touchBeginPoints = CFDictionaryCreateMutable( NULL, 0, NULL, NULL ) ;

	// OpenGL ES 2 コンテキスト作成
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    if( !self.context )
	{
        NSLog(@"Failed to create ES context");
    }
    
//    GLKView *view = (GLKView *)self.view;
//    view.context = self.context;
//    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;

	// モーションマネージャのセットアップ
    [ self setupMotionManager ] ;
	
    g_iOSSysObjC.ViewWidth = ( int )( [[UIScreen mainScreen]bounds].size.width * [[UIScreen mainScreen]scale] ) ;
    g_iOSSysObjC.ViewHeight = ( int )( [[UIScreen mainScreen]bounds].size.height * [[UIScreen mainScreen]scale] ) ;
    Graphics_iOS_SetScreenSize( g_iOSSysObjC.ViewWidth, g_iOSSysObjC.ViewHeight );

    [EAGLContext setCurrentContext:self.context];
    
    SetupViewBuffer() ;

    g_iOSSysObjC.GameControllerArray = [ GCController controllers ] ;
    
    /// スクリーンと同期する
	mpDisplayLink = [ [ UIScreen mainScreen ] displayLinkWithTarget:self selector:@selector( VSyncCallback ) ];
	[ mpDisplayLink addToRunLoop:[ NSRunLoop currentRunLoop ] forMode:NSDefaultRunLoopMode ];

	NSNotificationCenter* center = [NSNotificationCenter defaultCenter];  
  
	// 接続されたときの通知を登録  
	[ center addObserver:self
				selector:@selector( setupControllers: )
				name:GCControllerDidConnectNotification
				object:nil ] ;
  
	// 切断されたときの通知を登録  
	[ center addObserver:self
				selector:@selector( setupControllers: )
				name:GCControllerDidDisconnectNotification
				object:nil ] ;

    self.icadeView = [[DxLibiCadeReaderView alloc] initWithFrame:CGRectZero];
    [self.view addSubview:_icadeView];
    _icadeView.active = YES;

    DxSystemInitialize_CPP() ;
}

- (void)viewDidAppear:(BOOL)animated
{
	[ super viewDidAppear: animated ] ;
	_icadeView.active = YES;
}
- (void)viewWillDisappear:(BOOL)animated 
{
	[ super viewWillDisappear: animated ] ;
	_icadeView.active = NO;
}

- (void)setupControllers:(NSNotification *)notification
{  
	g_iOSSysObjC.GameControllerArray = [ GCController controllers ] ;
}

- ( bool )startInputString:( NSString * )TitleArg Message:( NSString * )MessageArg
{
	int OSVersion = ( int )[ UIDevice currentDevice ].systemVersion.floatValue ;

	if( OSVersion <= 7 )
	{
		g_iOSSysObjC.TextInputAlertView = [ UIAlertView alloc ] ;
		[ g_iOSSysObjC.TextInputAlertView initWithTitle:TitleArg message:MessageArg delegate:self cancelButtonTitle:@"Cancel" otherButtonTitles:nil ] ;
		g_iOSSysObjC.TextInputAlertView.alertViewStyle = UIAlertViewStylePlainTextInput ;
		[ g_iOSSysObjC.TextInputAlertView show ] ;
	}
	else
	if( OSVersion == 8 )
	{
		g_iOSSysObjC.TextInputAlertController = [ UIAlertController alertControllerWithTitle:TitleArg message:MessageArg preferredStyle:UIAlertControllerStyleAlert ] ;
		[ g_iOSSysObjC.TextInputAlertController addAction:[ UIAlertAction actionWithTitle:@"Click" style:UIAlertActionStyleDefault handler:nil ] ] ;
		[ g_iOSSysObjC.TextInputAlertController addTextFieldWithConfigurationHandler:^(UITextField *textField) {
			textField.placeholder = @"Enter text:" ;
			textField.secureTextEntry = YES ;
		}];
		[ self presentViewController:g_iOSSysObjC.TextInputAlertController animated:YES completion:nil ] ;
	}
	else
	{
		g_iOSSysObjC.TextInputAlertController = [ UIAlertController alertControllerWithTitle:TitleArg
																   message:MessageArg
																  preferredStyle:UIAlertControllerStyleAlert ] ;

		UIAlertAction* defaultAction = [ UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault
														  handler:^(UIAlertAction * action) {
							//use alert.textFields[0].text
							ConvertNSStringtoTCHAR_NoAlloc( g_iOSSysObjC.TextInputAlertController.textFields[0].text, g_iOSSysObjC.TextInputResultStringBuffer, g_iOSSysObjC.TextInputResultStringBufferBytes ) ;
							g_iOSSysObjC.TextInputAlertRunFlag = FALSE ;
															   }];
		UIAlertAction* cancelAction = [UIAlertAction actionWithTitle:@"Cancel" style:UIAlertActionStyleDefault
															  handler:^(UIAlertAction * action) {
																  //cancel action
							g_iOSSysObjC.TextInputAlertCancelFlag = TRUE ;
							g_iOSSysObjC.TextInputAlertRunFlag = FALSE ;
															  }];
		[g_iOSSysObjC.TextInputAlertController addTextFieldWithConfigurationHandler:^(UITextField * _Nonnull textField) {
			// A block for configuring the text field prior to displaying the alert
		}];
		[ g_iOSSysObjC.TextInputAlertController addAction:defaultAction ] ;
		[ g_iOSSysObjC.TextInputAlertController addAction:cancelAction ] ;
		[ self presentViewController:g_iOSSysObjC.TextInputAlertController animated:YES completion:nil ] ;
	}

	return true ;
}

- ( void )setupMotionManager
{
    // CMMotionManagerのインスタンス生成
    g_iOSSysObjC.MotionManager = [ [ CMMotionManager alloc ] init ] ;

//	if( g_iOSSysObjC.MotionManager.accelerometerAvailable )
//	{
//		// センサーの更新間隔の指定、60Hz
//		g_iOSSysObjC.MotionManager.accelerometerUpdateInterval = 1.0f / 60.0f ;
// 
//		// ハンドラを設定
//		CMAccelerometerHandler handler = ^( CMAccelerometerData *data, NSError *error )
//		{
//			// 加速度を保存
//			g_iOSSysObjC.AccelerationVector = data.acceleration ;
//		} ;
// 
//		// 加速度の取得開始
//		[ g_iOSSysObjC.MotionManager startAccelerometerUpdatesToQueue:[ NSOperationQueue currentQueue ] withHandler:handler ] ;
//	}

	if( g_iOSSysObjC.MotionManager.deviceMotionAvailable)
	{
		// デバイスモーションの通知を開始する
		[ g_iOSSysObjC.MotionManager startDeviceMotionUpdatesToQueue:[ NSOperationQueue currentQueue ]
			withHandler:^( CMDeviceMotion *motion, NSError *error )
			{
				// パラメータを保存する
				g_iOSSysObjC.MotionInfo.Attitude_Roll      = motion.attitude.roll ;
				g_iOSSysObjC.MotionInfo.Attitude_Pitch     = motion.attitude.pitch ;
				g_iOSSysObjC.MotionInfo.Attitude_Yaw       = motion.attitude.yaw ;
				g_iOSSysObjC.MotionInfo.RotationRate.x     = motion.rotationRate.x ;
				g_iOSSysObjC.MotionInfo.RotationRate.y     = motion.rotationRate.y ;
				g_iOSSysObjC.MotionInfo.RotationRate.z     = motion.rotationRate.z ;
				g_iOSSysObjC.MotionInfo.Gravity.x          = motion.gravity.x ;
				g_iOSSysObjC.MotionInfo.Gravity.y          = motion.gravity.y ;
				g_iOSSysObjC.MotionInfo.Gravity.z          = motion.gravity.z ;
				g_iOSSysObjC.MotionInfo.UserAcceleration.x = motion.userAcceleration.x ;
				g_iOSSysObjC.MotionInfo.UserAcceleration.y = motion.userAcceleration.y ;
				g_iOSSysObjC.MotionInfo.UserAcceleration.z = motion.userAcceleration.z ;
//				g_iOSSysObjC.MotionInfo.MagneticField.x    = motion.magneticField.field.x ;
//				g_iOSSysObjC.MotionInfo.MagneticField.y    = motion.magneticField.field.y ;
//				g_iOSSysObjC.MotionInfo.MagneticField.z    = motion.magneticField.field.z ;
	//			g_iOSSysObjC.MotionInfo.Heading            = motion.heading ;
			}
		];
	}

	g_iOSSysObjC.MotionManager.magnetometerUpdateInterval = 1 / 60.0f ;
	[ g_iOSSysObjC.MotionManager startMagnetometerUpdatesToQueue:[ NSOperationQueue currentQueue ]
		withHandler:^(CMMagnetometerData *magnetometerData, NSError *error) 
		{
			// 磁力データの保存
			if( error == NULL )
			{
				g_iOSSysObjC.MotionInfo.MagneticField.x = magnetometerData.magneticField.x ;
				g_iOSSysObjC.MotionInfo.MagneticField.y = magnetometerData.magneticField.y ;
				g_iOSSysObjC.MotionInfo.MagneticField.z = magnetometerData.magneticField.z ;
			}
		}
	];
}

+ ( void )dummyFunction
{
}

// アプリ内メーラーのデリゲートメソッド
- ( void )mailComposeController:( MFMailComposeViewController * )controller didFinishWithResult:( MFMailComposeResult )result error:( NSError * )error
{
	switch( result )
	{
	case MFMailComposeResultCancelled :
		// キャンセル
		break ;

	case MFMailComposeResultSaved :
		// 保存 (ここでアラート表示するなど何らかの処理を行う)
		break ;

	case MFMailComposeResultSent :
		// 送信成功 (ここでアラート表示するなど何らかの処理を行う)
		break ;

	case MFMailComposeResultFailed :
		// 送信失敗 (ここでアラート表示するなど何らかの処理を行う)
		break ;

	default:
		break ;
	}

	[ self dismissViewControllerAnimated:YES completion:nil ] ;
}

- ( int )mailApp_Send:( NSString * )MailAddrArg MailCCAddr:( NSString * )MailCCAddrArg MailBCCAddr:( NSString * )MailBCCAddrArg Subject:( NSString * )SubjectArg Text:( NSString * )TextArg
{
	MFMailComposeViewController *mailPicker = [ [ MFMailComposeViewController alloc ] init ] ;
	mailPicker.mailComposeDelegate = self ;

	if( MailAddrArg != nil )
	{
		NSArray *MailAddrs = [ MailAddrArg componentsSeparatedByString:@"," ] ;
		[ mailPicker setToRecipients:MailAddrs ] ;
	}

	if( MailCCAddrArg != nil )
	{
		NSArray *MailCCAddrs = [ MailCCAddrArg componentsSeparatedByString:@"," ] ;
		[ mailPicker setCcRecipients:MailCCAddrs ] ;
	}

	if( MailBCCAddrArg != nil )
	{
		NSArray *MailBCCAddrs = [ MailBCCAddrArg componentsSeparatedByString:@"," ] ;
		[ mailPicker setBccRecipients:MailBCCAddrs ] ;
	}

	if( SubjectArg != nil )
	{
		[ mailPicker setSubject:SubjectArg ] ;
	}
 
	if( TextArg != nil )
	{
		[ mailPicker setMessageBody:TextArg isHTML:NO ] ;
	}

	[ self presentViewController:mailPicker animated:TRUE completion:nil ] ;

	return 0 ;
}

- (void)alertView:(UIAlertView*)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    /* buttonIndex に押されたボタンのインデックスが入っている */
}

// VSYNC検出
- ( void )VSyncCallback
{
	Graphics_iOS_ResetWaitVSyncFlag() ;

	if( g_iOSSysObjC.TextInputAlertRunRequestFlag )
	{
		g_iOSSysObjC.TextInputAlertRunRequestFlag = FALSE ;
		g_iOSSysObjC.TextInputAlertRunFlag = TRUE ;
		[ self startInputString:g_iOSSysObjC.TextInputAlertTitle  Message:g_iOSSysObjC.TextInputAlertMessage ] ;
	}
}

- (void)dealloc
{
    if( [ EAGLContext currentContext ] == self.context )
	{
        [ EAGLContext setCurrentContext:nil ] ;
    }

	if( mpDisplayLink != NULL )
	{
		[ mpDisplayLink invalidate ] ;
	}
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    
    if ([self isViewLoaded] && ([[self view] window] == nil)) 
	{
        self.view = nil;
        
        if ([EAGLContext currentContext] == self.context) 
		{
            [EAGLContext setCurrentContext:nil];
        }
        self.context = nil;
    }
    
    // Dispose of any resources that can be recreated.
}


- (void)insertText:(NSString *)text 
{
    iCadeInputSet( [ text characterAtIndex:0 ] ) ;
    
    static int cycleResponder = 0;
    if( ++cycleResponder > 20 )
	{
        // necessary to clear a buffer that accumulates internally
        cycleResponder = 0;
        [self resignFirstResponder];
        [self becomeFirstResponder];
    }
}

OSStatus AudioUnitRenderCallback(
	void *inRefCon,
	AudioUnitRenderActionFlags *ioActionFlags,
	const AudioTimeStamp *inTimeStamp,
	UInt32 inBusNumber,
	UInt32 inNumberFrames,
	AudioBufferList *ioData)
{
	if( ioData->mNumberBuffers > 1 )
	{
		WriteSelfMixingSample( 
			( BYTE * )ioData->mBuffers[ 0 ].mData,
            ( BYTE * )ioData->mBuffers[ 1 ].mData,
			4,
			inNumberFrames
		) ;
	}
	else
	{
		WriteSelfMixingSample( 
            ( BYTE * )ioData->mBuffers[ 0 ].mData,
            ( BYTE * )ioData->mBuffers[ 0 ].mData + 4,
			8,
			inNumberFrames
		) ;
	}

	return noErr;
}

- ( void )initializeAudioUnit
{
	OSStatus status ;

	// RemoteIO AudioUnitのAudioComponentDescriptionを作成
	AudioComponentDescription cd;
	cd.componentType         = kAudioUnitType_Output ;
	cd.componentSubType      = kAudioUnitSubType_RemoteIO ;
	cd.componentManufacturer = kAudioUnitManufacturer_Apple ;
	cd.componentFlags        = 0 ;
	cd.componentFlagsMask    = 0 ; 

	// Audio Componentの定義を取得
	AudioComponent comp = AudioComponentFindNext( NULL, &cd ) ;

	// インスタンス化
	status = AudioComponentInstanceNew( comp, &audioUnit ) ; 

	// コールバック関数の設定
	AURenderCallbackStruct input ;
	input.inputProc       = AudioUnitRenderCallback;
	input.inputProcRefCon = NULL ;
	status = AudioUnitSetProperty( audioUnit, kAudioUnitProperty_SetRenderCallback, kAudioUnitScope_Input, 0, &input, sizeof( input ) ) ;

	// 自前ミキシングフォーマット情報をセット
	_MEMSET( &SoundSysData.SelfMixingFormat, 0, sizeof( SoundSysData.SelfMixingFormat ) ) ;
	SoundSysData.SelfMixingFormat.cbSize          = 0 ;
	SoundSysData.SelfMixingFormat.wFormatTag      = WAVE_FORMAT_PCM ;
	SoundSysData.SelfMixingFormat.nChannels       = 2 ;
	SoundSysData.SelfMixingFormat.nSamplesPerSec  = 48000 ;
	SoundSysData.SelfMixingFormat.wBitsPerSample  = 32 ;
	SoundSysData.SelfMixingFormat.nBlockAlign     = SoundSysData.SelfMixingFormat.nChannels * SoundSysData.SelfMixingFormat.wBitsPerSample / 8 ;
	SoundSysData.SelfMixingFormat.nAvgBytesPerSec = SoundSysData.SelfMixingFormat.nSamplesPerSec * SoundSysData.SelfMixingFormat.nBlockAlign ;

	SoundSysData.SelfMixingFormatValidBitsPerSample = 32 ;
	SoundSysData.SelfMixingFormatIsMSB = FALSE ;
	SoundSysData.SelfMixingFormatIsFloat = TRUE ;

	// 自前ミキシング作業用バッファの準備
	SetupSelfMixingWorkBuffer( SoundSysData.SelfMixingFormatIsFloat, 1024 ) ;

	// 出力フォーマットを設定
	AudioStreamBasicDescription asbd ;
	asbd.mSampleRate		= 48000.0 ;
	asbd.mFormatID			= kAudioFormatLinearPCM ;
	asbd.mFormatFlags		= kAudioFormatFlagIsFloat ;
	asbd.mChannelsPerFrame	= 2 ;
	asbd.mBytesPerPacket	= 8 ;
	asbd.mBytesPerFrame		= 8 ;
	asbd.mFramesPerPacket	= 1 ;
	asbd.mBitsPerChannel	= 32 ;
	asbd.mReserved			= 0 ;
	status = AudioUnitSetProperty( audioUnit, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Input, 0, &asbd, sizeof( asbd ) ) ;

	// 初期化
	status = AudioUnitInitialize( audioUnit ) ;

	// 再生開始
	status = AudioOutputUnitStart( audioUnit ) ;
}

-( void ) terminateAudioUnit
{
	AudioOutputUnitStop( audioUnit ) ;
    AudioUnitUninitialize( audioUnit ) ;
    AudioComponentInstanceDispose( audioUnit ) ;
}

@end













@implementation DxLibGLView

// このクラスメソッドをオーバーライドすることで OpenGL ESを描画できるレイヤーをセットする
+ ( Class )layerClass
{
    return [ CAEAGLLayer class ];
}

- ( id ) initWithCoder:(NSCoder *)aDecoder
{
    self = [super initWithCoder:aDecoder];
    if( self )
    {
        // ビューのアドレスを保存
        g_iOSSysObjC.DxLibGLView = self;
    }

    return self;
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event 
{
	for( UITouch *touch in touches )
	{
		DXLIB_IOS_TOUCH_INFO *TouchInfo = NULL ;
		if( g_iOSSysObjC.touchInfoNum < MAX_TOUCH_NUM )
		{
			for( TouchInfo = g_iOSSysObjC.touchInfos; TouchInfo->UseFlag; TouchInfo++ ){}
			TouchInfo->UseFlag = TRUE ;
			TouchInfo->TouchID = g_iOSSysObjC.NextTouchID ;
			g_iOSSysObjC.NextTouchID ++ ;

			CGPoint currentPoint = [ touch locationInView:self.superview ] ;
			TouchInfo->PositionX = currentPoint.x * [[UIScreen mainScreen] scale] ;
			TouchInfo->PositionY = currentPoint.y * [[UIScreen mainScreen] scale] ;

			g_iOSSysObjC.touchInfoNum++ ;
		}

		CFDictionarySetValue( g_iOSSysObjC.touchBeginPoints, ( const void * )touch, TouchInfo ) ;
	}

	UpdateTouchInputData( ( LONGLONG )( event.timestamp * 1000.0 ) ) ;
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	for( UITouch *touch in touches ) 
	{
		DXLIB_IOS_TOUCH_INFO *TouchInfo = ( DXLIB_IOS_TOUCH_INFO * )CFDictionaryGetValue( g_iOSSysObjC.touchBeginPoints, ( const void * )touch ) ;

		if( TouchInfo != NULL )
		{
			CGPoint currentPoint = [ touch locationInView:self.superview ] ;
			TouchInfo->PositionX = currentPoint.x * [[UIScreen mainScreen] scale] ;
			TouchInfo->PositionY = currentPoint.y * [[UIScreen mainScreen] scale] ;
		}
	}

	UpdateTouchInputData( ( LONGLONG )( event.timestamp * 1000.0 ) ) ;
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	for( UITouch *touch in touches )
	{
		DXLIB_IOS_TOUCH_INFO *TouchInfo = ( DXLIB_IOS_TOUCH_INFO * )CFDictionaryGetValue( g_iOSSysObjC.touchBeginPoints, ( const void * )touch ) ;

		if( TouchInfo != NULL )
		{
			TouchInfo->UseFlag = FALSE ;
			g_iOSSysObjC.touchInfoNum -- ;
		}

		CFDictionaryRemoveValue( g_iOSSysObjC.touchBeginPoints, ( const void * )touch ) ;
	}

	UpdateTouchInputData( ( LONGLONG )( event.timestamp * 1000.0 ) ) ;
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event 
{
    [self touchesEnded:touches withEvent:event] ;
}

// 終了
- ( void )dealloc
{
	if( g_iOSSysObjC.touchBeginPoints )
	{
		CFRelease( g_iOSSysObjC.touchBeginPoints ) ;
		g_iOSSysObjC.touchBeginPoints = NULL ;
	}
}

- ( void )disableGesture
{
	// ジェスチャー機能無効化
	for( int i = 0; i < self.window.gestureRecognizers.count; i++ )
	{
		self.window.gestureRecognizers[i].delaysTouchesBegan = false ;
	}
}

+ (void)dummyFunction
{
}

@end















int main(int argc, char *argv[])
{
	[ DxLibViewController dummyFunction ] ;
	[ DxLibGLView dummyFunction ] ;

    @autoreleasepool 
	{
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([DxLibAppDelegate class]));
    }
}

int GraphicsInitializeFlag = 0 ;

static void UpdateTouchInputData( LONGLONG Time )
{
	TOUCHINPUTDATA TouchInputData ;
	TOUCHINPUTPOINT *TouchPoint ;
	DXLIB_IOS_TOUCH_INFO *TouchInfo ;
	int i, j ;

	TouchInputData.PointNum = 0 ;
	TouchInputData.Time = Time ;
	TouchPoint = TouchInputData.Point ;
	TouchInfo = g_iOSSysObjC.touchInfos ;
	j = 0 ;
	for( i = 0 ; j < g_iOSSysObjC.touchInfoNum ; i ++, TouchInfo ++ )
	{
		if( TouchInfo->UseFlag == FALSE )
		{
			continue ;
		}

		j++ ;

		TouchPoint->Device = 0 ;
		TouchPoint->ID = TouchInfo->TouchID ;
		ConvScreenPositionToDxScreenPosition(
			( int )TouchInfo->PositionX,
			( int )TouchInfo->PositionY,
			&TouchPoint->PositionX,
			&TouchPoint->PositionY
		) ;

		TouchInputData.PointNum ++ ;
		TouchPoint ++ ;
	}

	AddTouchInputData( &TouchInputData ) ;
}

// ゲームコントローラー関係関数
void UpdateControllerInfo( int PadNo )
{
	INPUTPADDATA *pad = &InputSysData.Pad[ PadNo ] ;
	float DeadZone = pad->DeadZone / 65536.0f ;

	g_iOSSysObjC.GameControllerArray = [ GCController controllers ] ;

	if( [ g_iOSSysObjC.GameControllerArray count ] <= PadNo )
	{
		return ;
	}

	GCController *Controller = [ g_iOSSysObjC.GameControllerArray objectAtIndex:PadNo ] ;

	if( Controller.extendedGamepad != nil )
	{
		pad->State.X  = Controller.extendedGamepad.leftThumbstick.xAxis.value  < DeadZone && Controller.extendedGamepad.leftThumbstick.xAxis.value  > -DeadZone ? 0 : ( int )( Controller.extendedGamepad.leftThumbstick.xAxis.value  * 1000.0f ) ;
		pad->State.Y  = -( Controller.extendedGamepad.leftThumbstick.yAxis.value  < DeadZone && Controller.extendedGamepad.leftThumbstick.yAxis.value  > -DeadZone ? 0 : ( int )( Controller.extendedGamepad.leftThumbstick.yAxis.value  * 1000.0f ) ) ;
		pad->State.Rx = Controller.extendedGamepad.rightThumbstick.xAxis.value < DeadZone && Controller.extendedGamepad.rightThumbstick.xAxis.value > -DeadZone ? 0 : ( int )( Controller.extendedGamepad.rightThumbstick.xAxis.value * 1000.0f ) ;
		pad->State.Ry = -( Controller.extendedGamepad.rightThumbstick.yAxis.value < DeadZone && Controller.extendedGamepad.rightThumbstick.yAxis.value > -DeadZone ? 0 : ( int )( Controller.extendedGamepad.rightThumbstick.yAxis.value * 1000.0f ) ) ;
		pad->State.Rz = 0 ;
		pad->State.Slider[ 0 ]   = ( int )( Controller.extendedGamepad.leftShoulder.value  * 1000.0f ) ;
		pad->State.Slider[ 1 ]   = ( int )( Controller.extendedGamepad.rightShoulder.value * 1000.0f ) ;
		if( Controller.extendedGamepad.dpad.xAxis.value > 0.5f )
		{
			if( -Controller.extendedGamepad.dpad.yAxis.value > 0.5f )
			{
				pad->State.POV[ 0 ] = 13500 ;
			}
			else
			if( -Controller.extendedGamepad.dpad.yAxis.value < -0.5f )
			{
				pad->State.POV[ 0 ] = 4500 ;
			}
			else
			{
				pad->State.POV[ 0 ] = 9000 ;
			}
		}
		else
		if( Controller.extendedGamepad.dpad.xAxis.value < -0.5f )
		{
			if( -Controller.extendedGamepad.dpad.yAxis.value > 0.5f )
			{
				pad->State.POV[ 0 ] = 22500 ;
			}
			else
			if( -Controller.extendedGamepad.dpad.yAxis.value < -0.5f )
			{
				pad->State.POV[ 0 ] = 31500 ;
			}
			else
			{
				pad->State.POV[ 0 ] = 27000 ;
			}
		}
		else
		{
			if( -Controller.extendedGamepad.dpad.yAxis.value > 0.5f )
			{
				pad->State.POV[ 0 ] = 18000 ;
			}
			else
			if( -Controller.extendedGamepad.dpad.yAxis.value < -0.5f )
			{
				pad->State.POV[ 0 ] = 0 ;
			}
			else
			{
				pad->State.POV[ 0 ] = 0xffffffff ;
			}
		}
		pad->State.POV[ 1 ]      = 0xffffffff ;
		pad->State.POV[ 2 ]      = 0xffffffff ;
		pad->State.POV[ 3 ]      = 0xffffffff ;

		pad->State.Z = 0 ;
		if( Controller.extendedGamepad.leftTrigger.value  > DeadZone )
		{
			pad->State.Z += ( Controller.extendedGamepad.leftTrigger.value - DeadZone ) * DIRINPUT_MAX / ( 1.0f - DeadZone ) ;
		}
		if( Controller.extendedGamepad.rightTrigger.value > DeadZone )
		{
			pad->State.Z -= ( Controller.extendedGamepad.rightTrigger.value - DeadZone ) * DIRINPUT_MAX / ( 1.0f - DeadZone ) ;
		}

		pad->State.Buttons[  0 ] = Controller.extendedGamepad.buttonA.isPressed					? 0x80 : 0x00 ;
		pad->State.Buttons[  1 ] = Controller.extendedGamepad.buttonB.isPressed					? 0x80 : 0x00 ;
		pad->State.Buttons[  2 ] = Controller.extendedGamepad.buttonX.isPressed					? 0x80 : 0x00 ;
		pad->State.Buttons[  3 ] = Controller.extendedGamepad.buttonY.isPressed					? 0x80 : 0x00 ;
		pad->State.Buttons[  4 ] = Controller.extendedGamepad.leftShoulder.isPressed			? 0x80 : 0x00 ;
		pad->State.Buttons[  5 ] = Controller.extendedGamepad.rightShoulder.isPressed			? 0x80 : 0x00 ;
		pad->State.Buttons[  6 ] = Controller.extendedGamepad.leftTrigger.isPressed				? 0x80 : 0x00 ;
		pad->State.Buttons[  7 ] = Controller.extendedGamepad.rightTrigger.isPressed			? 0x80 : 0x00 ;
		pad->State.Buttons[  8 ] = 0 /*Controller.extendedGamepad.leftThumbstickButton.isPressed	? 0x80 : 0x00*/ ;
		pad->State.Buttons[  9 ] = 0 /*Controller.extendedGamepad.rightThumbstickButton.isPressed	? 0x80 : 0x00*/ ;
		pad->State.Buttons[ 10 ] = 0 ;
		pad->State.Buttons[ 11 ] = 0 ;
		pad->State.Buttons[ 12 ] = 0 ;
		pad->State.Buttons[ 13 ] = 0 ;
		pad->State.Buttons[ 14 ] = 0 ;
		pad->State.Buttons[ 15 ] = 0 ;
		pad->State.Buttons[ 16 ] = 0 ;
		pad->State.Buttons[ 17 ] = 0 ;
		pad->State.Buttons[ 18 ] = 0 ;
		pad->State.Buttons[ 19 ] = 0 ;
		pad->State.Buttons[ 20 ] = 0 ;
		pad->State.Buttons[ 21 ] = 0 ;
		pad->State.Buttons[ 22 ] = 0 ;
		pad->State.Buttons[ 23 ] = 0 ;
		pad->State.Buttons[ 24 ] = 0 ;
		pad->State.Buttons[ 25 ] = 0 ;
		pad->State.Buttons[ 26 ] = 0 ;
		pad->State.Buttons[ 27 ] = 0 ;
		pad->State.Buttons[ 28 ] = 0 ;
		pad->State.Buttons[ 29 ] = 0 ;
		pad->State.Buttons[ 30 ] = 0 ;
		pad->State.Buttons[ 31 ] = 0 ;
	}
	else
	if( Controller.gamepad != nil )
	{
		pad->State.X  = Controller.gamepad.dpad.xAxis.value  < DeadZone && Controller.gamepad.dpad.xAxis.value  > -DeadZone ? 0 : ( int )( Controller.gamepad.dpad.xAxis.value  * 1000.0f ) ;
		pad->State.Y  = Controller.gamepad.dpad.yAxis.value  < DeadZone && Controller.gamepad.dpad.yAxis.value  > -DeadZone ? 0 : ( int )( Controller.gamepad.dpad.yAxis.value  * 1000.0f ) ;
		pad->State.Z  = 0 ;
		pad->State.Rx = 0 ;
		pad->State.Ry = 0 ;
		pad->State.Rz = 0 ;
		pad->State.Slider[ 0 ]   = ( int )( Controller.gamepad.leftShoulder.value  * 1000.0f ) ;
		pad->State.Slider[ 1 ]   = ( int )( Controller.gamepad.rightShoulder.value * 1000.0f ) ;
		pad->State.POV[ 0 ]      = 0xffffffff ;
		pad->State.POV[ 1 ]      = 0xffffffff ;
		pad->State.POV[ 2 ]      = 0xffffffff ;
		pad->State.POV[ 3 ]      = 0xffffffff ;

		pad->State.Buttons[  0 ] = Controller.gamepad.buttonA.isPressed					? 0x80 : 0x00 ;
		pad->State.Buttons[  1 ] = Controller.gamepad.buttonB.isPressed					? 0x80 : 0x00 ;
		pad->State.Buttons[  2 ] = Controller.gamepad.buttonX.isPressed					? 0x80 : 0x00 ;
		pad->State.Buttons[  3 ] = Controller.gamepad.buttonY.isPressed					? 0x80 : 0x00 ;
		pad->State.Buttons[  4 ] = Controller.gamepad.leftShoulder.isPressed			? 0x80 : 0x00 ;
		pad->State.Buttons[  5 ] = Controller.gamepad.rightShoulder.isPressed			? 0x80 : 0x00 ;
		pad->State.Buttons[  6 ] = 0 ;
		pad->State.Buttons[  7 ] = 0 ;
		pad->State.Buttons[  8 ] = 0 ;
		pad->State.Buttons[  9 ] = 0 ;
		pad->State.Buttons[ 10 ] = 0 ;
		pad->State.Buttons[ 11 ] = 0 ;
		pad->State.Buttons[ 12 ] = 0 ;
		pad->State.Buttons[ 13 ] = 0 ;
		pad->State.Buttons[ 14 ] = 0 ;
		pad->State.Buttons[ 15 ] = 0 ;
		pad->State.Buttons[ 16 ] = 0 ;
		pad->State.Buttons[ 17 ] = 0 ;
		pad->State.Buttons[ 18 ] = 0 ;
		pad->State.Buttons[ 19 ] = 0 ;
		pad->State.Buttons[ 20 ] = 0 ;
		pad->State.Buttons[ 21 ] = 0 ;
		pad->State.Buttons[ 22 ] = 0 ;
		pad->State.Buttons[ 23 ] = 0 ;
		pad->State.Buttons[ 24 ] = 0 ;
		pad->State.Buttons[ 25 ] = 0 ;
		pad->State.Buttons[ 26 ] = 0 ;
		pad->State.Buttons[ 27 ] = 0 ;
		pad->State.Buttons[ 28 ] = 0 ;
		pad->State.Buttons[ 29 ] = 0 ;
		pad->State.Buttons[ 30 ] = 0 ;
		pad->State.Buttons[ 31 ] = 0 ;
	}
}

// マイク許可処理完了確認
int CheckRecordPermissionProcessEnd( void )
{
	if( g_iOSSysObjC.RecordPermissionProcessEnd == 0 )
	{
        // マイク許可確認
//		if( g_iOSSysObjC.RecordPermissionProcessStart == 0 )
//		{
//			g_iOSSysObjC.RecordPermissionProcessStart = 1 ;
//			[ [ AVAudioSession sharedInstance ] requestRecordPermission:^(bool granted){
//				g_iOSSysObjC.RecordPermissionProcessEnd = 1 ;
//				// 許可
//				if( granted )
//				{
//　　　　			// 録音処理
//				}
//				else
//				{
//　					// ユーザーに拒否設定のため録音デバイスを利用できない旨通知
//				}
//			}];
//		}

//		AVAuthorizationStatus status = [ AVCaptureDevice authorizationStatusForMediaType:AVMediaTypeAudio ] ;
//		switch( status )
//		{
//		case AVAuthorizationStatusAuthorized:
//			// プライバシー設定でカメラの使用が許可されている場合
//			g_iOSSysObjC.RecordPermissionProcessEnd = 1 ;
//			break;
//
//		case AVAuthorizationStatusDenied:
//			// プライバシー設定でカメラの使用が禁止されている場合
//			g_iOSSysObjC.RecordPermissionProcessEnd = 1 ;
//			break;
//
//		case AVAuthorizationStatusRestricted:
//			// 機能制限の場合とあるが、実際にこの値をとることがなかった
//			g_iOSSysObjC.RecordPermissionProcessEnd = 1 ;
//			break;
//
//		case AVAuthorizationStatusNotDetermined:
//			// 初回起動時に許可設定を促すダイアログが表示される
//	/*		[ AVCaptureDevice requestAccessForMediaType:AVMediaTypeAudio completionHandler:^(bool granted)
//				{
//					g_iOSSysObjC.RecordPermissionProcessEnd = 1 ;
//					if( granted )
//					{
//						// 許可された場合の処理
//						dispatch_async(dispatch_get_main_queue(), ^{
//							// do something
//						} ) ;
//					}
//					else
//					{
//						// 許可してもらえない場合
//						dispatch_async(dispatch_get_main_queue(), ^{
//							// do something
//						} ) ;
//					}
//				}
//			];*/
//			break;
//
//		default:
//			g_iOSSysObjC.RecordPermissionProcessEnd = 1 ;
//			break;
//		}
	}

	return g_iOSSysObjC.RecordPermissionProcessEnd ;
}


// メールアプリ起動
int	MailApp_Send_iOS( const char *MailAddr, const char *MailCCAddr, const char *MailBCCAddr, const char *Subject, const char *Text )
{
	NSString *MailAddrNSStr = nil ;
	NSString *MailCCAddrNSStr = nil ;
	NSString *MailBCCAddrNSStr = nil ;
	NSString *SubjectNSStr = nil ;
	NSString *TextNSStr = nil ;
	int Result = -1 ;

	Class mailClass = ( NSClassFromString( @"MFMailComposeViewController" ) ) ;
	if( mailClass == nil )
	{
		return -1 ;
	}

	if( MailAddr != NULL )
	{
		MailAddrNSStr = [ NSString stringWithCString:MailAddr encoding:NSUTF8StringEncoding ] ;
	}

	if( MailCCAddr != NULL )
	{
		MailCCAddrNSStr = [ NSString stringWithCString:MailCCAddr encoding:NSUTF8StringEncoding ] ;
	}

	if( MailBCCAddr != NULL )
	{
		MailBCCAddrNSStr = [ NSString stringWithCString:MailBCCAddr encoding:NSUTF8StringEncoding ] ;
	}

	if( Subject != NULL )
	{
		SubjectNSStr = [ NSString stringWithCString:Subject encoding:NSUTF8StringEncoding ] ;
	}

	if( Text != NULL )
	{
		TextNSStr = [ NSString stringWithCString:Text encoding:NSUTF8StringEncoding ] ;
	}

    DxLibViewController *lDxlibViewController = ( DxLibViewController * )g_iOSSysObjC.ViewController ;
	[ lDxlibViewController mailApp_Send:MailAddrNSStr MailCCAddr:MailCCAddrNSStr MailBCCAddr:MailBCCAddrNSStr Subject:SubjectNSStr Text:TextNSStr ] ;

	return 0 ;
}

// ジェスチャー無効化
extern int DisableGesture_iOS( void )
{
    DxLibGLView *lDxlibGLView = ( DxLibGLView * )g_iOSSysObjC.DxLibGLView ;
	[ lDxlibGLView disableGesture ] ;

	return 0 ;
}















// 自前ミキシング用のサウンド再生の準備を行う
extern int SelfMixingPlayer_Setup( void )
{
    DxLibViewController *lDxlibViewController = ( DxLibViewController * )g_iOSSysObjC.ViewController ;
	[ lDxlibViewController initializeAudioUnit ] ;

	// 正常終了
	return 0 ;
}

// 自前ミキシング用のサウンド再生の後始末を行う
extern int SelfMixingPlayer_Terminate( void )
{
    DxLibViewController *lDxlibViewController = ( DxLibViewController * )g_iOSSysObjC.ViewController ;
	[ lDxlibViewController terminateAudioUnit ] ;

	// 正常終了
	return 0 ;
}
















int CheckGraphicsInitialize( void )
{
    return GraphicsInitializeFlag;
}

static int SetupViewBuffer( void )
{
	if( g_iOSSysObjC.ViewFrameBuffer == 0 )
	{
        DxLibViewController *lDxlibViewController = ( DxLibViewController * )g_iOSSysObjC.ViewController ;
	    [ EAGLContext setCurrentContext:lDxlibViewController.context ] ;

		glGenFramebuffers( 1, &g_iOSSysObjC.ViewFrameBuffer ) ;
		glBindFramebuffer( GL_FRAMEBUFFER, g_iOSSysObjC.ViewFrameBuffer ) ;

		glGenRenderbuffers( 1, &g_iOSSysObjC.ViewDepthBuffer ) ;
		glBindRenderbuffer( GL_RENDERBUFFER, g_iOSSysObjC.ViewDepthBuffer ) ;
		glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, GLsizei( ( int )( [[UIScreen mainScreen]bounds].size.width * [[UIScreen mainScreen]scale] ) ), GLsizei( ( int )( [[UIScreen mainScreen]bounds].size.height * [[UIScreen mainScreen]scale] ) ) ) ;
    
		glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, g_iOSSysObjC.ViewDepthBuffer ) ;

		glGenRenderbuffers( 1, &g_iOSSysObjC.ViewRenderBuffer ) ;
		glBindRenderbuffer( GL_RENDERBUFFER, g_iOSSysObjC.ViewRenderBuffer ) ;
		glRenderbufferStorage( GL_RENDERBUFFER, GL_RGB, GLsizei( ( int )( [[UIScreen mainScreen]bounds].size.width * [[UIScreen mainScreen]scale] ) ), GLsizei( ( int )( [[UIScreen mainScreen]bounds].size.height * [[UIScreen mainScreen]scale] ) ) ) ;
    
		glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, g_iOSSysObjC.ViewRenderBuffer ) ;

		[ lDxlibViewController.context renderbufferStorage:GL_RENDERBUFFER fromDrawable:g_iOSSysObjC.AEAGLayer ] ;
	}

	return 0 ;
}

static int TerminateViewBuffer( void )
{
	if( g_iOSSysObjC.ViewFrameBuffer != 0 )
	{
//		if( [ EAGLContext currentContext ] == g_iOSSysObjC.ViewController.context )
//		{
//	        [ EAGLContext setCurrentContext:nil ] ;
//		}

		glBindFramebuffer( GL_FRAMEBUFFER, 0 ) ;
		glBindFramebuffer( GL_RENDERBUFFER, 0 ) ;

		if( g_iOSSysObjC.ViewFrameBuffer != 0 )
		{
			glDeleteFramebuffers( 1, &g_iOSSysObjC.ViewFrameBuffer ) ;
			g_iOSSysObjC.ViewFrameBuffer = 0 ;
		}

		if( g_iOSSysObjC.ViewDepthBuffer != 0 )
		{
			glDeleteRenderbuffers( 1, &g_iOSSysObjC.ViewDepthBuffer ) ;
			g_iOSSysObjC.ViewDepthBuffer = 0 ;
		}

		if( g_iOSSysObjC.ViewRenderBuffer != 0 )
		{
			glDeleteRenderbuffers( 1, &g_iOSSysObjC.ViewRenderBuffer ) ;
			g_iOSSysObjC.ViewRenderBuffer = 0 ;
		}
	}

	return 0 ;
}

// ビューフレームバッファの情報を取得する
int GetGraphicsViewFramebufferInfo( unsigned int *ViewFrameBuffer, int *Width, int *Height )
{
	if( g_iOSSysObjC.ViewWidth != ( int )( [[UIScreen mainScreen]bounds].size.width * [[UIScreen mainScreen]scale] ) ||
		g_iOSSysObjC.ViewHeight != ( int )( [[UIScreen mainScreen]bounds].size.height * [[UIScreen mainScreen]scale] ) )
	{
		TerminateViewBuffer() ;

		SetupViewBuffer() ;

		g_iOSSysObjC.ViewWidth = ( int )( [[UIScreen mainScreen]bounds].size.width * [[UIScreen mainScreen]scale] );
		g_iOSSysObjC.ViewHeight = ( int )( [[UIScreen mainScreen]bounds].size.height * [[UIScreen mainScreen]scale] );
		Graphics_iOS_SetScreenSize( g_iOSSysObjC.ViewWidth, g_iOSSysObjC.ViewHeight );
	}

    if( ViewFrameBuffer ) *ViewFrameBuffer = g_iOSSysObjC.ViewFrameBuffer;
    if( Width ) *Width = g_iOSSysObjC.ViewWidth;
    if( Height ) *Height = g_iOSSysObjC.ViewHeight;
    
    return 0;
}

// カレントコンテキストをセットする
void GraphicsSetCurrentContext( void )
{
    DxLibViewController *lDxlibViewController = ( DxLibViewController * )g_iOSSysObjC.ViewController ;
    [EAGLContext setCurrentContext:lDxlibViewController.context];
}

// ビューにレンダーバッファをアウトプットする
void GraphicsViewRenderBufferPresent( void )
{
    DxLibViewController *lDxlibViewController = ( DxLibViewController * )g_iOSSysObjC.ViewController ;
    [ EAGLContext setCurrentContext:lDxlibViewController.context ] ;

    glBindRenderbuffer( GL_RENDERBUFFER, g_iOSSysObjC.ViewRenderBuffer ) ;
    
    [ lDxlibViewController.context presentRenderbuffer:GL_RENDERBUFFER ] ;
}

// アプリケーションディレクトリを取得する
const char *GetApplicationDirectory( void )
{
	return [ [ [ NSBundle mainBundle ] bundlePath ] UTF8String ] ;
}

// Homeディレクトリを取得する
const char *GetHomeDirectory( void )
{
	return ( char * )[ NSHomeDirectory() UTF8String ] ;
}

// Tmpディレクトリを取得する
const char *GetTmpDirectory( void )
{
	return ( char * )[ NSTemporaryDirectory() UTF8String ] ;
}


// フォント関係関数

// フォントデータの作成
void * CreateDxiOSFontData( const char *FontName, int FontSize )
{
	DXLIB_IOS_FONTINFO *Font ;

	// メモリの確保
	Font = ( DXLIB_IOS_FONTINFO * )DXALLOC( sizeof( DXLIB_IOS_FONTINFO ) ) ;
	if( Font == NULL )
	{
		return NULL ;
	}
	_MEMSET( Font, 0, sizeof( DXLIB_IOS_FONTINFO ) ) ;

	// フォントの作成
	if( FontName == NULL || FontName[ 0 ] == 0 )
	{
		// フォント名の指定が無い場合はデフォルトフォントを使用する
		UIFont *SystemFont = [ UIFont systemFontOfSize:FontSize ] ;
		Font->Font = CTFontCreateWithName( ( CFStringRef )SystemFont.fontName, SystemFont.pointSize, NULL ) ;
	}
	else
	{
		CFStringRef FontNameCFStr = CFStringCreateWithBytes( kCFAllocatorDefault, ( const UInt8 * )FontName, CL_strlen( DX_CHARCODEFORMAT_ASCII, FontName ), kCFStringEncodingUTF8, FALSE ) ;
		Font->Font = CTFontCreateWithName( FontNameCFStr, FontSize, NULL ) ;
	}
	if( Font->Font == NULL )
	{
		return NULL ;
	}

	// フォントの情報を取得
	Font->FontAscent = CTFontGetAscent( Font->Font ) ;
	Font->FontDescent = CTFontGetDescent( Font->Font ) ;
	Font->FontLeading = CTFontGetLeading( Font->Font ) ;
	Font->FontUnderlinePosition = CTFontGetUnderlinePosition( Font->Font ) ;
	Font->FontUnderlineThickness = CTFontGetUnderlineThickness( Font->Font ) ;
	Font->FontCapHeight = CTFontGetCapHeight( Font->Font ) ;
	Font->FontXHeight = CTFontGetXHeight( Font->Font ) ;

	// 一時バッファのサイズはフォントサイズの1.5倍
	Font->ImageWidth  = FontSize * 3 / 2 ;
	Font->ImageHeight = FontSize * 3 / 2 ;

	// 一時バッファのサイズを4の倍数にする
	Font->ImageWidth  = ( Font->ImageWidth  + 3 ) / 4 * 4 ;
	Font->ImageHeight = ( Font->ImageHeight + 3 ) / 4 * 4 ;

	// ピッチの算出
	Font->ImagePitch = Font->ImageWidth /* * 4 */ ;

	// メモリの確保
	Font->ImageData = DXALLOC( Font->ImagePitch * Font->ImageHeight ) ;
    
    // ビットマップをクリア
    _MEMSET( Font->ImageData, 0, Font->ImagePitch * Font->ImageHeight ) ;

	// ビットマップコンテキストの作成
	Font->ColorSpace = CGColorSpaceCreateDeviceGray() ;//RGB() ;
    Font->BitmapContext = CGBitmapContextCreate(
		Font->ImageData,
		Font->ImageWidth,
		Font->ImageHeight,
		8,
		Font->ImagePitch,
		Font->ColorSpace,
		kCGImageAlphaOnly//kCGImageAlphaPremultipliedFirst
	) ;
   
	// 文字列を作成
//	Font->AttrString = CFAttributedStringCreateMutable( kCFAllocatorDefault, 0 ) ;

    // 描画先の矩形パスを生成
	Font->RectPath = CGPathCreateMutable() ;
	CGRect bounds = CGRectMake( 0, 0, Font->ImageWidth, Font->ImageHeight ) ;
	CGPathAddRect( Font->RectPath, NULL, bounds ) ;

	// 終了
	return Font ;
}

// フォントデータの削除
int DeleteDxiOSFontData( void *FontData )
{
	DXLIB_IOS_FONTINFO *Font = ( DXLIB_IOS_FONTINFO * )FontData ;

	// 矩形パスの削除
	CGPathRelease( Font->RectPath ) ;

	// 文字列の削除
//	CFRelease( Font->AttrString ) ;

	// フォントの解放
	CFRelease( Font->Font ) ;

	// カラースペースの解放
	CGColorSpaceRelease( Font->ColorSpace ) ;

	// ビットマップコンテキストの解放
	CGContextRelease( Font->BitmapContext ) ;

	// ビットマップイメージを格納していたメモリの解放
	DXFREE( Font->ImageData ) ;

	// メモリの解放
	DXFREE( Font ) ;

	// 終了
	return 0 ;
}

// フォントデータの基本情報を取得
void GetDxiOSFontBaseInfo( void *FontData, int *ImageWidth, int *ImageHeight, int *ImagePitch, float *FontAscent, float *FontDescent, float *FontLeading, float *FontUnderlinePosition, float *FontUnderlineThickness, float *FontCapHeight, float *FontXHeight )
{
	DXLIB_IOS_FONTINFO *Font = ( DXLIB_IOS_FONTINFO * )FontData ;

	if( ImageWidth				) *ImageWidth				= Font->ImageWidth ;
	if( ImageHeight				) *ImageHeight				= Font->ImageHeight ;
	if( ImagePitch				) *ImagePitch				= Font->ImagePitch ;
	if( FontAscent				) *FontAscent				= Font->FontAscent ;
	if( FontDescent				) *FontDescent				= Font->FontDescent ;
	if( FontLeading				) *FontLeading				= Font->FontLeading ;
	if( FontUnderlinePosition	) *FontUnderlinePosition	= Font->FontUnderlinePosition ;
	if( FontUnderlineThickness	) *FontUnderlineThickness	= Font->FontUnderlineThickness ;
	if( FontCapHeight			) *FontCapHeight			= Font->FontCapHeight ;
	if( FontXHeight				) *FontXHeight				= Font->FontXHeight ;
}

// フォントデータを下に文字画像のセットアップ
int SetupDxiOSFontImage( void *FontData, const char *Chara )
{
	DXLIB_IOS_FONTINFO *Font = ( DXLIB_IOS_FONTINFO * )FontData ;
    float f ;
	char AddA_Chara[ 64 ] ;

	CL_strcpy( DX_CHARCODEFORMAT_UTF8, AddA_Chara, Chara ) ;
	CL_strcat( DX_CHARCODEFORMAT_UTF8, AddA_Chara, " A" ) ;

	// 文字列を作成する
	CFStringRef      CharaCFStr = CFStringCreateWithBytes( kCFAllocatorDefault, ( const UInt8 * )     Chara, CL_strlen( DX_CHARCODEFORMAT_UTF8,      Chara ), kCFStringEncodingUTF8, FALSE ) ;
	CFStringRef AddA_CharaCFStr = CFStringCreateWithBytes( kCFAllocatorDefault, ( const UInt8 * )AddA_Chara, CL_strlen( DX_CHARCODEFORMAT_UTF8, AddA_Chara ), kCFStringEncodingUTF8, FALSE ) ;
//	CFStringRef CharaCFStr = CFStringCreateWithBytes( kCFAllocatorDefault, ( const UInt8 * )"あA", CL_strlen( DX_CHARCODEFORMAT_UTF8, "Aあ" ), kCFStringEncodingUTF8, FALSE ) ;
    CFMutableAttributedStringRef      AttrString = CFAttributedStringCreateMutable( kCFAllocatorDefault, 0 ) ;
    CFMutableAttributedStringRef AddA_AttrString = CFAttributedStringCreateMutable( kCFAllocatorDefault, 0 ) ;
	CFAttributedStringReplaceString(      AttrString, CFRangeMake( 0, 0 ),      CharaCFStr ) ;
	CFAttributedStringReplaceString( AddA_AttrString, CFRangeMake( 0, 0 ), AddA_CharaCFStr ) ;

	// フォントを設定
	CFAttributedStringSetAttribute(      AttrString, CFRangeMake( 0, CFStringGetLength(      CharaCFStr ) ), kCTFontAttributeName, Font->Font ) ;
	CFAttributedStringSetAttribute( AddA_AttrString, CFRangeMake( 0, CFStringGetLength( AddA_CharaCFStr ) ), kCTFontAttributeName, Font->Font ) ;

    // 色を設定
    CGFloat components[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    CGColorRef color = CGColorCreate( Font->ColorSpace, components ) ;
//  CFAttributedStringSetAttribute(      AttrString, CFRangeMake( 0, CFStringGetLength(      CharaCFStr ) ), kCTForegroundColorAttributeName, color ) ;
    CFAttributedStringSetAttribute( AddA_AttrString, CFRangeMake( 0, CFStringGetLength( AddA_CharaCFStr ) ), kCTForegroundColorAttributeName, color ) ;
    CGColorRelease( color ) ;

	// CTFramesetterを使ってCTFrameを生成
    CTFramesetterRef framesetter = CTFramesetterCreateWithAttributedString( AddA_AttrString ) ;
    CTFrameRef frame = CTFramesetterCreateFrame( framesetter, CFRangeMake( 0, 0 ), Font->RectPath, NULL ) ;

	// サイズの取得
//	CGSize contentSize = CTFramesetterSuggestFrameSizeWithConstraints( framesetter, CFRange(), nil, CGSizeMake(CGFLOAT_MAX, CGFLOAT_MAX), nil ) ;

    // ビットマップをクリア
    _MEMSET( Font->ImageData, 0, Font->ImagePitch * Font->ImageHeight ) ;

    // ビットマップコンテキストにCTFrameを描画
    CTFrameDraw( frame, Font->BitmapContext ) ;

	//文字列の行(Line)情報取得
	CTLineRef line = CTLineCreateWithAttributedString( AttrString ) ;

	//文字列の行(Line)の幅
	CGRect rect = CTLineGetBoundsWithOptions( line, kCTLineBoundsUseGlyphPathBounds ) ;

    f = CGRectGetMaxX( rect ) ;
    Font->ImageAddX  = ( int )f ;
    if( f - Font->ImageAddX  > 0 )
    {
        Font->ImageAddX  += 1 ;
    }

    f = CGRectGetWidth( rect ) ;
    Font->ImageSizeX = ( int )f + 2 ;
    if( Font->ImageSizeX > Font->ImageWidth )
    {
        Font->ImageSizeX = Font->ImageWidth ;
    }
    
	f = CGRectGetHeight( rect ) ;
//	f = contentSize.height ;
    Font->ImageSizeY = ( int )f + 2 ;
    if( Font->ImageSizeY > Font->ImageHeight )
    {
        Font->ImageSizeY = Font->ImageHeight ;
    }
    
    f = CGRectGetMinX( rect ) ;
    Font->ImageDrawX = ( int )f ;
	if( Font->ImageDrawX < 0 )
	{
		Font->ImageDrawX = 0 ;
	}
    if( f - Font->ImageDrawX > 0 && Font->ImageWidth > Font->ImageSizeX )
    {
        Font->ImageSizeX += 1 ;
        Font->ImageAddX += 1 ;
    }
    
    f = CGRectGetMinY( rect ) ;
//    Font->ImageDrawY = ( int )f ;
    Font->ImageDrawY = ( int )0 ;
	if( Font->ImageDrawY < 0 )
	{
		Font->ImageDrawY = 0 ;
	}
    if( f - Font->ImageDrawY > 0 && Font->ImageHeight > Font->ImageSizeY )
    {
        Font->ImageSizeY += 1 ;
    }

	// 行の情報解放
	CFRelease( line ) ;

    // 文字列の削除
    CFRelease(      AttrString ) ;
    CFRelease( AddA_AttrString ) ;

	// 終了
	return 0 ;
}

// 文字画像の情報を取得
void * GetDxiOSFontImage( void *FontData, int *ImageDrawX, int *ImageDrawY, int *ImageSizeX, int *ImageSizeY, int *ImagePitch, int *ImageAddX )
{
	DXLIB_IOS_FONTINFO *Font = ( DXLIB_IOS_FONTINFO * )FontData ;

	if( ImageDrawX ) *ImageDrawX = Font->ImageDrawX ;
	if( ImageDrawY ) *ImageDrawY = Font->ImageDrawY ;
	if( ImageSizeX ) *ImageSizeX = Font->ImageSizeX ;
	if( ImageSizeY ) *ImageSizeY = Font->ImageSizeY ;
	if( ImagePitch ) *ImagePitch = Font->ImagePitch ;
	if( ImageAddX  ) *ImageAddX  = Font->ImageAddX ;

	return Font->ImageData ;
}

static NSString *ConvertTCHARtoNSString( const TCHAR *Source )
{
	NSString *ResultString ;
#ifdef UNICODE
//	ResultString = [ [ NSString alloc ] initWithCString:( char * )Source encoding:NSUTF16LittleEndianStringEncoding ] ;
	ResultString = [ NSString stringWithCString:( char * )Source encoding:NSUTF16LittleEndianStringEncoding ] ;
#else // UNICODE
	if( CHAR_CHARCODEFORMAT == DX_CHARCODEFORMAT_UTF8 )
	{
//		ResultString = [ [ NSString alloc ] initWithCString:Source encoding:NSUTF8StringEncoding ] ;
		ResultString = [ NSString stringWithCString:Source encoding:NSUTF8StringEncoding ] ;
	}
	else
	{
		int CharCodeFormat ;
		char StringBuffer[ TCHAR_TO_UTF8_TEMPSTRINGLENGTH ] ;
		char *AllocStringBuffer = NULL ;
		char *UseStringBuffer ;
		size_t StrLength ;

		CharCodeFormat = CHAR_CHARCODEFORMAT ;
		StrLength = CL_strlen( CharCodeFormat, Source ) ;
		if( 16 * ( StrLength + 1 ) > TCHAR_TO_UTF8_TEMPSTRINGLENGTH - 1 )
		{
			AllocStringBuffer = ( char * )DXALLOC( 16 * ( StrLength + 1 ) ) ;
			if( AllocStringBuffer == NULL )
			{
				return NULL ;
			}
			UseStringBuffer = AllocStringBuffer ;
		}
		else
		{
			UseStringBuffer = StringBuffer ;
		}

		ConvString( ( const char * )Source, StrLength, CharCodeFormat, ( char * )UseStringBuffer, 16 * ( StrLength + 1 ), DX_CHARCODEFORMAT_UTF8 ) ;

//		ResultString = [ [ NSString alloc ] initWithCString:UseStringBuffer encoding:NSUTF8StringEncoding ] ;
		ResultString = [ NSString stringWithCString:UseStringBuffer encoding:NSUTF8StringEncoding ] ;

		if( AllocStringBuffer != NULL )
		{
			DXFREE( AllocStringBuffer ) ;
			AllocStringBuffer = NULL ;
		}
	}
#endif // UNICODE
	
	return ResultString ;
}

TCHAR *ConvertNSStringtoTCHAR( NSString *Source )
{
	TCHAR *ResultString ;
	char *UTF8String = ( char * )[ Source UTF8String ] ;
	size_t UTF8StringLength = CL_strlen( DX_CHARCODEFORMAT_UTF8, UTF8String ) ;
#ifdef UNICODE
	ResultString = ( TCHAR * )DXALLOC( ( UTF8StringLength + 1 ) * 16 ) ;
	if( ResultString == NULL )
	{
		return NULL ;
	}

	ConvString( ( const char * )UTF8String, UTF8StringLength, DX_CHARCODEFORMAT_UTF8, ( char * )ResultString, ( UTF8StringLength + 1 ) * 16, WCHAR_T_CHARCODEFORMAT ) ;
#else // UNICODE
	if( CHAR_CHARCODEFORMAT == DX_CHARCODEFORMAT_UTF8 )
	{
		ResultString = ( TCHAR * )DXALLOC( UTF8StringLength + 1 ) ;
		if( ResultString == NULL )
		{
			return NULL ;
		}
		CL_strcpy( DX_CHARCODEFORMAT_UTF8, ResultString, UTF8String ) ;
	}
	else
	{
		ResultString = ( TCHAR * )DXALLOC( ( UTF8StringLength + 1 ) * 16 ) ;
		if( ResultString == NULL )
		{
			return NULL ;
		}

		ConvString( ( const char * )UTF8String, UTF8StringLength, DX_CHARCODEFORMAT_UTF8, ( char * )ResultString, ( UTF8StringLength + 1 ) * 16, CHAR_CHARCODEFORMAT ) ;
	}
#endif // UNICODE
	
	return ResultString ;
}

void ConvertNSStringtoTCHAR_NoAlloc( NSString *Source, TCHAR *DestBuffer, size_t DestBufferBytes )
{
	TCHAR *ResultString ;
	char *UTF8String = ( char * )[ Source UTF8String ] ;
	size_t UTF8StringLength = CL_strlen( DX_CHARCODEFORMAT_UTF8, UTF8String ) ;

#ifdef UNICODE

	ConvString( ( const char * )UTF8String, UTF8StringLength, DX_CHARCODEFORMAT_UTF8, ( char * )DestBuffer, DestBufferBytes, WCHAR_T_CHARCODEFORMAT ) ;

#else // UNICODE
	if( CHAR_CHARCODEFORMAT == DX_CHARCODEFORMAT_UTF8 )
	{
		CL_strcpy_s( DX_CHARCODEFORMAT_UTF8, DestBuffer, DestBufferBytes, UTF8String ) ;
	}
	else
	{
		ConvString( ( const char * )UTF8String, UTF8StringLength, DX_CHARCODEFORMAT_UTF8, ( char * )DestBuffer, DestBufferBytes, CHAR_CHARCODEFORMAT ) ;
	}
#endif // UNICODE
}

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// ディスプレイの解像度を取得する
int GetDisplayResolution_iOS( int *SizeX, int *SizeY )
{
	if( SizeX != NULL ) *SizeX = g_iOSSysObjC.ViewWidth ;
	if( SizeY != NULL ) *SizeY = g_iOSSysObjC.ViewHeight ;

	return 0 ;
}

// 端末に設定されている言語を取得する( 戻り値　-1：エラー　0以上：言語名文字列の格納に必要なバイト数 )
int GetLanguage_iOS( TCHAR *StringBuffer, int StringBufferSize )
{
	NSString *LanguageNSStr = [ [ NSLocale currentLocale ] objectForKey:NSLocaleLanguageCode ] ;
	TCHAR *LanguageCharStr = ConvertNSStringtoTCHAR( LanguageNSStr ) ;
	int Bytes ;

	if( LanguageCharStr == NULL )
	{
		return -1 ;
	}

	Bytes = ( CL_strlen( _TCHARCODEFORMAT, LanguageCharStr ) + 1 ) * GetCharCodeFormatUnitSize( _TCHARCODEFORMAT ) ;

	if( StringBuffer != NULL )
	{
		CL_strcpy_s( _TCHARCODEFORMAT, StringBuffer, StringBufferSize, LanguageCharStr ) ;
	}

	DXFREE( LanguageCharStr ) ;

	return Bytes ;
}

// 端末に設定されている国を取得する( 戻り値　-1：エラー　0以上：国名文字列の格納に必要なバイト数 )
int GetCountry_iOS( TCHAR *StringBuffer, int StringBufferSize )
{
	NSString *CountryNSStr = [ [ NSLocale currentLocale ] objectForKey:NSLocaleCountryCode ] ;
	TCHAR *CountryCharStr = ConvertNSStringtoTCHAR( CountryNSStr ) ;
	int Bytes ;

	if( CountryCharStr == NULL )
	{
		return -1 ;
	}

	Bytes = ( CL_strlen( _TCHARCODEFORMAT, CountryCharStr ) + 1 ) * GetCharCodeFormatUnitSize( _TCHARCODEFORMAT ) ;

	if( StringBuffer != NULL )
	{
		CL_strcpy_s( _TCHARCODEFORMAT, StringBuffer, StringBufferSize, CountryCharStr ) ;
	}

	DXFREE( CountryCharStr ) ;

	return Bytes ;
}

// 文字列入力ダイアログを出す
// StringBuffer : 入力された文字列を代入するバッファの先頭アドレス
// StringBufferBytes : 入力された文字列を代入するバッファのサイズ( 単位：バイト )
// Title : ダイアログのタイトル
// Message : ダイアログのメッセージ
// IsBlock : 入力が完了するまで関数から出てこないかどうか( 1:入力が完了するまで関数から出てこない  0:ダイアログを表示したら関数から出てくる( 入力完了のチェックは GetStateInputStringDialogBox で行う ) )
// 戻り値 : 0=成功  -1=エラー発生
int StartInputStringDialogBox( TCHAR *StringBuffer, size_t StringBufferBytes, const TCHAR *Title, const TCHAR *Message, int IsBlock )
{
	if( g_iOSSysObjC.TextInputAlertRunFlag || g_iOSSysObjC.TextInputAlertRunRequestFlag )
	{
		return -1 ;
	}

	g_iOSSysObjC.TextInputAlertRunRequestFlag = TRUE ;
	g_iOSSysObjC.TextInputAlertRunFlag = FALSE ;
	g_iOSSysObjC.TextInputAlertCancelFlag = FALSE ;

	g_iOSSysObjC.TextInputResultStringBuffer = StringBuffer ;
	g_iOSSysObjC.TextInputResultStringBufferBytes = StringBufferBytes ;

	g_iOSSysObjC.TextInputAlertTitle = ConvertTCHARtoNSString( Title ) ;
	g_iOSSysObjC.TextInputAlertMessage = ConvertTCHARtoNSString( Message ) ;

	if( IsBlock )
	{
		while( g_iOSSysObjC.TextInputAlertRunFlag || g_iOSSysObjC.TextInputAlertRunRequestFlag )
		{
			usleep( 16000 ) ;
		}
	}

	// 終了
	return 0 ;
}

// 文字列入力ダイアログの処理が終わったかどうかを取得する
// 戻り値： 0=入力中   1=入力完了  2=キャンセルされた
int GetStateInputStringDialogBox( void )
{
	if( g_iOSSysObjC.TextInputAlertRunFlag || g_iOSSysObjC.TextInputAlertRunRequestFlag )
	{
		return 0 ;
	}
	
	return g_iOSSysObjC.TextInputAlertCancelFlag ? 2 : 1 ;
}

// モーションセンサーの情報を取得する
int GetDeviceMotionInfo_iOS( DEVICEMOTIONINFO_IOS *DeviceMotionInfo )
{
	if( DeviceMotionInfo != NULL )
	{
		*DeviceMotionInfo = *( ( DEVICEMOTIONINFO_IOS * )&g_iOSSysObjC.MotionInfo ) ;
	}

	return 0 ;
}

// 振動を再生する
// VibrationType : 振動の種類( IOS_VIBRATE_ONE_STRONG_LONG など )
int PlayVibration_iOS( int VibrationType )
{
	AudioServicesPlaySystemSound( VibrationType ) ;
    
    return 0 ;
}

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE



