require 'color'

module Color # :nodoc:

	# == Synopsis
	#   silver = Color::Gray.new(192)
	#   silver.white # => 192
	#   silver.black # => 63
	#
	# == Description
	# Grayscale or monochromous color space.
	#
	class Gray
		include Common

		class <<self
			# used to load with Marshal.load
			def _load(marshalled) # :nodoc:
				new(*marshalled.unpack("C2"))
			end

			# === Synopsis
			#   Color::Gray.from(Color::RGB.new(255,255,255)) # => <Gray: 255 0a>
			#
			# === Description
			# Coerces +value+ to Color::Gray.
			# 
			def from(value)
				value.to_gray
			end
			
			# === Synopsis
			#   Color::Gray.floats(0.5, 0.75) # => <Gray: 128 192a>
			#
			# === Description
			# Create a Gray color from float values. Counterpart to Color::Gray#to_a(true).
			# 
			def floats(white, alpha=0)
				new((white*255).round, (alpha*255).round)
			end			
		end
		
		# The white portion of this color. A value between 0 and 255.
		attr_reader :white

		# The transparency of this color. A value between 0 and 255, where
		# 0 means opaque and 255 fully transparent.
		attr_reader :alpha
		
		alias value white

		# === Synopsis
		#   Color::Gray.new(white[, alpha])
		#
		# Create a new Gray instance. +White+ and +alpha+ are Integers
		# within 0 and 255 each, where for white 0 means black and
		# 255 means white, for alpha 0 means opaque and 255 fully
		# transparent.
		#
		def initialize(white, alpha=0)
			@white = white.round
			@alpha = alpha.round

			unless [white, alpha].all? { |v| v.between?(0,255) }
				raise ArgumentError, "Value must be between 0 and 255"
			end
		end
		
		# Inverse of white
		def black
			255-white
		end

		# === Synopsis
		#   gray.to_a # => array
		#
		# === Description
		# Returns all values in an array. If +as_floats+ is true, the values
		# are converted to float values between 0 and 1.
		#
		def to_a(as_floats=false)
			as_floats ?
				[white/255.0, alpha/255.0] :
				[white, alpha]
		end

		# === Synopsis
		#   gray.to_hash # => hash
		#
		# === Description
		# Returns all values in a hash with keys :white and :alpha.
		# If +as_floats+ is true, the values are converted to float
		# values between 0 and 1.
		#
		def to_hash
			as_floats ?
				{ :white => white/255.0, :alpha => alpha/255.0 } :
				{ :white => white, :alpha => alpha }
		end

		# === Synopsis
		#    gray.to_s # => string
		# 
		# === Description
		# Returns a String representation of this color.
		#
		def to_s
			"Gray: %d %da" % [white, alpha]
		end

		def to_gray
			dup
		end

		# Used with Marshal.dump to create a dump of this color.
		def _dump(*a) # :nodoc:
			[white, alpha].pack("C2")
		end
	end
end