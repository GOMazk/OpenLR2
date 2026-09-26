
void AlphaTest( lowp float inAlpha )
{
#ifdef ALPHATEST_NEVER			// FALSE
	discard ;
#endif
#ifdef ALPHATEST_LESS			// lColor.a <  uATestRef
	if( inAlpha >= uATestRef )
	{
		discard ;
	}
#endif
#ifdef ALPHATEST_EQUAL			// inAlpha == uATestRef
	if( inAlpha != uATestRef )
	{
		discard ;
	}
#endif
#ifdef ALPHATEST_LESSEQUAL		// inAlpha <= uATestRef
	if( inAlpha > uATestRef )
	{
		discard ;
	}
#endif
#ifdef ALPHATEST_GREATER		// inAlpha >  uATestRef
	if( inAlpha <= uATestRef )
	{
		discard ;
	}
#endif
#ifdef ALPHATEST_NOTEQUAL		// inAlpha != uATestRef
	if( inAlpha == uATestRef )
	{
		discard ;
	}
#endif
#ifdef ALPHATEST_GREATEREQUAL	// inAlpha >= uATestRef
	if( inAlpha < uATestRef )
	{
		discard ;
	}
#endif
#ifdef ALPHATEST_ALWAYS			// TRUE
#endif
#ifdef ALPHATEST_MIX // 使用しない箇所はコメントアウト
//	// DX_CMP_NEVER
//	if( uATestCmpMode == 1 )
//	{
//		discard ;
//	}
//	else
	// DX_CMP_LESS
	if( uATestCmpMode == 2 )
	{
		if( inAlpha >= uATestRef )
		{
			discard ;
		}
	}
	else
	// DX_CMP_EQUAL
	if( uATestCmpMode == 3 )
	{
		if( inAlpha != uATestRef )
		{
			discard ;
		}
	}
	else
	// DX_CMP_LESSEQUAL
	if( uATestCmpMode == 4 )
	{
		if( inAlpha >  uATestRef )
		{
			discard ;
		}
	}
	else
	// DX_CMP_GREATER
	if( uATestCmpMode == 5 )
	{
		if( inAlpha <= uATestRef )
		{
			discard ;
		}
	}
	else
	// DX_CMP_NOTEQUAL
	if( uATestCmpMode == 6 )
	{
		if( inAlpha == uATestRef )
		{
			discard ;
		}
	}
	else
	// DX_CMP_GREATEREQUAL
	if( uATestCmpMode == 7 )
	{
		if( inAlpha <  uATestRef )
		{
			discard ;
		}
	}
//	else
//	// DX_CMP_ALWAYS
//	if( uATestCmpMode == 8 )
//	{
//	}
#endif // ALPHATEST_MIX
}
