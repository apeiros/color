require 'test/unit'
require 'color'

class TestRGB < Test::Unit::TestCase
	MaxDistance = 0.005

	def setup
	end
	
	def test_initialize
		a = Color::RGB.new(1,2,3,4)
		b = Color::RGB.new(1,2,3,4)
		c = Color::RGB.new(1,2,5,4)
		d = Color::RGB.new(1,2,3,5)
		assert_equal(1, a.red)
		assert_equal(2, a.green)
		assert_equal(3, a.blue)
		assert_equal(4, a.alpha)
		assert_equal(a, b)
		assert_not_equal(a, c)
		assert_not_equal(a, d)
		
		assert_raise(ArgumentError) { Color::RGB.new(-1,0,0) }
		assert_raise(ArgumentError) { Color::RGB.new(0,-1,0) }
		assert_raise(ArgumentError) { Color::RGB.new(0,0,-1) }
		assert_raise(ArgumentError) { Color::RGB.new(256,0,0) }
		assert_raise(ArgumentError) { Color::RGB.new(0,256,0) }
		assert_raise(ArgumentError) { Color::RGB.new(0,0,256) }
	end
	
	def test_conversion
		a = Color::RGB.new(255,100,0)
		b = Color::Gray.new(118)
		assert_in_delta(a.to_hsv.to_rgb.distance(a), 0, MaxDistance)
		assert_in_delta(a.to_hsl.to_rgb.distance(a), 0, MaxDistance)
		assert_in_delta(a.to_cmyk.to_rgb.distance(a), 0, MaxDistance)
		assert_equal(a.to_gray, b)
	end
	
	def test_hashing
		a = Color::RGB.new(255, 255, 255)
		b = Color::RGB.new(255,   0,   0)
		c = Color::RGB.new(255, 255, 255, 128)
		d = Color::RGB.new(255, 255, 255)
		e = Color::RGB.new(128, 255, 255)
		h = { a => 1, b => 2, c => 3, :x => 4 }
		assert_equal(1, h[a])
		assert_equal(2, h[b])
		assert_equal(3, h[c])
		assert_equal(1, h[d])
		assert_equal(nil, h[e])
	end
	
	def test_blending
		a     = Color::RGB.new(255, 255, 255)
		b     = Color::RGB.new(255,   0,   0)
		ab128 = Color::RGB.new(255, 128, 128)
		assert_in_delta(a.blend(b).distance(b), 0, MaxDistance)
		assert_in_delta(a.blend(b, 128).distance(ab128), 0, MaxDistance)
	end
	
	def test_marshalling
		a = Color::RGB.new(117, 243, 21, 93)
		assert_equal(Marshal.load(Marshal.dump(a)), a)
	end
end