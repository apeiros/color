extern VALUE rb_color_cmyk__allocate(VALUE class);
extern VALUE rb_color_cmyk_initialize(int argc, VALUE *argv, VALUE self);
extern VALUE rb_color_cmyk_initialize_copy(VALUE self, VALUE original);
extern VALUE rb_color_cmyk_cyan(VALUE self);
extern VALUE rb_color_cmyk_magenta(VALUE self);
extern VALUE rb_color_cmyk_yellow(VALUE self);
extern VALUE rb_color_cmyk_key(VALUE self);
extern VALUE rb_color_cmyk_alpha(VALUE self);
extern VALUE rb_color_cmyk_add(VALUE self, VALUE other);
extern VALUE rb_color_cmyk_sub(VALUE self, VALUE other);
extern VALUE rb_color_cmyk_distance(VALUE self, VALUE other);
extern VALUE rb_color_cmyk_eql(VALUE self, VALUE other);
extern VALUE rb_color_cmyk_hash(VALUE self);
extern VALUE rb_color_cmyk_to_gray(VALUE self);
extern VALUE rb_color_cmyk_to_rgb(VALUE self);