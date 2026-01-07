# ctodo
Simple ncurses-based todo application written in C


## Building
### Debug Build
Requires ncurses-lib and [tup](https://gittup.org/tup)

```sh
tup init
tup
```

### Release Build
Requires ncurses-lib and [zig](https://ziglang.org)

```sh
zig build -Doptimize=ReleaseSmall
```


## License 
See [LICENSE](./LICENSE) file
