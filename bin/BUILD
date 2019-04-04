
cc_binary(
    name = "hyperscale",
    srcs = glob([
        "hyperscale-cmd/**/*.cpp",
        "hyperscale-cmd/**/*.hpp"
    ]),
    copts = ["-Iinclude -I/usr/local/include"],
    deps = [
        "//lib:hyperscale",
        "@hyperconsole//:hyperconsole"
    ],
)
