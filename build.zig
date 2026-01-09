const std = @import("std");
const builtin = @import("builtin");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});
    const allocator = std.heap.page_allocator;

    const exe = b.addExecutable(.{
        .name = "ctodo",
        .root_module = b.createModule(.{
            .target = target,
            .optimize = optimize,
            .link_libc = true,
        }),
    });

    if (builtin.zig_version.major == 0 and builtin.zig_version.minor < 15) {
        @compileError("Building requires Zig 0.15.1 or later");
    }


    exe.root_module.linkSystemLibrary("ncursesw", .{});

    var dir = std.fs.cwd().openDir("./src/", .{ .iterate = true }) catch {
        std.process.exit(1);
    };
    defer dir.close();
    var dir_iterator = dir.iterate();


    while (dir_iterator.next() catch std.process.exit(1)) |entry| {
        const file = std.fmt.allocPrint(allocator, "src/{s}", .{ entry.name }) catch std.process.exit(1);
        exe.addCSourceFile(.{
            .file = b.path(file),
            .flags = &.{ "-Wno-date-time" },
        });

        allocator.free(file);
    }
    exe.addIncludePath(b.path("include/"));


    b.installArtifact(exe);

}
