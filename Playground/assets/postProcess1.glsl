#include <fxaa>


vec4 color(){	
	
	return vec4(
		FXAA(16.0, 1.0/12.0, 1.0/128.0, UVs, u_FinalColorSampler, u_TargetSize),
		1.0
	);
	
	//return vec4(FinalColor,1);
}