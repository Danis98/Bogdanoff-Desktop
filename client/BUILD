cc_library(
    name = "account",
    srcs = [
        "blockchain_info_client.cpp",
        "client_factory.cpp",
        "client.cpp",
        "coinex_client.cpp",
        "etherscan_client.cpp",
    ],
    hdrs = [
        "blockchain_info_client.h",
        "client_factory.h",
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