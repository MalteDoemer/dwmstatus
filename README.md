# DWM Status

This is a program to display a status bar for the dwm window manager written in c.
It is simmilar to [slstatus](https://tools.suckless.org/slstatus/) and [dwmblocks](https://github.com/torrinfail/dwmblocks) but adjusted to my preferences.

## Patches 
In order for this program to work I wrote my own dwm patch that combines status colors and the ability to click on blocks. The patch is available [here](https://github.com/MalteDoemer/dwm/blob/master/patches/dwm-statuscolorsclick.diff).

## Configuration

The configuration is done similar to dwm, in the config.h file.

### Not Available String
The `not_available` string in config.h will be displayed if a function fails to provide a value.

### Blocks Array
The `blocks` array in config.h defines what to show in the status bar.

Every block consists of these parts:
- __format__: A printf style format specifier that must include at most one `%s`
- __function__: A function from blocks.h that will be used or `NULL`
- __argument__: A string used as first argument for this function. Can sometimes be `NULL`.
- __on click__: A shell command that should be run if the block was clicked or `NULL`.
- __interval__: How many seconds should be waited to rerun this block. The value `0` means never rerun.
- __signal__: A signal number with which the function should be rerun. For example with signal=3 the function can be executed using the command `pkill -RTMIN+3 dwmstatus`. This is useful for information that changes on demand like volume or the keyboard layout.

### Colors

When using my dwm patch (based on statuscolors), one can write the character `\x01` to the status bar and the first color schema will be selected. The character `\x02` selects the second `\x03` the third and so on.
## License

Licensed under the MIT license ([LICENSE-MIT](LICENSE-MIT) or http://opensource.org/licenses/MIT)