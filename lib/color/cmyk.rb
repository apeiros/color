require 'color'

module Color # :nodoc:

	# == Synopsis
	#   cmyk   = Color::CMYK.new(245,0,118,10)
	#   as_rgb = cmyk.to_rgb
	#   cyan   = cmyk.cyan
	#
	# == Description
	# CMYK (short for cyan, magenta, yellow, and key (Black)) is a subtractive
	# color model. In this way the mixture of ideal CMY colors is subtractive
	# (cyan, magenta, and yellow printed together on white result in black).
	# The CMYK model works by partially, or entirely masking, certain colors
	# on the typically white background. White is the combination of all colors
	# in the RGB-model, while in the CMYK model it's combining no colors.
	# --from http://en.wikipedia.org/wiki/CMYK_color_model
	#
	# All compontent values are integers between 0 and 255, the alpha value
	# represents the transparency, where 0 means opaque and 255 fully transparent.
	#
	class CMYK
		include Common

		class <<self
			# used to load with Marshal.load
			def _load(marshalled) # :nodoc:
				new(*marshalled.unpack("C5"))
			end

			# === Synopsis
			#   Color::CMYK.from(Color::RGB.new(255,255,255)) # => <CMYK: 0C, 0M, 0Y, 0K, 0a>
			#
			# === Description
			# Coerces +value+ to CMYK.
			# 
			def from(value)
				value.to_cmyk
			end

			# === Synopsis
			#   Color::CMYK.floats(0, 0.25, 0.5, 0.75, 1) # => <CMYK: 0, 64, 128, 192, 255>
			#
			# === Description
			# Create a CMYK color from float values. Counterpart to Color::CMYK#to_a(true).
			# 
			def floats(cyan, magenta, yellow, key, alpha=0)
				new((cyan*255).round, (magenta*255).round, (yellow*255).round, (key*255).round, (alpha*255).round)
			end			
		end
		
		# The cyan portion of this color. A value between 0 and 255.
		attr_reader :cyan
		
		# The magenta portion of this color. A value between 0 and 255.
		attr_reader :magenta

		# The yellow portion of this color. A value between 0 and 255.
		attr_reader :yellow

		# The key (black) portion of this color. A value between 0 and 255.
		attr_reader :key

		# The transparency of this color. A value between 0 and 255, where
		# 0 means opaque and 255 fully transparent.
		attr_reader :alpha
		alias black key

		# === Synopsis
		#   Color::CMYK.new(cyan, magenta, yellow, key[, alpha])
		#
		# === Description
		# Create a new CMYK instance. cyan, magenta, yellow, key and alpha are
		# Integers within 0 and 255 each, where for alpha 0 means
		# opaque and 255 fully transparent.
		#
		def initialize(cyan, magenta, yellow, key, alpha=0)
			@cyan    = cyan.round
			@magenta = magenta.round
			@yellow  = yellow.round
			@key     = key.round
			@alpha   = alpha.round
			
			unless [cyan, magenta, yellow, key, alpha].all? { |v| v.between?(0,255) }
				raise ArgumentError, "Value must be between 0 and 255"
			end
		end

		def to_rgb # :nodoc:
			key     = @key/255.0
			cyan    = 1-(@cyan/255.0*(1-key)+key)
			magenta = 1-(@magenta/255.0*(1-key)+key)
			yellow  = 1-(@yellow/255.0*(1-key)+key)
			RGB.new(cyan*255, magenta*255, yellow*255, @alpha)
		end

		# === Synopsis
		#   cmyk.to_a # => array
		#
		# === Description
		# Returns all values in an array. If +as_floats+ is true, the values
		# are converted to float values between 0 and 1.
		#
		def to_a(as_floats=false)
			as_floats ? 
				[cyan/255.0, magenta/255.0, yellow/255.0, key/255.0, alpha/255.0] :
				[cyan, magenta, yellow, key, alpha]
		end
		
		# === Synopsis
		#   cmyk.to_hash # => hash
		#
		# === Description
		# Returns all values in a hash with keys :cyan, :magenta, :yellow, :key
		# and :alpha. If +as_floats+ is true, the values are converted to float
		# values between 0 and 1.
		#
		def to_hash(as_floats=false)
			as_floats ? 
				{
					:cyan    => cyan/255.0,
					:magenta => magenta/255.0,
					:yellow  => yellow/255.0,
					:key     => key/255.0,
					:alpha   => alpha/255.0
				} :
				{
					:cyan    => cyan,
					:magenta => magenta,
					:yellow  => yellow,
					:key     => key,
					:alpha   => alpha
				}
		end
		
		# === Synopsis
		#    cmyk.to_s # => string
		# 
		# === Description
		# Returns a String representation of this color.
		#
		def to_s
			"CMYK: %dC, %dM, %dY, %dK, %da" %  [cyan, magenta, yellow, key, alpha]
		end

		def to_cmyk # :nodoc:
			dup
		end
		
		# Used with Marshal.dump to create a dump of this color.
		def _dump(*a) # :nodoc:
			[cyan, magenta, yellow, key, alpha].pack("C5")
		end
	end
end