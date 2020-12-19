load("@rules_cc//cc:defs.bzl", "cc_binary")

filegroup(
    name = "input",
    srcs = ["input.txt"],
)

cc_binary(
    name = "day19",
    srcs = ["main.cc"],
    data = [":input"],
)
