# Call of Duty Extended

CoDExtended is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

====

Call of Duty Extended is a modification for Call of Duty 1, which does what it says it extends CoD with new features, increasing limits and fixing bugs. 

# Update

With this custom version of CoDExtended, bugs are fixed, crashes are eliminated, unnecessary things are removed, and chat commands are redone.

# Usage

Your startup commandline for your Call of Duty dedicated server would look something like this.
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:. LD_PRELOAD=/path/to/codextended.so ./cod_lnxded

# Building

Install GCC and libs if you haven't yet with
apt-get install build-essential
run build.sh
[ If you're on x86_64 then you would add :i386 after the library package names ]

OR use the codextended.so (no debug and no mysql) in /code/bin/

# Questions

If you have any questions, add me on steam.

Steam: indyanz

# Bugs

- Invisible models (kinda rare)

====

Thanks to php (cod1.eu),  and kung foo man (http://killtube.org) for his libcod project (CoD 2 memory modification) (https://github.com/kungfooman/libcod)
