load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "bogdanoff",
    srcs = [
        "bogdanoff.cpp"
    ],
    deps = [
        "//chrono:timing",
        "//http",
        "//client:account",
        "//price",
        "//gui",
        "@dear_imgui//:dear_imgui",
    ],
    visibility = ["//visibility:public"]
)