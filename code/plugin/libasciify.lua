local ffi = require("ffi")

local dirname = string.sub(debug.getinfo(1).source, 2, #"/libasciify.lua" * -1)
local library_path = (function()
	local is_on_windows = package.config:sub(1, 1) == "\\"
	return dirname .. (is_on_windows and "/libasciify.dll" or "/libasciify.so")
end)()
local native = ffi.load(library_path)

---@class ffilib
---@field NULL ffi.cdata*
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

AsciiArt *ascii_art_create(ImageOfColor const *const image, Size const frame_size_chars, Size const char_size);
void ascii_art_delete(AsciiArt const *const self);

typedef struct AsciiArtOutput { char const *const cstr; size_t const len;} AsciiArtOutput;
AsciiArtOutput ascii_art_write(AsciiArt const *const self, Size const frame_size_chars, AsciiArtWriterColorMode const mode);

void ascii_art_apply_fill(AsciiArt *const self, FillAsciiFilter const *const fill);
void ascii_art_apply_color(AsciiArt *const self, ColorAsciiFilter const *const color);
void ascii_art_apply_edge(AsciiArt *const self, EdgeAsciiFilter const *const edge);
]])

---------
-- oop --
---------

---@param Base ?table
---@return table
local function class(Base)
	local Self = {}
	Self.__index = Self

	if Base then
		setmetatable(Self, Base)
	else
		---@param a table|nil
		---@return table|nil
		function Self._constructor(a)
			return a or {}
		end
	end

	---A function to construct a new object. Should NOT be overloaded.
	---@param a table|nil
	---@return table|nil
	function Self.new(a)
		local self = Self._constructor(a)
		return self and setmetatable(self, Self) or nil
	end

	return Self
end

------------
-- module --
------------

local M = {}

local Native = class()

---@param a { _self:ffi.cdata*, _delete:function }
---@return table|nil
function Native._constructor(a)
	if not a._self or a._self == ffi.NULL then
		return nil
	end

	ffi.gc(a._self, a._delete)

	return a
end

-- fill filter

local FillFilter = class(Native)

---@param a { palette:string }
---@return table|nil
function FillFilter._constructor(a)
	return Native._constructor({
		_self = native.fill_filter_create(a.palette),
		_delete = native.fill_filter_delete,
		palette = a.palette,
	})
end

-- color filter

local ColorFilter = class(Native)

---@return table|nil
function ColorFilter._constructor()
	return Native._constructor({
		_self = native.color_filter_create(),
		_delete = native.color_filter_delete,
	})
end

-- edge filter

local EdgeFilter = class(Native)

---@param a { threshold:number, palette:string, dog?:{ eps:number, p:number }}
---@return table|nil
function EdgeFilter._constructor(a)
	local _self
	if a.dog then
		_self = native.edge_filter_create_extra(a.threshold, a.palette, a.dog.eps, a.dog.p)
	else
		_self = native.edge_filter_create(a.threshold, a.palette)
	end

	return Native._constructor({
		_self = _self,
		_delete = native.edge_filter_delete,
		threshold = a.threshold,
		palette = a.palette,
		dog = a.dog,
	})
end

-- image

local Image = class(Native)

---@param a { path:string }
---@return table|nil
function Image._constructor(a)
	return Native._constructor({
		_self = native.image_decode(a.path),
		_delete = native.image_delete,
		path = a.path,
	})
end

-- ascii art

local AsciiArt = class(Native)

---@alias Size { w:integer, h:integer }

---@enum AsciiArt.ColorMode
AsciiArt.ColorMode = { EMPTY = 0, INDEXED = 1, TRUE = 2 }

---@param a { image:table, frame_size:Size, char_size:Size }
---@return table|nil
function AsciiArt._constructor(a)
	return Native._constructor({
		_self = native.ascii_art_create(a.image._self, a.frame_size, a.char_size),
		_delete = native.ascii_art_delete,
		image = a.image,
		char_size = a.char_size,
	})
end

---@param frame_size Size
---@param mode AsciiArt.ColorMode
---@return string|string[]
function AsciiArt:write(frame_size, mode)
	local native_out = native.ascii_art_write(self._self, frame_size, mode)
	local out = ffi.string(native_out.cstr, native_out.len)
	ffi.C.free(ffi.cast("void *", native_out.cstr))

	return out
end

---@param fill table
function AsciiArt:apply_fill(fill)
	return native.ascii_art_apply_fill(self._self, fill._self)
end
---@param color table
function AsciiArt:apply_color(color)
	return native.ascii_art_apply_color(self._self, color._self)
end
---@param edge table
function AsciiArt:apply_edge(edge)
	return native.ascii_art_apply_edge(self._self, edge._self)
end

M.FillFilter = FillFilter
M.ColorFilter = ColorFilter
M.EdgeFilter = EdgeFilter
M.Image = Image
M.AsciiArt = AsciiArt

return M
