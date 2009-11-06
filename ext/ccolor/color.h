#define CHR2LONG(x) (long)((x)&0xff)
#define CHR2FLOAT(x) (((float)((x)&0xff))/255)
#define FLOAT2CHR(x) (((unsigned char)(roundf((x)*255)))&0xff)
#define IN_DELTA(x,y) (fabsf((x)-(y)) < 0.0001)

extern VALUE rb_mColor;
extern VALUE rb_cRGB;
extern VALUE rb_cHSV;
extern VALUE rb_cHSL;
extern VALUE rb_cCMYK;
extern VALUE rb_cGray;
extern VALUE rb_cXYZ;

typedef struct _cRGB {
	unsigned char r;     // red
	unsigned char g;     // green
	unsigned char b;     // blue
	unsigned char alpha; // transparency, 0 = opaque, 255 = transparent
} cRGB;

typedef struct _cCMYK {
	unsigned char c;     // cyan
	unsigned char m;     // magenta
	unsigned char y;     // yellow
	unsigned char k;     // key (black)
	unsigned char alpha; // transparency, 0 = opaque, 255 = transparent
} cCMYK;

typedef struct _cHSV {
	float h;             // hue (0...1) (represents 0...360)
	float s;             // saturation (0..1) (represents 0..100)
	float v;             // value (0..1) (represents 0..100)
	unsigned char alpha; // transparency, 0 = opaque, 255 = transparent
} cHSV;

typedef struct _cHSL {
	float h;             // hue (0...1) (represents 0...360)
	float s;             // saturation (0..1) (represents 0..100)
	float l;             // value (0..1) (represents 0..100)
	unsigned char alpha; // transparency, 0 = opaque, 255 = transparent
} cHSL;

typedef struct _cGray {
	unsigned char white; // gray value, 0 = black, 255 = white
	unsigned char alpha; // transparency, 0 = opaque, 255 = transparent
} cGray;

extern VALUE rb_color__native(VALUE class);