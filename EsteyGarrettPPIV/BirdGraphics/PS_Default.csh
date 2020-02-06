#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 10.1
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_POSITION              0   xyzw        0      POS   float       
// NORMAL                   0   xyz         1     NONE   float       
// TEXCOORD                 0   xy          2     NONE   float       
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_TARGET                0   xyzw        0   TARGET   float   xyzw
//
//
// Level9 shader bytecode:
//
    ps_2_x
    def c0, 1, 1, 0, 1
    mov oC0, c0

// approximately 1 instruction slot used
ps_4_0
dcl_output o0.xyzw
mov o0.xyzw, l(1.000000,1.000000,0,1.000000)
ret 
// Approximately 2 instruction slots used
#endif

const BYTE PS_Default[] =
{
     68,  88,  66,  67,  17, 190, 
    219,  79,  29, 207, 132, 248, 
     82,  32, 229,   7,  27, 237, 
    119, 204,   1,   0,   0,   0, 
     64,   2,   0,   0,   6,   0, 
      0,   0,  56,   0,   0,   0, 
    144,   0,   0,   0, 208,   0, 
      0,   0,  76,   1,   0,   0, 
    152,   1,   0,   0,  12,   2, 
      0,   0,  65, 111, 110,  57, 
     80,   0,   0,   0,  80,   0, 
      0,   0,   0,   2, 255, 255, 
     44,   0,   0,   0,  36,   0, 
      0,   0,   0,   0,  36,   0, 
      0,   0,  36,   0,   0,   0, 
     36,   0,   0,   0,  36,   0, 
      0,   0,  36,   0,   1,   2, 
    255, 255,  81,   0,   0,   5, 
      0,   0,  15, 160,   0,   0, 
    128,  63,   0,   0, 128,  63, 
      0,   0,   0,   0,   0,   0, 
    128,  63,   1,   0,   0,   2, 
      0,   8,  15, 128,   0,   0, 
    228, 160, 255, 255,   0,   0, 
     83,  72,  68,  82,  56,   0, 
      0,   0,  64,   0,   0,   0, 
     14,   0,   0,   0, 101,   0, 
      0,   3, 242,  32,  16,   0, 
      0,   0,   0,   0,  54,   0, 
      0,   8, 242,  32,  16,   0, 
      0,   0,   0,   0,   2,  64, 
      0,   0,   0,   0, 128,  63, 
      0,   0, 128,  63,   0,   0, 
      0,   0,   0,   0, 128,  63, 
     62,   0,   0,   1,  83,  84, 
     65,  84, 116,   0,   0,   0, 
      2,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  82,  68,  69,  70, 
     68,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  28,   0, 
      0,   0,   0,   4, 255, 255, 
      0,   1,   0,   0,  28,   0, 
      0,   0,  77, 105,  99, 114, 
    111, 115, 111, 102, 116,  32, 
     40,  82,  41,  32,  72,  76, 
     83,  76,  32,  83, 104,  97, 
    100, 101, 114,  32,  67, 111, 
    109, 112, 105, 108, 101, 114, 
     32,  49,  48,  46,  49,   0, 
     73,  83,  71,  78, 108,   0, 
      0,   0,   3,   0,   0,   0, 
      8,   0,   0,   0,  80,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
     15,   0,   0,   0,  92,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
      7,   0,   0,   0,  99,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   2,   0,   0,   0, 
      3,   0,   0,   0,  83,  86, 
     95,  80,  79,  83,  73,  84, 
     73,  79,  78,   0,  78,  79, 
     82,  77,  65,  76,   0,  84, 
     69,  88,  67,  79,  79,  82, 
     68,   0,  79,  83,  71,  78, 
     44,   0,   0,   0,   1,   0, 
      0,   0,   8,   0,   0,   0, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,  15,   0,   0,   0, 
     83,  86,  95,  84,  65,  82, 
     71,  69,  84,   0, 171, 171
};
