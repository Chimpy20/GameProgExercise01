#pragma once

#ifdef _DEBUG
_Check_return_ _CRT_JIT_INTRINSIC inline double fabs( _In_ double _Xx )
{
	if( _Xx < 0.0 )
	{
		_Xx = -_Xx;
	}

	return _Xx;
}
#endif // _DEBUG
