load("@rules_cc//cc:defs.bzl", "cc_binary")

filegroup(
    name = "input",
    srcs = ["input.txt"],
)

cc_binary(
    name = "day21",
    srcs = ["main.cc"],
    data = [":input"],
)
