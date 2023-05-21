cc_library(
    name = "http",
    srcs = [
        "http_session.cpp",
        "http_util.cpp",
    ],
    hdrs = [
        "http_session.h",
        "http_util.h",
    ],
    copts = [
        "-lcurl"
    ],
    linkopts = [
        "-lcurl"
    ],
    deps = [
        "//json",
    ],
    visibility = ["//visibility:public"]
)