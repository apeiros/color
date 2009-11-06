#include <ruby.h>
#include <math.h>
#include "color.h"
#include "tools.h"
#include "rgb.h"
#include "hsv.h"
#include "hsl.h"
#include "cmyk.h"
#include "gray.h"

/* 
 *  :nodoc:
 */
extern VALUE
rb_color_cmyk__allocate(VALUE class)
{
	cCMYK *color;
	VALUE rb_color = Data_Make_Struct(class, cCMYK, NULL, free, color);
	color->c     = 0;
	color->m     = 0;
	color->y     = 0;
	color->k     = 0;
	color->alpha = 0;
	return rb_color;
}

/*
 *  call-seq:
 *     Color::CMYK.new(cyan, magenta, yellow, key[, alpha])
 *
 *  Create a new CMYK instance. cyan, magenta, yellow, key and alpha are
 *  Integers within 0 and 255 each, where for alpha 0 means
 *  opaque and 255 fully transparent.
 */
extern VALUE
rb_color_cmyk_initialize(int argc, VALUE *argv, VALUE self)
{
	cCMYK *color;
	Data_Get_Struct(self, cCMYK, color);
	VALUE cyan, magenta, yellow, key, alpha;
	rb_scan_args(argc, argv, "41", &cyan, &magenta, &yellow, &key, &alpha);

	int   c,m,y,k,a;
	c = (NUM2INT(cyan));
	m = (NUM2INT(magenta));
	y = (NUM2INT(yellow));
	k = (NUM2INT(key));
	a = (NIL_P(alpha) ? 0 : NUM2INT(alpha));
	
	if (0 > c || c > 255) {
		rb_raise(rb_eArgError, "Invalid value for cyan, must be between 0 and 255");
	}
	if (0 > m || m > 255) {
		rb_raise(rb_eArgError, "Invalid value for magenta, must be between 0 and 255");
	}
	if (0 > y || y > 255) {
		rb_raise(rb_eArgError, "Invalid value for yellow, must be between 0 and 255");
	}
	if (0 > k || k > 255) {
		rb_raise(rb_eArgError, "Invalid value for key (black), must be between 0 and 255");
	}
	if (0 > a || a > 255) {
		rb_raise(rb_eArgError, "Invalid value for alpha, must be between 0 and 255");
	}

	color->c     = c;
	color->m     = m;
	color->y     = y;
	color->k     = k;
	color->alpha = a;

	return self;
}

/*
 * :nodoc:
 */
extern VALUE
rb_color_cmyk_initialize_copy(VALUE self, VALUE original)
{
	cCMYK *color1, *color2;
	Data_Get_Struct(self, cCMYK, color1);
	Data_Get_Struct(original, cCMYK, color2);
	*color1 = *color2;
	return self;
}

/*
 *  call-seq:
 *     rgb.cyan -> fixnum
 *
 *  The cyan portion of this color. A value between 0 and 255.
 */
extern VALUE
rb_color_cmyk_cyan(VALUE self)
{
	cCMYK *color;
	Data_Get_Struct(self, cCMYK, color);
	return CHR2FIX(color->c);
}

/*
 *  call-seq:
 *     rgb.magenta -> fixnum
 *
 *  The magenta portion of this color. A value between 0 and 255.
 */
extern VALUE
rb_color_cmyk_magenta(VALUE self)
{
	cCMYK *color;
	Data_Get_Struct(self, cCMYK, color);
	return CHR2FIX(color->m);
}

/*
 *  call-seq:
 *     rgb.yellow -> fixnum
 *
 *  The yellow portion of this color. A value between 0 and 255.
 */
extern VALUE
rb_color_cmyk_yellow(VALUE self)
{
	cCMYK *color;
	Data_Get_Struct(self, cCMYK, color);
	return CHR2FIX(color->y);
}

/*
 *  call-seq:
 *     rgb.key -> fixnum
 *     rgb.black -> fixnum
 *
 *  The key (black) portion of this color. A value between 0 and 255.
 */
extern VALUE
rb_color_cmyk_key(VALUE self)
{
	cCMYK *color;
	Data_Get_Struct(self, cCMYK, color);
	return CHR2FIX(color->k);
}

/*
 *  call-seq:
 *     rgb.alpha -> fixnum
 *
 *  The transparency of this color. A value between 0 and 255, where
 *  0 means opaque and 255 fully transparent.
 */
extern VALUE
rb_color_cmyk_alpha(VALUE self)
{
	cCMYK *color;
	Data_Get_Struct(self, cCMYK, color);
	return CHR2FIX(color->alpha);
}

/*
 *  call-seq:
 *     cmyk1 + cmyk2 -> cmyk
 *
 *  Component wise sum.
 */
extern VALUE
rb_color_cmyk_add(VALUE self, VALUE other)
{
	cCMYK *color1, *color2, *color3;
	Data_Get_Struct(self, cCMYK, color1);
	Data_Get_Struct(other, cCMYK, color2);
	VALUE rb_color = Data_Make_Struct(rb_cCMYK, cCMYK, NULL, free, color3);
	color3->c     = color_cap(CHR2LONG(color1->c) + CHR2LONG(color2->c), 0, 255);
	color3->m     = color_cap(CHR2LONG(color1->m) + CHR2LONG(color2->m), 0, 255);
	color3->y     = color_cap(CHR2LONG(color1->y) + CHR2LONG(color2->y), 0, 255);
	color3->k     = color_cap(CHR2LONG(color1->k) + CHR2LONG(color2->k), 0, 255);
	color3->alpha = color_cap(CHR2LONG(color1->alpha) + CHR2LONG(color2->alpha), 0, 255);
	return rb_color;
}

/*
 *  call-seq:
 *     cmyk1 - cmyk2 -> cmyk
 *
 *  Component wise difference.
 */
extern VALUE
rb_color_cmyk_sub(VALUE self, VALUE other)
{
	cCMYK *color1, *color2, *color3;
	Data_Get_Struct(self, cCMYK, color1);
	Data_Get_Struct(other, cCMYK, color2);
	VALUE rb_color = Data_Make_Struct(rb_cCMYK, cCMYK, NULL, free, color3);
	color3->c     = color_cap(CHR2LONG(color1->c) - CHR2LONG(color2->c), 0, 255);
	color3->m     = color_cap(CHR2LONG(color1->m) - CHR2LONG(color2->m), 0, 255);
	color3->y     = color_cap(CHR2LONG(color1->y) - CHR2LONG(color2->y), 0, 255);
	color3->k     = color_cap(CHR2LONG(color1->k) - CHR2LONG(color2->k), 0, 255);
	color3->alpha = color_cap(CHR2LONG(color1->alpha) - CHR2LONG(color2->alpha), 0, 255);
	return rb_color;
}

/*
 *  call-seq:
 *     rgb.to_rgb -> rgb
 *
 *  Returns an RGB representation of this color.
 */
extern VALUE
rb_color_cmyk_to_rgb(VALUE self)
{
	cCMYK *cmyk;
	cRGB *rgb;
	Data_Get_Struct(self, cCMYK, cmyk);
	VALUE rb_color = Data_Make_Struct(rb_cRGB, cRGB, NULL, free, rgb);
	color_convert_cmyk_to_rgb(cmyk, rgb);
	return rb_color;
}

/*
 *  call-seq:
 *     rgb.to_gray -> gray
 *
 *  Returns a Gray representation of this color.
 */
extern VALUE
rb_color_cmyk_to_gray(VALUE self)
{
	cCMYK *cmyk;
	cGray *gray;
	Data_Get_Struct(self, cCMYK, cmyk);
	VALUE rb_color = Data_Make_Struct(rb_cGray, cGray, NULL, free, gray);
	color_convert_cmyk_to_gray(cmyk, gray);
	return rb_color;
}

/*
 *  call-seq:
 *     cmyk.distance(other) -> float
 *
 *  Returns the distance to another color of the same class.
 *  Distance is a Float between 0 and 1, where 1 is the maximum
 *  distance. Be aware that this is purely mathematical and human
 *  perception may differ.
 */
extern VALUE
rb_color_cmyk_distance(VALUE self, VALUE other)
{
	cCMYK *color1, *color2;
	Data_Get_Struct(self, cCMYK, color1);
	Data_Get_Struct(other, cCMYK, color2);
	return rb_float_new(sqrtf((
		powf(CHR2FLOAT(color1->c) - CHR2FLOAT(color2->c), 2) +
		powf(CHR2FLOAT(color1->m) - CHR2FLOAT(color2->m), 2) +
		powf(CHR2FLOAT(color1->y) - CHR2FLOAT(color2->y), 2) +
		powf(CHR2FLOAT(color1->k) - CHR2FLOAT(color2->k), 2) +
		powf(CHR2FLOAT(color1->alpha) - CHR2FLOAT(color2->alpha), 2)
	)/5));
}

/*
 *  call-seq:
 *     cmyk.eql?(other) -> true/false
 *
 *  Compares two CMYK color instances for equality. Two CMYK colors are eql? if
 *  their cyan, magenta, yellow, key and alpha values are equal.
 */
extern VALUE
rb_color_cmyk_eql(VALUE self, VALUE other)
{
	if (CLASS_OF(self) != CLASS_OF(other)) {
		return Qfalse;
	}
	cCMYK *color1, *color2;
	Data_Get_Struct(self, cCMYK, color1);
	Data_Get_Struct(other, cCMYK, color2);
	return (
		color1->c     == color2->c &&
		color1->m     == color2->m &&
		color1->y     == color2->y &&
		color1->k     == color2->k &&
		color1->alpha == color2->alpha
	) ? Qtrue : Qfalse;
}

/*
 *  call-seq:
 *     cmyk.hash   -> fixnum
 *
 *  Compute a hash-code for this color. Two colors with the same components
 *  will have the same hash code (and will compare using <code>eql?</code>).
 */
extern VALUE
rb_color_cmyk_hash(VALUE self)
{
	cCMYK *color;
	Data_Get_Struct(self, cCMYK, color);

	return INT2FIX(
		(8) ^
		(CHR2LONG(color->alpha) << 26) ^
		(CHR2LONG(color->c) << 21) ^
		(CHR2LONG(color->m) << 16) ^
		(CHR2LONG(color->k) << 11) ^
		(CHR2LONG(color->y << 6))
	);
}
