# C implementation of the Solitaire game

This is a project I made during my first semester at [ENSIIE](http://www.ensiie.fr) to learn the C language.

The subject was the following !

* Code a solitaire game (release 1.0);
* Recode it using modules and add the ability for the user to import his own checkerboard (release 2.0);
* Recode it using [ncurses](http://www.gnu.org/software/ncurses/) (release 3.0).

## Compilation
Very simply, using make : `$ make`.  
If you want to get all the debug feature and symbols, just call the `dev` target instead : `$ make dev`

## Usage
`$ ./solitaire [options]`

Options:  
 `-h` Prints this help and exits.  
 `-n` Allows vertical and horizontal moves (default).  
 `-d` Allows diagonal moves.   
 `<str>` Path to the configuration file (read nex section).

## Configuration file
The game uses the following checkerboard as the default :
<pre>
     A  B  C  D  E  F  G
   ---------------------
1 |           #
2 |     #  #  #  #  #
3 |     #     #     #
4 |  #  #  #  -  #  #  #
5 |     #     #     #
6 |     #  #  #  #  #
7 |           #
</pre>
You can specifiy the checkerboard you wan to play on by passing its path as an argument of the application. The format should be the following :  

* `*` for a case that is occupied by a pawn (`#`);
* `.` for an empty case (`-`);
* `V` or `v` for a case that isn't playable.

For example, this model :
<pre>
**.**
**.** 
**v**
</pre>
will give the following checkerboard :
<pre>
     A  B  C
   ----------
1 |  #  -  #
2 |  #  -  #
3 |  #     #
</pre>

**Beware !** Configuration file should not exceed 26 lines in height and 26 columns in width.

## Copying / License
This free software is provided under the terms of the [GNU General Public License 2](https://www.gnu.org/licenses/gpl-2.0.html). [A copy](LICENSE) of the license is integrated in the repo.