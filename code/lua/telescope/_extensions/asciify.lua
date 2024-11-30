local telescope = require("telescope")
local from_entry = require("telescope.from_entry")
local conf = require("telescope.config").values
local defaulter = require("telescope.utils").make_default_callable
local previewers = require("telescope.previewers.buffer_previewer")
local Path = require("plenary.path")

local libasciify = require("asciify.libasciify")

local fill = libasciify.FillFilter.new({ palette = " .:+*csS&$@" })
local color = libasciify.ColorFilter.new()
local edge = libasciify.EdgeFilter.new({ threshold = 4.5, palette = "|\\`~;/" })

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

			local image = libasciify.Image.new({ path = path })
			if not image then
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

			local art = libasciify.AsciiArt.new({
				image = image,
				frame_size = frame_size,
				font_family = default_mono_font.family,
				font_size = default_mono_font.size,
			})

			local bufnr = self.state.bufnr
			vim.schedule(function()
				local chan = vim.api.nvim_open_term(bufnr, {})
				if chan == 0 then
					return preview_error("Cannot open an internal terminal inside buffer #" .. self.state.bufnr)
				end

				art:apply_fill(fill)
				art:apply_edge(edge)
				art:apply_color(color)

				local out = art:write(frame_size, libasciify.AsciiArt.ColorMode.INDEXED)
				vim.api.nvim_chan_send(chan, out)
			end)
		end,
	})
end, {})

local M = {}

M.asciify_previewer = asciify_previewer.new

return require("telescope").register_extension({
	exports = M,
})
