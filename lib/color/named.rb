require 'color'

module Color # :nodoc:

	# == Description
	# Named colors and their values
	#
	class Named
		include Common

		class <<self
			# used to load with Marshal.load
			def _load(marshalled) # :nodoc:
				new(name.to_sym)
			end

			# === Synopsis
			#   Color::Named.from(Color::RGB.new(255,255,255)) # => <Named: White, 0>
			#
			# === Description
			# Coerces +value+ to Named.
			# 
			def from(value)
				value.to_named
			end

			# Uses Color::RGB::floats.
			def floats(*values)
				RGB.floats(*values).to_named
			end
		end

		include Common
		
		attr_reader :name
		def initialize(name)
			@name = name
		end
		
		# === Synopsis
		#    named.to_s # => string
		# 
		# === Description
		# Returns a String representation of this color.
		#
		def to_s
			@name.dup
		end
		
		# === Synopsis
		#   named.to_a # => array
		#
		# === Description
		# Returns all values of the rgb representation in an array.
		# If +as_floats+ is true, the values are converted to float values
		# between 0 and 1.
		#
		def to_a(*a)
			to_rgb.to_a
		end
		
		# === Synopsis
		#   named.to_hash # => hash
		#
		# === Description
		# Returns the hash of the rgb representation, see Color::RGB#to_hash.
		#
		def to_hash(as_floats=false)
			to_rgb.to_hash(as_floats)
		end
		
		def to_rgb # :nodoc:
			Names[@name].dup
		end
		
		# Used with Marshal.dump to create a dump of this color.
		def _dump # :nodoc:
			@name.to_s
		end

		# list of color-names and their RGB representation
		Names = {
			'Alice blue'                   => 0xF0F8FF,
			'Alizarin Crimson'             => 0xE32636,
			'Amaranth'                     => 0xE52B50,
			'Amber'                        => 0xFFBF00,
			'Amethyst'                     => 0x9966CC,
			'Apricot'                      => 0xFBCEB1,
			'Aqua'                         => 0x00FFFF,
			'Aquamarine'                   => 0x7FFFD4,
			'Asparagus'                    => 0x7BA05B,
			'Azure'                        => 0x007FFF,
			'Baby blue'                    => 0xE0FFFF,
			'Beige'                        => 0xF5F5DC,
			'Bistre'                       => 0x3D2B1F,
			'Black'                        => 0x000000,
			'Blue'                         => 0x0000FF,
			'Bondi blue'                   => 0x0095B6,
			'Bright green'                 => 0x66FF00,
			'Bright turquoise'             => 0x08E8DE,
			'Brown'                        => 0x964B00,
			'Buff'                         => 0xF0DC82,
			'Burgundy'                     => 0x900020,
			'Burnt orange'                 => 0xCC5500,
			'Burnt sienna'                 => 0xE97451,
			'Burnt umber'                  => 0x8A3324,
			'Camouflage green'             => 0x78866B,
			'Cardinal'                     => 0xC41E3A,
			'Carmine'                      => 0x960018,
			'Carnation'                    => 0xF95A61,
			'Carrot orange'                => 0xED9121,
			'Celadon'                      => 0xACE1AF,
			'Cerise'                       => 0xDE3163,
			'Cerulean'                     => 0x007BA7,
			'Cerulean blue'                => 0x2A52BE,
			'Chartreuse'                   => 0x7FFF00,
			'Chartreuse yellow'            => 0xDFFF00,
			'Chestnut'                     => 0xCD5C5C,
			'Chocolate'                    => 0xD2691E,
			'Cinnamon'                     => 0x7B3F00,
			'Cobalt'                       => 0x0047AB,
			'Copper'                       => 0xB87333,
			'Copper rose'                  => 0x996666,
			'Coral'                        => 0xFF7F50,
			'Coral Red'                    => 0xFF4040,
			'Corn'                         => 0xFBEC5D,
			'Cornflower blue'              => 0x6495ED,
			'Cream'                        => 0xFFFDD0,
			'Crimson'                      => 0xDC143C,
			'Cyan'                         => 0x00FFFF,
			'Dark blue'                    => 0x0000C8,
			'Denim'                        => 0x1560BD,
			'Dodger blue'                  => 0x1E90FF,
			'Emerald'                      => 0x50C878,
			'Eggplant'                     => 0x990066,
			'Falu red'                     => 0x801818,
			'Fern green'                   => 0x4F7942,
			'Flax'                         => 0xEEDC82,
			'Forest green'                 => 0x228B22,
			'French Rose'                  => 0xF64A8A,
			'Fuchsia'                      => 0xFF00FF,
			'Gamboge'                      => 0xE49B0F,
			'Gold'                         => 0xFFD700,
			'Goldenrod'                    => 0xDAA520,
			'Grey'                         => 0x808080,
			'Grey-asparagus'               => 0x465945,
			'Green'                        => 0x00FF00,
			'Green-yellow'                 => 0xADFF2F,
			'Harlequin'                    => 0x3FFF00,
			'Heliotrope'                   => 0xDF73FF,
			'Hollywood Cerise'             => 0xF400A1,
			'Hot Magenta'                  => 0xFF00CC,
			'Hot Pink'                     => 0xFF69B4,
			'Indigo'                       => 0x4B0082,
			'International Klein Blue'     => 0x002FA7,
			'International orange'         => 0xFF4F00,
			'Ivory'                        => 0xFFFFF0,
			'Jade'                         => 0x00A86B,
			'Khaki'                        => 0xC3B091,
			'Khaki (X11)'                  => 0xF0E68C,
			'Lavender'                     => 0xB57EDC,
			'Lavender blue'                => 0xCCCCFF,
			'Lavender blush'               => 0xFFF0F5,
			'Lavender grey'                => 0xBDBBD7,
			'Lavender magenta'             => 0xEE82EE,
			'Lavender pink'                => 0xFBAED2,
			'Lavender purple'              => 0x967BB6,
			'Lavender rose'                => 0xFBA0E3,
			'Lemon'                        => 0xFDE910,
			'Lemon chiffon'                => 0xFFFACD,
			'Lilac'                        => 0xC8A2C8,
			'Lime'                         => 0xBFFF00,
			'Linen'                        => 0xFAF0E6,
			'Magenta'                      => 0xFF00FF,
			'Malachite'                    => 0x0BDA51,
			'Maroon'                       => 0x800000,
			'Mauve'                        => 0xE0B0FF,
			'Medium carmine'               => 0xAF4035,
			'Medium Purple'                => 0x9370DB,
			'Midnight Blue'                => 0x003366,
			'Mint Green'                   => 0x98FF98,
			'Moss green'                   => 0xADDFAD,
			'Mountbatten pink'             => 0x997A8D,
			'Mustard'                      => 0xFFDB58,
			'Navajo white'                 => 0xFFDEAD,
			'Navy Blue'                    => 0x000080,
			'Ochre'                        => 0xCC7722,
			'Old Gold'                     => 0xCFB53B,
			'Old Lace'                     => 0xFDF5E6,
			'Old Lavender'                 => 0x796878,
			'Old Rose'                     => 0xC08081,
			'Olive'                        => 0x808000,
			'Olive Drab'                   => 0x6B8E23,
			'Orange (color wheel)'         => 0xFF7F00,
			'Orange (web)'                 => 0xFFA500,
			'Orange Peel'                  => 0xFFA000,
			'Orchid'                       => 0xDA70D6,
			'Papaya whip'                  => 0xFFEFD5,
			'Pastel green'                 => 0x77DD77,
			'Pastel pink'                  => 0xFFD1DC,
			'Peach'                        => 0xFFE5B4,
			'Peach-orange'                 => 0xFFCC99,
			'Peach-yellow'                 => 0xFADFAD,
			'Pear'                         => 0xD1E231,
			'Periwinkle'                   => 0xCCCCFF,
			'Persian blue'                 => 0x1C39BB,
			'Persian green'                => 0x00A693,
			'Persian indigo'               => 0x32127A,
			'Persian pink'                 => 0xF77FBE,
			'Persian red'                  => 0xCC3333,
			'Persian rose'                 => 0xFE28A2,
			'Pine Green'                   => 0x01796F,
			'Pink'                         => 0xFFC0CB,
			'Pink-orange'                  => 0xFF9966,
			'Pomegranate'                  => 0xF34723,
			'Powder blue (web)'            => 0xB0E0E6,
			'Puce'                         => 0xCC8899,
			'Prussian blue'                => 0x003153,
			'Pumpkin'                      => 0xFF7518,
			'Purple'                       => 0x660099,
			'Raw umber'                    => 0x734A12,
			'Red'                          => 0xFF0000,
			'Red-violet'                   => 0xC71585,
			'Robin egg blue'               => 0x00CCCC,
			'Rose'                         => 0xFF007F,
			'Royal Blue'                   => 0x4169E1,
			'Russet'                       => 0x80461B,
			'Rust'                         => 0xB7410E,
			'Safety Orange (Blaze Orange)' => 0xFF6600,
			'Saffron'                      => 0xF4C430,
			'Sapphire'                     => 0x082567,
			'Salmon'                       => 0xFF8C69,
			'Sandy brown'                  => 0xF4A460,
			'Sangria'                      => 0x92000A,
			'Scarlet'                      => 0xFF2400,
			'School bus yellow'            => 0xFFD800,
			'Sea Green'                    => 0x2E8B57,
			'Seashell'                     => 0xFFF5EE,
			'Selective yellow'             => 0xFFBA00,
			'Sepia'                        => 0x704214,
			'Shocking Pink'                => 0xFC0FC0,
			'Silver'                       => 0xC0C0C0,
			'Slate grey'                   => 0x708090,
			'Smalt (Dark powder blue)'     => 0x003399,
			'Spring Green'                 => 0x00FF7F,
			'Steel blue'                   => 0x4682B4,
			'Swamp green'                  => 0xACB78E,
			'Tan'                          => 0xD2B48C,
			'Tangerine'                    => 0xFFCC00,
			'Taupe'                        => 0x483C32,
			'Tea Green'                    => 0xD0F0C0,
			'Teal'                         => 0x008080,
			'Tenné (Tawny)'                => 0xCD5700,
			'Terra cotta'                  => 0xE2725B,
			'Thistle'                      => 0xD8BFD8,
			'Turquoise'                    => 0x30D5C8,
			'Ultramarine'                  => 0x120A8F,
			'Vermilion'                    => 0xFF4D00,
			'Violet'                       => 0x8B00FF,
			'Violet (web)'                 => 0xEE82EE,
			'Violet-eggplant'              => 0x991199,
			'Viridian'                     => 0x40826D,
			'Wheat'                        => 0xF5DEB3,
			'White'                        => 0xFFFFFF,
			'Wisteria'                     => 0xC9A0DC,
			'Yellow'                       => 0xFFFF00,
			'Zinnwaldite'                  => 0xEBC2AF,
		}
		Names.each { |key,value| Names[key] = RGB.from_int(value) }
		Values = Names.invert
	end
end
