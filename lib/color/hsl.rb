require 'color'

module Color # :nodoc:

	# == Description
	# HSL color model
	#
	class HSL
		include Common

		class <<self
			# used to load with Marshal.load
			def _load(marshalled) # :nodoc:
				new(*marshalled.unpack("G3C"))
			end

			# === Synopsis
			#   Color::HSL.from(Color::RGB.new(255,255,255)) # => <HSL: 0°h 0%s, 100%l, 0>
			#
			# === Description
			# Coerces +value+ to HSL.
			# 
			def from(value)
				value.to_hsl
			end

			# === Synopsis
			#   Color::HSV.floats(0.5, 0.2, 0.3 0.75) # => <HSL: 180°h 20%s, 30%l, 192>
			#
			# === Description
			# Create a HSL color from float values. Counterpart to Color::HSL#to_a(true).
			# 
			def floats(hue, saturation, lightning, alpha=0)
				new(hue, saturation, lightning, (alpha*255).round)
			end			
		end
		
		# The hue of this color. A value between 0 and 1.
		attr_reader :hue

		# The saturation of this color. A value between 0 and 1.
		attr_reader :saturation

		# The luminance of this color. A value between 0 and 1.
		attr_reader :luminance

		# The transparency of this color. A value between 0 and 255, where
		# 0 means opaque and 255 fully transparent.
		attr_reader :alpha

		# === Synopsis
		#   Color::HSL.new(hue, saturation, luminance[, alpha])
		#
		# === Description
		# Create a new HSV instance. Hue, saturation, luminance are floats
		# between 0 and 1, alpha is an Integer within 0 and 255, where 0
		# means opaque and 255 fully transparent.
		#
		def initialize(hue, saturation, luminance, alpha=0)
			unless [hue, saturation, luminance].all? { |v| 0 <= v && v <= 1 } # between? fails at NaN
				raise ArgumentError, "Invalid Value, hue, saturation and luminance must be between 0 and 1"
			end
			raise ArgumentError, "Invalid alpha, must be between 0 and 255" unless alpha.between?(0,255)

			@hue        = (hue %  1).to_f
			@saturation = saturation.to_f
			@luminance  = luminance.to_f
			@alpha      = alpha.to_i
		end
		
		# === Synopsis
		#    hsl.to_s # => string
		# 
		# === Description
		# Returns a String representation of this color.
		#
		def to_s
			"HSL: %d°h %d%%s, %d%%l, %d" %  [(hue*360).round, (saturation*100).round, (luminance*100).round, alpha]
		end
		
		# === Synopsis
		#   hsl.to_a # => array
		#
		# === Description
		# Returns all values in an array. If +as_floats+ is true, the values
		# are converted to float values between 0 and 1.
		#
		def to_a(as_floats=false)
			[hue, saturation, luminance, as_floats ? alpha/255.0 : alpha]
		end
		
		# === Synopsis
		#   hsl.to_hash # => hash
		#
		# === Description
		# Returns all values in a hash with keys :hue, :saturation, :luminance and :alpha.
		# If +as_floats+ is true, the values are converted to float
		# values between 0 and 1.
		#
		def to_hash(as_floats=false)
			{ :hue => hue, :saturation => saturation, :luminance => luminance, :alpha => as_floats ? alpha/255.0 : alpha }
		end

		def to_rgb # :nodoc:
			if @saturation.in_delta(0) then
				v = @luminance*255
				RGB.new(v, v, v, @alpha)
			else
				co_var1 = (@luminance < 0.5) ? @luminance * (1.0+@saturation) : (@luminance + @saturation) - (@luminance * @saturation)
				co_var2 = 2*@luminance - co_var1
				red     = hue_to_rgb(co_var1, co_var2, (@hue + 1.0/3))
				green   = hue_to_rgb(co_var1, co_var2, @hue)
				blue    = hue_to_rgb(co_var1, co_var2, (@hue - 1.0/3))
				RGB.new(red, green, blue, @alpha)
			end
		end
		
		def to_hsl # :nodoc:
			dup
		end

		# Used with Marshal.dump to create a dump of this color.
		def _dump(*a) # :nodoc:
			[hue, saturation, luminance, alpha].pack("G3C")
		end

		protected
		# required in conversion to rgb
		def hue_to_rgb(co_var1, co_var2, hue) #:nodoc:
			hue += 1 if hue < 0
			hue -= 1 if hue > 1
			case
				when (6*hue < 1): ((co_var1-co_var2)*6*hue+co_var2)
				when (2*hue < 1): co_var1
				when (3*hue < 2): (co_var2+(co_var1-co_var2)*(2.0/3-hue)*6)
				else co_var2
			end * 255
		end
	end
end
