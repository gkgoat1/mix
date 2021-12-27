cc_library(name = "mix",hdrs = ["mix.hpp"],visibility = ["//visibility:public"])
cc_binary(name = "example",srcs = ["example.cpp"],deps = [":mix"])
cc_binary(name = "bf2m",srcs = ["bf2m.cpp"],visibility = ["//visibility:public"],deps=[":mix"])