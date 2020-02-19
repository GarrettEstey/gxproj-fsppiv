#include "ShaderHeader.fx"
#define POINTCOUNT 6

// the output stream must contain clip space verticies (if sent to the rasterizer)
struct GSOutput
{
	float4 posH : SV_POSITION;
};
// you may not exceed 1024 scalars. the max number of scalars output from this shader is 24
[maxvertexcount(POINTCOUNT)]
// this operation builds each point into a blade of grass 
void main(point PS_INPUT input[1], inout TriangleStream<GSOutput> output)
{
    GSOutput simple[POINTCOUNT];
	// positions
	simple[0].posH = float4(input[0].wPos.xyz,1);
    for (uint i = 1; i < POINTCOUNT; ++i)
    {
        simple[i].posH = simple[0].posH;
    }
    float size = 0.3f;
	// shift to make new shape
    simple[0].posH.x -= size;
    simple[0].posH.z -= size;
    simple[1].posH.x -= size;
    simple[1].posH.z += size;
    simple[2].posH.z += size;
    simple[2].posH.x += size;
    simple[3].posH.x -= size;
    simple[3].posH.z -= size;
    simple[4].posH.x += size;
    simple[4].posH.z -= size;
    simple[5].posH.x += size;
    simple[5].posH.z += size;
	// move to projection space
    for (uint j = 0; j < POINTCOUNT; ++j)
	{
		simple[j].posH = mul(simple[j].posH, View);
        simple[j].posH = mul(simple[j].posH, Projection);
		output.Append(simple[j]);
	}
}