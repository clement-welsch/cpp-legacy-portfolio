//***************************************************************************************
// SubMarinViewHelper.fx
//
// Structures and functions for lighting calculations.
//***************************************************************************************

void ComputeSubMarineView(in float4 color, out float4 result)
{
	color.b += 0.2f;
	color.g += 0.1f;

	result = color;
}