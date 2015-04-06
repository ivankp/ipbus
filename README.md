To compile executables run `make`. This will create static objects and compile executables.
To create shared objects run `make shared`.
To install run `make install prefix=some/dir/path`

Edit PYTHON_INC variable in the Makefile if you are using a different python version.
Python is only used for the Herakles module.

You can get the original Carlos' ipbus from:
svn co https://svn.cern.ch/reps/atlasgroups/Detectors/TileCal/Prometeo/ipbus/trunk ipbus
This fork is all the same, but just has a nice Makefile.
