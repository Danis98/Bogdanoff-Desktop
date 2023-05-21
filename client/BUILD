cc_library(
    name = "account",
    srcs = [
        "client.cpp",
        "coinex_client.cpp",
        "etherscan_client.cpp",
    ],
    hdrs = [
        "client.h",
        "coinex_client.h",
        "etherscan_client.h",
    ],
    deps = [
        "//chrono:timing",
        "//crypto:md5",
        "//http",
        "//json",
    ],
    visibility = ["//visibility:public"]
)