# to-si
Command line too for converting from various units to SI.

Totally insecure, made this just for fun, please use a python library or something.

## Options:
```
-f function  set source unit
-a argument  source unit quantity, repeatable
-p           displays with maximum precision
-h           show help
-?           show help
```

## Source units:
```
  ftin   foot+inch, requires -a option twice
  ft     foot
  in     inch
  mi     mile
  nmi    nautical mile
  lb     pound mass
  oz     US customary fluid ounce
  ozku   imperial fluid ounce
  F      fahrenheit
  K      kelvin
```

Exit codes:
```
0      Terminated without errors
1      Missing option argument
2      Function argument invalid
3      Too many argument options
4      Missing function option
```
