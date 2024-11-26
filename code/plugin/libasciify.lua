local ffi = require("ffi")

local dirname = string.sub(debug.getinfo(1).source, 2, #"/libasciify.lua" * -1)
local library_path = (function()
	if package.config:sub(1, 1) == "\\" then
		return dirname .. "/libasciify.dll"
	else
		return dirname .. "/libasciify.so"
	end
end)()
local native = ffi.load(library_path)
ffi.cdef([[
void free(void *ptr);

// fill
typedef struct FillAsciiFilter FillAsciiFilter;
FillAsciiFilter const *fill_filter_create(char const *const palette);
void fill_filter_delete(FillAsciiFilter const *const self);

// color
typedef struct ColorAsciiFilter ColorAsciiFilter;
ColorAsciiFilter const *color_filter_create() ;
void color_filter_delete(ColorAsciiFilter const *const self);

// edge
typedef struct EdgeAsciiFilter EdgeAsciiFilter;
EdgeAsciiFilter const *edge_filter_create(float const threshold, char const *const palette);
EdgeAsciiFilter const *edge_filter_create_extra(
    float const threshold,
    char const *const palette,
    float const dog_eps,
    float const dog_p
);
void edge_filter_delete(EdgeAsciiFilter const *const self);

// image
typedef struct ImageOfColor ImageOfColor;
ImageOfColor const *image_decode(char const *const path);
void image_delete(ImageOfColor const *const self);

// ascii art

typedef struct AsciiArt AsciiArt;
typedef struct Size { uint16_t w, h; } Size;
typedef enum AsciiArtWriterColorMode {COLOR_MODE_EMPTY, COLOR_MODE_INDEXED, COLOR_MODE_TRUE} AsciiArtWriterColorMode;

AsciiArt *ascii_art_create(ImageOfColor const *const image, Size const char_size);
void ascii_art_delete(AsciiArt const *const self);

typedef struct AsciiArtOutput { char const *const cstr; size_t const len;} AsciiArtOutput;
AsciiArtOutput ascii_art_write(AsciiArt const *const self, AsciiArtWriterColorMode const mode);

void ascii_art_apply_fill(AsciiArt *const self, FillAsciiFilter const *const fill);
void ascii_art_apply_color(AsciiArt *const self, ColorAsciiFilter const *const color);
void ascii_art_apply_edge(AsciiArt *const self, EdgeAsciiFilter const *const edge);
]])

---------
-- oop --
---------

local function class()
	local Self = {}
	Self.__index = Self

	function Self.construct(args)
		return Self.setmetatable(args)
	end
	function Self.setmetatable(args)
		return setmetatable(args, Self)
	end

	return Self
end

local function extends(Base)
	local Self = class()
	Self.__index = Self
	setmetatable(Self, Base)

	function Self.construct(args)
		return Self.setmetatable(Base.construct(args))
	end

	return Self
end

------------
-- module --
------------

local M = {}

local Native = class()
function Native:delete()
	self._delete(self._self)
	self._self = nil
end

-- color filter

local FillFilter = extends(Native)
function FillFilter.new(palette)
	return FillFilter.construct({
		_self = native.fill_filter_create(palette),
		_delete = native.fill_filter_delete,
	})
end

-- color filter

local ColorFilter = extends(Native)
function ColorFilter.new()
	return ColorFilter.construct({
		_self = native.color_filter_create(),
		_delete = native.color_filter_delete,
	})
end

-- edge filter

local EdgeFilter = extends(Native)
function EdgeFilter.new(threshold, palette)
	return EdgeFilter.construct({
		_self = native.edge_filter_create(threshold, palette),
		_delete = native.edge_filter_delete,
	})
end
function EdgeFilter.new_extra(threshold, palette, dog_eps, dog_p)
	return EdgeFilter.construct({
		_self = native.edge_filter_create_extra(threshold, palette, dog_eps, dog_p),
		_delete = native.edge_filter_delete,
	})
end

-- image

local Image = extends(Native)
function Image.decode(path)
	return Image.construct({
		_self = native.image_decode(path),
		_delete = native.image_delete,
	})
end

-- ascii art

local AsciiArt = extends(Native)

function AsciiArt.new(image, char_size)
	return AsciiArt.construct({
		_self = native.ascii_art_create(image._self, char_size),
		_delete = native.ascii_art_delete,
	})
end

function AsciiArt:write(mode)
	local out = native.ascii_art_write(self._self, mode)
	return ffi.string(out.cstr, out.len)
end

function AsciiArt:apply_fill(fill)
	return native.ascii_art_apply_fill(self._self, fill._self)
end
function AsciiArt:apply_color(color)
	return native.ascii_art_apply_color(self._self, color._self)
end
function AsciiArt:apply_edge(edge)
	return native.ascii_art_apply_edge(self._self, edge._self)
end

M.FillFilter = FillFilter
M.ColorFilter = ColorFilter
M.EdgeFilter = EdgeFilter
M.Image = Image
M.AsciiArt = AsciiArt

local scale = 1.25

local image = Image.decode(dirname .. "../test.webp")
local fill = FillFilter.new(" .:+*csS&$@")
local color = ColorFilter.new()
local edge = EdgeFilter.new(4.5, "|\\`~;/")
local art = AsciiArt.new(image, { w = 10 / scale, h = 22 / scale })
art:apply_fill(fill)
art:apply_edge(edge)
art:apply_color(color)
print(art:write(0))
art:delete()
edge:delete()
color:delete()
fill:delete()
image:delete()

return M
