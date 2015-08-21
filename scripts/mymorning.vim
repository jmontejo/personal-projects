" Vim color file
" This color scheme uses a light grey background.

" First remove all existing highlighting.
set background=light
hi clear
if exists("syntax_on")
  syntax reset
endif

let colors_name = "mymorning"

"hi Normal ctermfg=Black ctermbg=LightGrey guifg=Black guibg=grey90
"hi Normal ctermfg=White guifg=White guibg=grey90
"hi Normal ctermfg=Black guifg=Black guibg=grey90

" Groups used in the 'highlight' and 'guicursor' options default value.
hi ErrorMsg     term=standout ctermbg=DarkRed ctermfg=White guibg=DarkRed guifg=White
hi IncSearch    term=reverse cterm=reverse gui=reverse
hi ModeMsg      term=bold cterm=bold gui=bold
hi StatusLine   term=reverse,bold cterm=reverse,bold ctermfg=Brown gui=reverse,bold
hi StatusLineNC term=reverse cterm=reverse gui=reverse
hi VertSplit term=reverse cterm=reverse gui=reverse
hi Visual term=reverse cterm=reverse gui=reverse
hi VisualNOS term=underline,bold cterm=underline,bold gui=underline,bold
hi Cursor guibg=Green guifg=NONE
hi lCursor guibg=Cyan guifg=NONE
hi Directory term=bold ctermfg=DarkBlue guifg=Blue
hi LineNr term=underline ctermfg=Brown guifg=Brown
hi MoreMsg term=bold ctermfg=DarkGreen gui=bold guifg=SeaGreen
hi NonText term=bold ctermfg=Blue gui=bold guifg=Blue guibg=grey80
hi Question term=standout ctermfg=DarkGreen gui=bold guifg=SeaGreen
hi Search       term=reverse ctermbg=Yellow ctermfg=White guibg=Yellow guifg=White
hi SpecialKey term=bold ctermfg=DarkBlue guifg=Blue
hi Title term=bold ctermfg=DarkMagenta gui=bold guifg=Magenta
hi WarningMsg term=standout ctermfg=DarkRed guifg=Red
hi WildMenu term=standout ctermbg=Yellow ctermfg=Black guibg=Yellow guifg=Black
hi Folded term=standout ctermbg=Grey ctermfg=DarkBlue guibg=LightGrey guifg=DarkBlue
hi FoldColumn term=standout ctermbg=Grey ctermfg=DarkBlue guibg=Grey guifg=DarkBlue
hi DiffText term=reverse cterm=bold ctermfg=White ctermbg=Red gui=bold guibg=Red
hi DiffAdd term=bold ctermfg=White ctermbg=LightBlue guibg=LightBlue
hi DiffChange term=bold ctermfg=White ctermbg=LightMagenta guibg=LightMagenta
hi DiffDelete term=bold ctermfg=Blue ctermbg=LightCyan gui=bold guifg=Blue guibg=LightCyan
hi CursorLine term=underline cterm=underline guibg=grey80
hi CursorColumn term=reverse ctermbg=grey guibg=grey80

" Colors for syntax highlighting
hi Constant     term=underline ctermfg=DarkRed guifg=Magenta guibg=grey95
hi Special      term=bold ctermfg=DarkMagenta guifg=SlateBlue guibg=grey95
hi Statement    term=bold cterm=bold ctermfg=Brown gui=bold guifg=Brown
hi Ignore       ctermfg=LightGrey guifg=grey90

" vim: sw=2
