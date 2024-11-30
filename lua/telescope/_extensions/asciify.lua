local from_entry = require("telescope.from_entry")
local conf = require("telescope.config").values
local defaulter = require("telescope.utils").make_default_callable
local previewers = require("telescope.previewers.buffer_previewer")
local Path = require("plenary.path")

local libasciify = require("asciify.libasciify")

---@type table|nil
local fill
---@type table|nil
local edge
---@type table|nil
local color

---@type Ascii.ColorMode
local color_mode

---@type string[]
local order

local asciify_previewer = defaulter(function(opts)
	opts = opts or {}
	local cwd = opts.cwd or vim.loop.cwd()
	return previewers.new_buffer_previewer({
		title = "File Preview",
		dyn_title = function(_, entry)
			return Path:new(from_entry.path(entry, false, false)):normalize(cwd)
		end,

		get_buffer_by_name = function(_, entry)
			return from_entry.path(entry, false, false)
		end,

		define_preview = function(self, entry)
			local path = from_entry.path(entry, false, false)
			if not path or path == "" then
				return
			end

			---@param lines string[]
			local function preview_error(lines)
				return vim.api.nvim_buf_set_lines(self.state.bufnr, 0, -1, false, lines)
			end

			local bitmap = libasciify.Bitmap.new({ path = path })
			if not bitmap then
				return conf.buffer_previewer_maker(path, self.state.bufnr, {
					bufname = self.state.bufname,
					winid = self.state.winid,
					preview = opts.preview,
					file_encoding = opts.file_encoding,
				})
			end

			local frame_size = {
				w = vim.api.nvim_win_get_width(self.state.winid),
				h = vim.api.nvim_win_get_height(self.state.winid),
			}

			local default_mono_font = (function()
				local handle = io.popen('fc-match -f "%{file}:%{size}\n" monospace')
				if not handle then
					return { family = nil }
				end

				local out = handle:read("*a")
				handle:close()

				local sep_pos = string.find(out, ":") --[[@as integer]]
				return {
					family = string.sub(out, 1, sep_pos),
					size = tonumber(string.sub(out, sep_pos)) or 12,
				}
			end)()

			local art = libasciify.Ascii.new({
				bitmap = bitmap,
				frame_size = frame_size,
				font = default_mono_font,
			})

			local bufnr = self.state.bufnr
			vim.schedule(function()
				local chan = vim.api.nvim_open_term(bufnr, {})
				if chan == 0 then
					return preview_error("Cannot open an internal terminal inside buffer #" .. self.state.bufnr)
				end

				local filters = {
					fill = function()
						art:apply_fill(fill)
					end,
					edge = function()
						art:apply_edge(edge)
					end,
					color = function()
						art:apply_color(color)
					end,
				}
				for _, v in ipairs(order) do
					local filter = filters[string.lower(v)] or function() end
					filter()
				end

				local out = art:write(frame_size, color_mode)
				vim.api.nvim_chan_send(chan, out)
			end)
		end,
	})
end, {})

local M = {}

M.asciify_previewer = asciify_previewer.new

return require("telescope").register_extension({
	setup = function(extconf)
		extconf.fill_palette = extconf.fill_palette or " .:+*csS&$@"

		extconf.edge_palette = extconf.edge_palette or "|\\`~;/"
		extconf.edge_threshold = extconf.edge_threshold or 4.5
		extconf.edge_dog_conf = extconf.edge_dog_conf or { eps = 1, p = 10 }

		extconf.color_mode = extconf.color_mode or libasciify.Ascii.ColorMode.INDEXED

		extconf.order = extconf.order or { "fill", "edge", "color" }

		fill = libasciify.FillFilter.new({ palette = extconf.fill_palette })
		color = libasciify.ColorFilter.new()
		edge = libasciify.EdgeFilter.new({
			threshold = extconf.edge_threshold,
			palette = extconf.edge_palette,
			dog = extconf.edge_dog_conf,
		})

		order = extconf.order
		color_mode = extconf.color_mode
	end,
	exports = M,
})
