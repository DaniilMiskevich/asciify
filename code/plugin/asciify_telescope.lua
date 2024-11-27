local previewers = require("telescope.previewers")

-- local asciify_image_previewer = previewers.new_buffer_previewer({
-- 	title = "ASCIIfied",
-- 	define_preview = function(self, _, _)
-- 		vim.api.nvim_buf_set_lines(self.state.bufnr, 0, -1, false, { "hello", "world" })
-- 	end,
-- })

local conf = require("telescope.config").values
local from_entry = require("telescope.from_entry")

local function defaulter(f, default_opts)
	default_opts = default_opts or {}
	return {
		new = function(opts)
			if conf.preview == false and not opts.preview then
				return false
			end
			opts.preview = type(opts.preview) ~= "table" and {} or opts.preview
			if type(conf.preview) == "table" then
				for k, v in pairs(conf.preview) do
					opts.preview[k] = vim.F.if_nil(opts.preview[k], v)
				end
			end
			return f(opts)
		end,
		__call = function()
			local ok, err = pcall(f(default_opts))
			if not ok then
				error(debug.traceback(err))
			end
		end,
	}
end

local asciify_image_previewer = defaulter(function(opts)
	opts = opts or {}
	return previewers.new_buffer_previewer({
		title = "File Preview",
		dyn_title = function(_, _)
			return "===== WORKING TEST TITLE"
		end,

		get_buffer_by_name = function(_, entry)
			return from_entry.path(entry, false, false)
		end,

		define_preview = function(self, entry)
			local path = from_entry.path(entry, false, false)
			if path == nil or path == "" then
				return
			end

			local asciify = require("asciify.libasciify")

			local scale = 1.25

			local fill = asciify.FillFilter.new({ palette = " .:+*csS&$@" })
			local color = asciify.ColorFilter.new()
			local edge = asciify.EdgeFilter.new({ threshold = 4.5, palette = "|\\`~;/" })

			local image = asciify.Image.new({ path = path })
			if not image then
				vim.api.nvim_buf_set_lines(self.state.bufnr, 0, -1, false, { "Image failed to load: ", path })
				return
			end

			local art = asciify.AsciiArt.new({ image = image, char_size = { w = 10 / scale, h = 22 / scale } })
			art:apply_fill(fill)
			art:apply_edge(edge)
			art:apply_color(color)

			local out = art:write(asciify.AsciiArt.ColorMode.TRUE)
			vim.api.nvim_buf_set_lines(self.state.bufnr, 0, -1, false, out)
		end,
	})
end, {})

return asciify_image_previewer.new
