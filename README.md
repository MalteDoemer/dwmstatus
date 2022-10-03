# DWM Status

This is a program to display a status bar for the dwm window manager written in c.
It is simmilar to [slstatus](https://tools.suckless.org/slstatus/) and [dwmblocks](https://github.com/torrinfail/dwmblocks) but adjusted to my preferences.

## Configuration

The configuration is done simmilar to dwm, in the config.h file.

### Not Available String
The `not_available` string in config.h will be displayed if a function failes to provide a value.

### Status Line String
The `status_line` string in config.h is a `printf` style format string that is used to define the overall format of the status bar. 

The format is as follows:
- `%s` will be replaced by the corresponding block from the `blocks` array.
- `%%` will be replaced by a literal `%`
- any other characters will be simply copied over

### Blocks Array
The `blocks` array in config.h defines what to show in the `%s` format specifiers of the `status_line` string.

Every block consists of these parts:
- __function__: A function from blocks.h that will be used
- __argument__: A string used as first argument for this function. Can sometimes be NULL.
- __interval__: How many seconds should be waited to rerun this block. The value `0` means never rerun.
- __signal__: A signal number with wich the function should be rerun. For example with signal=3 the function can be executed using the command `pkill -RTMIN+3 dwmstatus`. This is useful for information that changes on demand like volume or the keyboard layout.

### Colors

Colors are implemented using the statuscolors path in dwm. When using this patch, one can write the character `\x01` to the status bar and the first color schema will be selected. The character `\x02` selects the second `\x03` the third and so on. The default dwm only has a two color schemes (selected and normal). So if you want to use colors, you have to adjust your dwm and colors.h. To actually enable colors uncomment the `#define NO_COLORS` in config.h.

## License

Licensed under the MIT license ([LICENSE-MIT](LICENSE-MIT) or http://opensource.org/licenses/MIT)