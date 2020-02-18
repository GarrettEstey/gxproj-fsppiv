#include "ShaderHeader.fx"

// the output stream must contain clip space verticies (if sent to the rasterizer)
struct GSOutput
{
	float4 clr : COLOR;
	float4 posH : SV_POSITION;
};
// you may not exceed 1024 scalars. the max number of scalars output from this shader is 24
[maxvertexcount(3)]
// this operation builds each point into a blade of grass 
void main(point PS_INPUT input[1], inout TriangleStream<GSOutput> output)
{
	float halfBW = 0.5;
	GSOutput simple[3];
	// colors
	simple[0].clr = float4(0.1f,0.6f,0.1f,1);
	simple[1].clr = float4(0,1,0,1);
	simple[2].clr = simple[0].clr;
	// positions
	simple[0].posH = float4(input[0].wPos.xyz,1);
	simple[1].posH = simple[0].posH;
	simple[2].posH = simple[0].posH;
	// shift to make triangle
	simple[0].posH.x -= halfBW;
	simple[1].posH.y += 2.0f;
	simple[2].posH.x += halfBW;
	// move to projection space
	for(uint i = 0; i < 3; ++i)
	{
		simple[i].posH = mul(simple[i].posH, View);
        simple[i].posH = mul(simple[i].posH, Projection);
		output.Append(simple[i]);
	}
}