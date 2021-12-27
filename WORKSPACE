load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

SKYLIB_VERSION = "1.0.3"

http_archive(
    name = "bazel_skylib",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/bazel-skylib/releases/download/{version}/bazel-skylib-{version}.tar.gz".format(version=SKYLIB_VERSION),
        "https://github.com/bazelbuild/bazel-skylib/releases/download/{version}/bazel-skylib-{version}.tar.gz".format(version=SKYLIB_VERSION),
    ],
)

# Replace with the LLVM commit you want to use.

# The easiest way to calculate this for a new commit is to set it to empty and
# then run a bazel build and it will report the digest necessary to cache the
# archive and make the build reproducible.

new_local_repository(
    name = "llvm-raw",
    build_file_content = "# empty",
    # Or wherever your submodule is located.
    path = "3rdparty/opt/llvm",
)

load("@llvm-raw//utils/bazel:configure.bzl", "llvm_configure", "llvm_disable_optional_support_deps")

llvm_configure(name = "llvm-project")

# Disables optional dependencies for Support like zlib and terminfo. You may
# instead want to configure them using the macros in the corresponding bzl
# files.
llvm_disable_optional_support_deps()