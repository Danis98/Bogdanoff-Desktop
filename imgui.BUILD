cc_library(
    name = "dear_imgui",
    srcs = [
        "imgui.cpp",
        "imgui_demo.cpp",
        "imgui_draw.cpp",
        "imgui_tables.cpp",
        "imgui_widgets.cpp",
        "backends/imgui_impl_sdl2.cpp",
        "backends/imgui_impl_opengl3.cpp",
    ],
    hdrs = [
        "imconfig.h",
        "imgui.h",
        "imgui_internal.h",
        "imstb_rectpack.h",
        "imstb_textedit.h",
        "imstb_truetype.h",
        "backends/imgui_impl_sdl2.h",
        "backends/imgui_impl_opengl3.h",
        "backends/imgui_impl_opengl3_loader.h",
    ],
    copts = [
        "-lGL",
        "-ldl",
        "-Iexternal/sdl2",
        "-D_REENTRANT",
    ],
    linkopts = [
        "-lGL",
        "-ldl",
        "-lSDL2",
    ],
    deps = [
        "@sdl2//:headers",
    ],
    visibility = ["//visibility:public"],
)
