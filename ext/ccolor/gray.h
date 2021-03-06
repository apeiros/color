extern VALUE rb_color_gray__allocate(VALUE class);
extern VALUE rb_color_gray_initialize(int argc, VALUE *argv, VALUE self);
extern VALUE rb_color_gray_initialize_copy(VALUE self, VALUE original);
extern VALUE rb_color_gray_white(VALUE self);
extern VALUE rb_color_gray_alpha(VALUE self);
extern VALUE rb_color_gray_add(VALUE self, VALUE other);
extern VALUE rb_color_gray_sub(VALUE self, VALUE other);
extern VALUE rb_color_gray_distance(VALUE self, VALUE other);
extern VALUE rb_color_gray_eql(VALUE self, VALUE other);
extern VALUE rb_color_gray_hash(VALUE self);
extern VALUE rb_color_gray_to_i(int argc, VALUE *argv, VALUE self);
extern VALUE rb_color_gray_to_cmyk(VALUE self);
extern VALUE rb_color_gray_to_rgb(VALUE self);
