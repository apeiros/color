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
rb_color_gray__allocate(VALUE class)
{
	cGray *color;
	VALUE rb_color = Data_Make_Struct(class, cGray, NULL, free, color);
	color->white = 0;
	color->alpha = 0;
	return rb_color;
}

/*
 *  call-seq:
 *     Color::Gray.new(white[, alpha])
 *
 *  Create a new Gray instance. White and alpha are Integers
 *  within 0 and 255 each, where for white 0 means black and
 *  255 means white, for alpha 0 means opaque and 255 fully
 *  transparent.
 */
extern VALUE
rb_color_gray_initialize(int argc, VALUE *argv, VALUE self)
{
	cGray *color;
	Data_Get_Struct(self, cGray, color);
	VALUE white, alpha;
	rb_scan_args(argc, argv, "11", &white, &alpha);

	int   w,a;
	w = (NUM2INT(white));
	a = (NIL_P(alpha) ? 0 : NUM2INT(alpha));
	
	if (0 > w || w > 255) {
		rb_raise(rb_eArgError, "Invalid value for white, must be between 0 and 255");
	}
	if (0 > a || a > 255) {
		rb_raise(rb_eArgError, "Invalid value for alpha, must be between 0 and 255");
	}

	color->white = w;
	color->alpha = a;

	return self;
}

/*
 * :nodoc:
 */
extern VALUE
rb_color_gray_initialize_copy(VALUE self, VALUE original)
{
	cGray *color1, *color2;
	Data_Get_Struct(self, cGray, color1);
	Data_Get_Struct(original, cGray, color2);
	*color1 = *color2;
	return self;
}

/*
 *  call-seq:
 *     rgb.white -> fixnum
 *     rgb.value -> fixnum
 *
 *  The white portion of this color. A value between 0 and 255, where
 *  0 means black and 255 white.
 */
extern VALUE
rb_color_gray_white(VALUE self)
{
	cGray *color;
	Data_Get_Struct(self, cGray, color);
	return CHR2FIX(color->white);
}

/*
 *  call-seq:
 *     rgb.alpha -> fixnum
 *
 *  The transparency of this color. A value between 0 and 255, where
 *  0 means opaque and 255 fully transparent.
 */
extern VALUE
rb_color_gray_alpha(VALUE self)
{
	cGray *color;
	Data_Get_Struct(self, cGray, color);
	return CHR2FIX(color->alpha);
}

/*
 *  call-seq:
 *     gray1 + gray2 -> gray
 *
 *  Component wise sum.
 */
extern VALUE
rb_color_gray_add(VALUE self, VALUE other)
{
	cGray *color1, *color2, *color3;
	Data_Get_Struct(self, cGray, color1);
	Data_Get_Struct(other, cGray, color2);
	VALUE rb_color = Data_Make_Struct(rb_cGray, cGray, NULL, free, color3);
	color3->white = color_cap(CHR2LONG(color1->white) + CHR2LONG(color2->white), 0, 255);
	color3->alpha = color_cap(CHR2LONG(color1->alpha) + CHR2LONG(color2->alpha), 0, 255);
	return rb_color;
}

/*
 *  call-seq:
 *     gray1 - gray2 -> gray
 *
 *  Component wise difference.
 */
extern VALUE
rb_color_gray_sub(VALUE self, VALUE other)
{
	cGray *color1, *color2, *color3;
	Data_Get_Struct(self, cGray, color1);
	Data_Get_Struct(other, cGray, color2);
	VALUE rb_color = Data_Make_Struct(rb_cGray, cGray, NULL, free, color3);
	color3->white = color_cap(CHR2LONG(color1->white) - CHR2LONG(color2->white), 0, 255);
	color3->alpha = color_cap(CHR2LONG(color1->alpha) - CHR2LONG(color2->alpha), 0, 255);
	return rb_color;
}

/*
 *  call-seq:
 *     gray.to_i -> integer
 *
 *  An integer representation of the RGB value in the form of
 *  0xaaww. The alpha value is optional.
 *  Also see Gray::from_int
 */
extern VALUE
rb_color_gray_to_i(int argc, VALUE *argv, VALUE self)
{
	cGray *color;
	Data_Get_Struct(self, cGray, color);
	VALUE alpha;
	rb_scan_args(argc, argv, "01", &alpha);

	if RTEST(alpha) {
		return UINT2NUM(
			(CHR2LONG(color->alpha) << 8) |
			(CHR2LONG(color->white))
		);
	} else {
		return UINT2NUM(CHR2LONG(color->white));
	}
}

/*
 *  call-seq:
 *     gray.to_cmyk -> cmyk
 *
 *  Returns a CMYK representation of this color.
 */
extern VALUE
rb_color_gray_to_cmyk(VALUE self)
{
	cGray *gray;
	cCMYK *cmyk;
	Data_Get_Struct(self, cGray, gray);
	VALUE rb_color = Data_Make_Struct(rb_cCMYK, cCMYK, NULL, free, cmyk);
	color_convert_gray_to_cmyk(gray, cmyk);
	return rb_color;
}

/*
 *  call-seq:
 *     gray.to_rgb -> rgb
 *
 *  Returns a RGB representation of this color.
 */
extern VALUE
rb_color_gray_to_rgb(VALUE self)
{
	cGray *gray;
	cRGB *rgb;
	Data_Get_Struct(self, cGray, gray);
	VALUE rb_color = Data_Make_Struct(rb_cRGB, cRGB, NULL, free, rgb);
	color_convert_gray_to_rgb(gray, rgb);
	return rb_color;
}

/*
 *  call-seq:
 *     gray.distance(other) -> float
 *
 *  Returns the distance to another color of the same class.
 *  Distance is a Float between 0 and 1, where 1 is the maximum
 *  distance. Be aware that this is purely mathematical and human
 *  perception may differ.
 */
extern VALUE
rb_color_gray_distance(VALUE self, VALUE other)
{
	cGray *color1, *color2;
	Data_Get_Struct(self, cGray, color1);
	Data_Get_Struct(other, cGray, color2);
	return rb_float_new(sqrtf((
		powf(CHR2FLOAT(color1->white) - CHR2FLOAT(color2->white), 2) +
		powf(CHR2FLOAT(color1->alpha) - CHR2FLOAT(color2->alpha), 2)
	)/2));
}

/*
 *  call-seq:
 *     rgb.eql?(other) -> true/false
 *
 *  Compares two Gray instances for equality. Two Gray instances are eql? if
 *  their white and alpha values are equal.
 */
extern VALUE
rb_color_gray_eql(VALUE self, VALUE other)
{
	if (CLASS_OF(self) != CLASS_OF(other)) {
		return Qfalse;
	}
	cGray *color1, *color2;
	Data_Get_Struct(self, cGray, color1);
	Data_Get_Struct(other, cGray, color2);
	return (
		color1->white == color2->white &&
		color1->alpha == color2->alpha
	) ? Qtrue : Qfalse;
}

/*
 *  call-seq:
 *     rgb.hash   -> fixnum
 *
 *  Compute a hash-code for this color. Two colors with the same components
 *  will have the same hash code (and will compare using <code>eql?</code>).
 */
extern VALUE
rb_color_gray_hash(VALUE self)
{
	cGray *color;
	Data_Get_Struct(self, cGray, color);

	return INT2FIX(
		(8) ^
		(CHR2LONG(color->alpha) << 15) ^
		(CHR2LONG(color->white << 7))
	);
}
