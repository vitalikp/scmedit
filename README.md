# scmedit

# steps to building scmedit
 - download [scmedit](https://github.com/vitalikp/scmedit/archive/master.tar.gz);
 - unpack archive and go into directory;
 - create **build** directory;
 - run commands:
```
$ cd build
$ cmake ..
$ make
```

# quick start

1) build scmedit
2) export channel list to flash drive from TV
3) run next commands:
```
$ unzip channel_list_<model>_XXXX.scm map-CableD
$ ./src/scmedit --file <path>/map-CableD
$ # run some commands in cli (help, print, mv etc.)
$ # run write command to save changes
scmedit>write <path>/map-CableD
$ # update map-CableD file in scm file
$ zip channel_list_<model>_XXXX.scm map-CableD
```

4) import channel list from flash drive to TV
