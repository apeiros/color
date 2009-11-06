require 'color'

module Color

	# == Description
	# Common methods of color classes. Most classes representing a color model
	# mix this module in.
	#
	module Common

		# === Synopsis
		#   gray(0).sequence(gray(20), 5).map { |e| e.white } # => [0, 4, 8, 12, 16, 20]
		#
		# === Description
		# Creates an array of colors, linearly approaching the color 'to'.
		# steps defines how many colors are created.
		#
		def sequence(to, steps)
			raise ArgumentError, "Steps must be bigger or equal 1" unless steps >= 1
			l = 1.0/(steps)
			Array.new(steps+1) { |i| interpolate(to, i*l) }
		end
		
		# === Synopsis
		#   gray(0).interpolate(gray(100), 0.3).white # => 30
		#
		# === Description
		# Interpolate a color between self and other, use
		# pos (0..1) to define where between self and other
		# the color should be. 0 is equal self, 1 equal other.
		def interpolate(other, pos=0.5)
			raise ArgumentError, "Position must be between 0 and 1" unless pos.between?(0,1)
			self.class.new(*to_a.zip(coerce(other).to_a).map { |a,b|
				(a+(b-a)*pos).round
			})
		end
		
		# === Synopsis
		#   gray(0,0).distance(gray(255,255)) # => 1
		#
		# === Description
		# A purely mathematical distance of two colors, does not correspond with
		# humans perception.
		# The value returned is a float between 0 and 1, where 0 would be equal and 1
		# maximum distance.
		#
		def distance(to)
			to  = coerce(to).to_a(true)
			sum = 0
			arr = to_a(true)
			to.zip(arr) { |a,b|
				sum += ((a-b)/255.0)**2
			}
			sum / arr.length
		end

		# === Synopsis
		#   gray(0).blend(gray(100, 10)).map { |e| e.white } # => [0, 4, 8, 12, 16, 20]
		#
		# === Description
		# Blend two colors, creating a new one. Can be used
		# to simulate layers of semi-transparent colors and
		# calculate the resulting color.
		# If +with_alpha+ is supplied, the alpha value of the
		# Color to blend +with+ is ignored and instead the
		# supplied is used
		# Some color models also offer other methods than
		# the default :interpolate
		#
		def blend(with, with_alpha=nil, using=:interpolate)
			with_alpha ||= with.alpha
			opacity = (255-with_alpha)/255.0
			value   = proc { |v,a| v+a-a*v }
			with    = coerce(with)
			case using
				when :interpolate
					values = to_a[0..-2].zip(with.to_a).map { |a,b|
						(a+(b-a)*opacity).round
					}+[alpha]
					self.class.new(*values)
				else
					raise ArgumentError, "Unknown mode, #{using}"
			end
		end
		
		def eql?(other) # :nodoc:
			other.class == self.class && other.to_a.eql?(self.to_a)
		end
		alias == eql?  # :nodoc:
		
		def hash # :nodoc:
			to_a.hash
		end

		# === Synopsis
		#   coerced = somecolor.coerce(othercolor)
		#   coerced.class == somecolor.class # => true
		#
		# === Description
		# Coerce a color to the same class as self.
		#
		def coerce(obj)
			self.class.from(obj)
		end
		
		# === Synopsis
		#   gray(0).sequence(gray(20), 5).map { |e| e.white } # => [0, 4, 8, 12, 16, 20]
		#
		# === Description
		# The complement of a color.
		#
		def complement
			coerce(to_hsv.complement)
		end

		# === Synopsis
		#   gray(50).closest([gray(20), gray(60), gray(45)]).white # => 45
		#
		# === Description
		# Returns the color with the minimal distance to self.
		# Please regard the note respecting perceived distance in
		# Color::Common#distance.
		#
		def closest(out_of)
			out_of.map { |c| [c, distance(c)] }.min { |a,b| a.last <=> b.last }.first
		end
		
		# === Synopsis
		#   rgb(255,0,100).with(nil,50,nil,255) == rgb(255,50,100,255)
		#
		# === Description
		# Create a new color with some values changed. Arguments are
		# the same as the class' new method. Values set to nil are taken
		# from self unchanged.
		#
		def with(*args)
			self.class.new(*to_a.zip(args).map { |a,b| b||a })
		end
		
		# === Synopsis
		#   semitransparent = somecolor.with_alpha(128)
		#
		# === Description
		# Create a new color from self with a different alpha value.
		#
		def with_alpha(a)
			self.class.new(*to_a[0..-2].push(a))
		end
		
		# === Synopsis
		#   somecolor + othercolor # => newcolor
		#
		# === Description
		# Coerces othercolor and returns a color with the
		# component wise sum.
		#
		def +(other)
			other = coerce(other)
			self.class.floats(*to_a.zip(other.to_a).map { |a,b| a+b })
		end

		# === Synopsis
		#   somecolor - othercolor # => newcolor
		#
		# === Description
		# Coerces othercolor and returns a color with the
		# component wise difference.
		#
		def -(other)
			other = coerce(other)
			self.class.floats(*to_a.zip(other.to_a).map { |a,b| a-b })
		end
		
		# === Synopsis
		#   somecolor.to_cmyk # => Color::CMYK color
		# 
		# === Description
		# Returns a Color::CMYK representation of this color.
		#
		def to_cmyk
			to_rgb.to_cmyk
		end

		# === Synopsis
		#   somecolor.to_gray # => Color::Gray color
		# 
		# === Description
		# Returns a Color::Gray representation of this color.
		#
		def to_gray
			to_rgb.to_gray
		end
		
		# === Synopsis
		#   somecolor.to_hsl # => Color::HSL color
		# 
		# === Description
		# Returns a Color::HSL representation of this color.
		#
		def to_hsl
			to_rgb.to_hsl
		end

		# === Synopsis
		#   somecolor.to_hsv # => Color::HSV color
		# 
		# === Description
		# Returns a Color::HSV representation of this color.
		#
		def to_hsv
			to_rgb.to_hsv
		end

		# === Synopsis
		#   somecolor.to_html      # => html color string
		#   rgb(255,127,0).to_html # => "#FF7F00"
		# 
		# === Description
		# Returns a String containing the html-hex representation of this color.
		#
		def to_html
			to_rgb.to_html
		end

		# === Synopsis
		#   somecolor.to_term # => Color::Term color
		# 
		# === Description
		# Returns a Color::Term representation of this color.
		#
		def to_term
			to_rgb.to_term
		end

		# === Synopsis
		#   somecolor.to_mixer # => Color::Mixer instance
		# 
		# === Description
		# Creates a Color::Mixer instance with self.
		#
		def to_mixer
			Mixer.new(self)
		end
		
		def inspect # :nodoc:
			"<#{self}>"
		end		
	end
end