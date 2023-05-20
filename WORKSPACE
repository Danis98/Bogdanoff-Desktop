load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "dear_imgui",
    url = "https://github.com/ocornut/imgui/archive/refs/tags/v1.89.5.zip",
    sha256 = "36a4ef22f7bec177a82bdbeac3342d4d10ff74903b571208d415aeeb5e6e1fad",
    strip_prefix = "imgui-1.89.5",
    build_file = "//:imgui.BUILD",
)

new_local_repository(
    name = "sdl2",
    path = "/usr/include/SDL2",
    build_file_content = """
package(default_visibility = ["//visibility:public"])
cc_library(
    name = "headers",
    hdrs = glob(["**/*.h"]),
    includes = [
        ".",
    ],
)
"""
)