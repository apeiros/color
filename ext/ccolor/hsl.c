#include <ruby.h>
#include <math.h>
#include "color.h"
#include "tools.h"
#include "rgb.h"
#include "hsv.h"
#include "hsl.h"
#include "cmyk.h"
#include "gray.h"

extern VALUE
rb_color_hsl__allocate(VALUE class)
{
	cHSL *color;
	VALUE rb_color = Data_Make_Struct(class, cHSL, NULL, free, color);
	color->h     = 0;
	color->s     = 0;
	color->l     = 0;
	color->alpha = 0;
	return rb_color;
}

/*
 *  call-seq:
 *     Color::HSL.new(hue, saturation, luminance[, alpha])
 *
 *  Create a new HSL instance. Hue, saturation, luminance are floats
 *  between 0 and 1, alpha is an Integer within 0 and 255, where 0
 *  means opaque and 255 fully transparent.
 */
extern VALUE
rb_color_hsl_initialize(int argc, VALUE *argv, VALUE self)
{
	cHSL *color;
	Data_Get_Struct(self, cHSL, color);
	VALUE hue, saturation, luminance, alpha;
	rb_scan_args(argc, argv, "31", &hue, &saturation, &luminance, &alpha);

	double h,s,l;
	int    a;
	h = (NUM2DBL(hue));
	s = (NUM2DBL(saturation));
	l = (NUM2DBL(luminance));
	a = (NIL_P(alpha) ? 0 : NUM2INT(alpha));
	
	if (-1e-6 > h || h > (1+1e-6)) {
		rb_raise(rb_eArgError, "Invalid value for hue, must be between 0 and 1");
	}
	if (-1e-6 > s || s > (1+1e-6)) {
		rb_raise(rb_eArgError, "Invalid value for saturation, must be between 0 and 1");
	}
	if (-1e-6 > l || l > (1+1e-6)) {
		rb_raise(rb_eArgError, "Invalid value for luminance, must be between 0 and 1");
	}
	if (0 > a || a > 255) {
		rb_raise(rb_eArgError, "Invalid value for alpha, must be between 0 and 255");
	}

	color->h     = fmodf(h,1);
	color->s     = color_capf(s,0,1);
	color->l     = color_capf(l,0,1);
	color->alpha = a;

	return self;
}

/*
 * :nodoc:
 */
extern VALUE
rb_color_hsl_initialize_copy(VALUE self, VALUE original)
{
	cHSL *color1, *color2;
	Data_Get_Struct(self, cHSL, color1);
	Data_Get_Struct(original, cHSL, color2);
	*color1 = *color2;
	return self;
}

extern VALUE
rb_color_hsl_hue(VALUE self)
{
	cHSL *color;
	Data_Get_Struct(self, cHSL, color);
	return rb_float_new(color->h);
}

extern VALUE
rb_color_hsl_saturation(VALUE self)
{
	cHSL *color;
	Data_Get_Struct(self, cHSL, color);
	return rb_float_new(color->s);
}

extern VALUE
rb_color_hsl_luminance(VALUE self)
{
	cHSL *color;
	Data_Get_Struct(self, cHSL, color);
	return rb_float_new(color->l);
}

extern VALUE
rb_color_hsl_alpha(VALUE self)
{
	cHSL *color;
	Data_Get_Struct(self, cHSL, color);
	return CHR2FIX(color->alpha);
}

extern VALUE
rb_color_hsl_complement(VALUE self)
{
	cHSL *color1, *color2;
	Data_Get_Struct(self, cHSL, color1);
	VALUE rb_color = Data_Make_Struct(rb_cHSL, cHSL, NULL, free, color2);
	*color2 = *color1;
	color2->h = fmodf(color2->h+0.5, 1);
	return rb_color;
}

extern VALUE
rb_color_hsl_add(VALUE self, VALUE other)
{
	cHSL *color1, *color2, *color3;
	Data_Get_Struct(self, cHSL, color1);
	Data_Get_Struct(other, cHSL, color2);
	VALUE rb_color = Data_Make_Struct(rb_cHSL, cHSL, NULL, free, color3);
	color3->h = fmodf(color1->h + color2->h, 1);
	color3->s = color_capf(color1->s + color2->s, 0, 1);
	color3->l = color_capf(color1->l + color2->l, 0, 1);
	return rb_color;
}

extern VALUE
rb_color_hsl_sub(VALUE self, VALUE other)
{
	cHSL *color1, *color2, *color3;
	Data_Get_Struct(self, cHSL, color1);
	Data_Get_Struct(other, cHSL, color2);
	VALUE rb_color = Data_Make_Struct(rb_cHSL, cHSL, NULL, free, color3);
	color3->h = fmodf(color1->h - color2->h, 1);
	color3->s = color_capf(color1->s - color2->s, 0, 1);
	color3->l = color_capf(color1->l - color2->l, 0, 1);
	return rb_color;
}

/*
 *  call-seq:
 *     hsl.distance(other) -> float
 *
 *  Returns the distance to another color of the same class.
 *  Distance is a Float between 0 and 1, where 1 is the maximum
 *  distance. Be aware that this is purely mathematical and human
 *  perception may differ.
 */
extern VALUE
rb_color_hsl_distance(VALUE self, VALUE other)
{
	cHSL *color1, *color2;
	Data_Get_Struct(self, cHSL, color1);
	Data_Get_Struct(other, cHSL, color2);
	return rb_float_new(sqrtf((
		powf(color1->h - color2->h, 2) +
		powf(color1->s - color2->s, 2) +
		powf(color1->l - color2->l, 2) +
		powf(CHR2FLOAT(color1->alpha) - CHR2FLOAT(color2->alpha), 2)
	)/4));
}

extern VALUE
rb_color_hsl_eql(VALUE self, VALUE other)
{
	cHSL *color1, *color2;
	Data_Get_Struct(self, cHSL, color1);
	Data_Get_Struct(other, cHSL, color2);
	return (
		color1->alpha == color2->alpha &&
		color1->h     == color2->h &&
		color1->s     == color2->s &&
		color1->l     == color2->l
	) ? Qtrue : Qfalse;
}

extern VALUE
rb_color_hsl_hash(VALUE self)
{
	long h;
	cHSL *color;
	Data_Get_Struct(self, cHSL, color);
	
	h  = 4;
	h  = (h << 1) | (h<0 ? 1 : 0);
	h ^= float_hash(color->h);
	h  = (h << 1) | (h<0 ? 1 : 0);
	h ^= float_hash(color->s);
	h  = (h << 1) | (h<0 ? 1 : 0);
	h ^= float_hash(color->l);
	h  = (h << 1) | (h<0 ? 1 : 0);
	h ^= color->alpha;
	return LONG2FIX(h);
}

extern VALUE
rb_color_hsl_to_rgb(VALUE self)
{
	cHSL *hsl;
	cRGB *rgb;
	Data_Get_Struct(self, cHSL, hsl);
	VALUE rb_color = Data_Make_Struct(rb_cRGB, cRGB, NULL, free, rgb);
	color_convert_hsl_to_rgb(hsl, rgb);
	return rb_color;
}

